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
#include "typhoeus/typhoeus.h"

#include <vector>

#include "curl.h"
#include "pool.h"
#include "callbacks.h"
#include "connection.h"

namespace typhoeus {

const Options Request::DEFAULT_OPTIONS = { GET, false, 50, 0, false, true, "", "", "", "", "", "" };

Request::Request(const std::string& baseUrl, const Options& options) 
   :  onComplete(nullptr),
      onSuccess(nullptr),
      onFailure(nullptr),
      onHeaders(nullptr),
      onProgress(nullptr),
      m_baseUrl(baseUrl),
      m_options(options) {
}

Response Request::run() { 
   Response response;
   Connection* connection = Pool::get();
   switch (m_options.method) { 
   case GET:
      response = connection->get(m_baseUrl, m_options);
      break;
   case POST:
      response = connection->post(m_baseUrl, m_options.body);
      break;
   case PUT:
      response = connection->put(m_baseUrl, m_options.body);
      break;
   case DELETE:
      response = connection->del(m_baseUrl);
      break;
   case HEAD:
      response = connection->head(m_baseUrl);
      break;
   };
   Pool::release(connection);
   executeCallbacks(response);
   return response;
}

void Request::finish(const Response& response) const {
   executeCallbacks(response);
}

void Request::executeCallbacks(const Response& response) const {
   if (onHeaders) {
      onHeaders(response);
   }
   if (onComplete) {
      onComplete(response);
   }
}

Response Typhoeus::get(const std::string& url, const Options& options) {
   Response response;
   Connection* connection = Pool::get();
   response = connection->get(url, options); 
   Pool::release(connection);
   return response;
}

Response Typhoeus::post(const std::string& url, const std::string& contentType, const std::string& data, const Options& options) {
   Response response;
   Connection* connection = Pool::get();
   response = connection->post(url, data, options);
   Pool::release(connection);
   return response;
}

Response Typhoeus::del(const std::string& url, const Options& options) {
   Response response;
   Connection* connection = Pool::get();
   response = connection->del(url, options);
   Pool::release(connection);
   return response;
}

Response Typhoeus::head(const std::string& url, const Options& options) {
   Response response;
   Connection* connection = Pool::get();
   response = connection->head(url, options);
   Pool::release(connection);
   return response;
}

} /* namespace typhoeus */
