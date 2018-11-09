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
#ifndef _TYPHOEUS_TYPHOEUS_H_
#define _TYPHOEUS_TYPHOEUS_H_

#include <string>
#include <map>
#include <vector>
#include <deque>

#include "curl.h"
#include "hydra.h"

namespace typhoeus {

enum Method {
   GET,
   POST,
   PUT,
   DELETE,
   HEAD
};

struct Options {
   Method method;
   bool verbose;
   int maxRedirect;
   int timeout;
   bool followRedirects;
   bool noSignal;
   std::string certificateAuthorityPath;
   std::string certificatePath;
   std::string certificateType;
   std::string certificateKeyPath;
   std::string certificateKeyPassword;
   std::string proxyUri;
   std::string body;
};

class Response;

typedef std::map<std::string, std::string> Headers;

class Request {

   friend class Hydra;

public:

   static const Options DEFAULT_OPTIONS;

   Request(const std::string& baseUrl, const Options& options = DEFAULT_OPTIONS);

   Response run();

   void finish(const Response& response) const;

   void (*onComplete)(const Response&);
   void (*onSuccess)(const Response&);
   void (*onFailure)(const Response&);
   void (*onHeaders)(const Response&);
   void (*onProgress)(const Response&);

private:

   void executeCallbacks(const Response& response) const;

   const std::string m_baseUrl;
   const Options& m_options;
};

class Response {
public:
   Headers headers; 
   std::string body;
   int status;
};

struct Typhoeus {
   static Response get(const std::string& url, const Options& options = Request::DEFAULT_OPTIONS);
   static Response post(const std::string& url, const std::string& contentType, const std::string& data, const Options& options = Request::DEFAULT_OPTIONS);
   static Response del(const std::string& url, const Options& options = Request::DEFAULT_OPTIONS);
   static Response head(const std::string& url, const Options& options = Request::DEFAULT_OPTIONS);
};

} /* namespace typhoeus */
#endif /* _TYPHOEUS_H_ */
