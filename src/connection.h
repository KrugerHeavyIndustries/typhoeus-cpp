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
#ifndef _TYPHOPHEUS_CONNECTION_H_
#define _TYPHOPHEUS_CONNECTION_H_

#include <string>
#include <curl/curl.h>

#include "typhoeus/typhoeus.h"

namespace typhoeus {

class Hydra;
class Request;

class Connection {

friend class MultiConnection;
friend class Hydra;

public:

   Connection();
   ~Connection();

   void setUrl(const std::string& url);

   Response get(const std::string& url, const Options& options = Request::DEFAULT_OPTIONS);

   Response post(const std::string& url, const std::string& data, const Options& options = Request::DEFAULT_OPTIONS);

   Response put(const std::string& url, const std::string& data, const Options& options = Request::DEFAULT_OPTIONS);

   Response del(const std::string& url, const Options& options = Request::DEFAULT_OPTIONS);
   
   Response head(const std::string& url, const Options& options = Request::DEFAULT_OPTIONS);

   void setup(const std::string& url, Method method, const Options& options);

   void setupMethod(Method method, const Options& options);

   void reset();

   void complete();

private:

   Response perform(const std::string& url);

   CURL* m_handle;
   Hydra* m_hydra;
   const Request* m_current;
   Response m_response;

   std::string m_url;
};

} //

#endif /* _TYPHOPHEUS_CONNECTION_H_ */
