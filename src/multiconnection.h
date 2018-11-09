// vim:set et ts=3 sw=3:
// __  __ ______ _______ _______ _______ ______ 
// |  |/  |   __ \   |   |     __|    ___|   __ \
// |     <|      <   |   |    |  |    ___|      <
// |__|\__|___|__|_______|_______|_______|___|__|
//        H E A V Y  I N D U S T R I E S
//
// Copyright (C) 2018 Kruger Heavy Industries
// http://www.krugerheavyindustries.com
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
#ifndef _TYPHOEUS_MULTICONNECTION_H_
#define _TYPHOEUS_MULTICONNECTION_H_

#include <unistd.h>

namespace typhoeus { 

class MultiConnection {

public:

   MultiConnection() 
      :  m_handle(nullptr),
         m_running(0) {
      Curl::init();
      m_handle = curl_multi_init();
   }

   ~MultiConnection() {
      cleanup();
      curl_multi_cleanup(m_handle);
   }

   void perform() {
      while (isOngoing()) {
         run();
         long timeout = getTimeout();
         if (timeout == 0) {
            continue;
         }
         reset();
         setFds(timeout);
      }
   }

   void add(Connection* connection) {
      CURLMcode code = curl_multi_add_handle(m_handle, connection->m_handle);
      if (code == CURLM_OK) {
         m_connections.push_back(connection);
      }
   }

   void del(Connection* connection) {
      m_connections.erase(std::find(m_connections.begin(), m_connections.end(), connection));
      CURLMcode code = curl_multi_remove_handle(m_handle, connection->m_handle);
      if (code != CURLM_OK) {
         printf("failed to remove a curl handle from multi curl\n");
      }
      m_defunct.push_back(connection);
   }

   int count() const { 
      return m_connections.size();
   }

private: 

   void run() {
      CURLMcode code = CURLM_CALL_MULTI_PERFORM;
      do {
         code = curl_multi_perform(m_handle, &m_running);
      } while (code == CURLM_CALL_MULTI_PERFORM);
      check();
   }

   void check() {
      int messageCount;
      int loopCount = 0;
      while (true) {
         CURLMsg* msg = curl_multi_info_read(m_handle, &messageCount);
         if (msg == nullptr)
            break;
         if (msg->msg != CURLMSG_DONE) 
            continue;
         std::vector<Connection*>::iterator found = std::find_if(m_connections.begin(), m_connections.end(), [msg](Connection * c) {
            return c->m_handle == msg->easy_handle;
         });
         if (found != m_connections.end()) {
            //(*found)->code = msg->data.code;
            (*found)->complete();
            del(*found);
         }
      }
   }

   long getTimeout() const {
      long timeout;
      CURLMcode code = curl_multi_timeout(m_handle, &timeout);
      if (code != CURLM_OK) {
         printf("curl_multi_timeout was not OK\n");
         return -1;
      }
      if (timeout < 0) {
         timeout = 100;
      }
      return timeout;
   }

   void reset() {
      FD_ZERO(&m_readset);
      FD_ZERO(&m_writeset);
      FD_ZERO(&m_errorset);
   }

   void setFds(long timeout) {
      int rc;
      int maxFd;
      CURLMcode code = curl_multi_fdset(m_handle, &m_readset, &m_writeset, &m_errorset, &maxFd);
      if (code != CURLM_OK) {
         printf("curl_multi_fdset was not OK\n");
         // error 
      }
      struct timeval wait = { timeout / 1000, static_cast<suseconds_t>((timeout % 1000) * 1000000) };
      if (maxFd == -1) {
         usleep(100);
      } else {
         do {
            rc = select(maxFd + 1, &m_readset, &m_writeset, &m_errorset, &wait);
            if (rc < 0 && errno == EINTR)
               continue;
            break;
         } while (true);
      }
   } 

   bool isOngoing() {
      return m_connections.size() > 0 || m_running > 0;
   }

   void cleanup() {
      for (std::vector<Connection*>::iterator it = m_defunct.begin(); it != m_defunct.end(); ++it) {
         delete (*it);
      }
      m_defunct.clear();
   }

   CURLM* m_handle;
   fd_set m_readset;
   fd_set m_writeset;
   fd_set m_errorset;
   int m_running;
   std::vector<Connection*> m_connections;
   std::vector<Connection*> m_defunct;
};

} /* namespace typhoeus */
#endif /* _TYPHOEUS_MULTICONNECTION_H_ */
