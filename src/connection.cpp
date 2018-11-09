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

#include "connection.h"

#include <curl/curl.h>

#include "typhoeus/hydra.h"

#include "curl.h"
#include "pool.h"
#include "callbacks.h"

namespace typhoeus {

const char* DEFAULT_USER_AGENT = "typhoeus-cpp";

Connection::Connection() 
   :  m_handle(nullptr),
      m_hydra(nullptr),
      m_current(nullptr) {
      Curl::init();   
      m_handle = curl_easy_init();
   }

Connection::~Connection() {
   curl_easy_cleanup(m_handle);
}

void Connection::setUrl(const std::string& url) {
   m_url = url;
   curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str());
}

Response Connection::get(const std::string& url, const Options& options) {
   setup(url, GET, options);
   return perform(url); 
}

Response Connection::post(const std::string& url, const std::string& data, const Options& options) {
   curl_easy_setopt(m_handle, CURLOPT_POST, 1L);
   curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, data.c_str());
   curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, data.size());
   setup(url, POST, options);
   return perform(url);
}

Response Connection::put(const std::string& url, const std::string& data, const Options& options) {
   /* what to do here */ 
   setup(url, PUT, options);
   return perform(url);
}

Response Connection::del(const std::string& url, const Options& options) {
   setup(url, DELETE, options);
   return perform(url);
}

Response Connection::head(const std::string& url, const Options& options) {
   setup(url, HEAD, options);
   return perform(url);
}

void Connection::setup(const std::string& url, Method method, const Options& options) {
   m_url = url;
   setupMethod(method, options);

   curl_easy_setopt(m_handle, CURLOPT_URL, m_url.c_str());
   curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION,
                callbacks::body);
   curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, &m_response);

   curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION,
                callbacks::header);
   curl_easy_setopt(m_handle, CURLOPT_DEBUGFUNCTION, callbacks::debug);
   curl_easy_setopt(m_handle, CURLOPT_HEADERDATA, &m_response);

   curl_easy_setopt(m_handle, CURLOPT_USERAGENT, DEFAULT_USER_AGENT);

   if (options.verbose) {
      curl_easy_setopt(m_handle, CURLOPT_VERBOSE, 1L);
   }

   if (options.timeout) {
      curl_easy_setopt(m_handle, CURLOPT_TIMEOUT, options.timeout);
      curl_easy_setopt(m_handle, CURLOPT_NOSIGNAL, 1);
   }
   
   if (options.followRedirects) {
      curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(m_handle, CURLOPT_MAXREDIRS, options.maxRedirect);
   }
   
   if (options.noSignal) {
      curl_easy_setopt(m_handle, CURLOPT_NOSIGNAL, 1);
   }

   if (!options.certificateAuthorityPath.empty()) {
      curl_easy_setopt(m_handle, CURLOPT_CAINFO, options.certificateAuthorityPath.c_str());
   }

   if (!options.certificatePath.empty()) {
      curl_easy_setopt(m_handle, CURLOPT_SSLCERT, options.certificatePath.c_str());
   }

   if (!options.certificateType.empty()) {
      curl_easy_setopt(m_handle, CURLOPT_SSLCERTTYPE, options.certificateType.c_str());
   }

   if (!options.certificateKeyPath.empty()) {
      curl_easy_setopt(m_handle, CURLOPT_SSLKEY, options.certificateKeyPath.c_str());
   }

   if (!options.certificateKeyPassword.empty()) {
      curl_easy_setopt(m_handle, CURLOPT_KEYPASSWD, options.certificateKeyPassword.c_str());
   }

   if (!options.proxyUri.empty()) {
      curl_easy_setopt(m_handle, CURLOPT_PROXY, options.proxyUri.c_str());
      curl_easy_setopt(m_handle, CURLOPT_HTTPPROXYTUNNEL, 1L);
  }
}

void Connection::setupMethod(Method method, const Options& options) {
   switch (method) {
   case GET:
      break;
   case POST: 
      curl_easy_setopt(m_handle, CURLOPT_POST, 1L);
      curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, options.body.c_str());
      curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE, options.body.size());
      break;
   case PUT:
      // TODO
      break;
   case DELETE:
      curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
      break;
   case HEAD:
      curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST, "HEAD");
      curl_easy_setopt(m_handle, CURLOPT_NOBODY, 1L);
   default:
      // nothing to do
      break;
   }
}

void Connection::reset() {
   curl_easy_reset(m_handle);
   m_hydra = nullptr;
   m_current = nullptr;
   m_response.headers.clear();
   m_response.body.clear();
   m_response.status = 0;
}

void Connection::complete(){
   if (m_current) {
      m_current->finish(m_response);
   }
   Pool::release(this); 
   if (m_hydra && m_hydra->queueCount() > 0) {
      m_hydra->dequeueMany();
   }
}

Response Connection::perform(const std::string& url) {
   CURLcode rv = CURLE_OK;
   rv = curl_easy_perform(m_handle);
   return m_response;
}

} // 
