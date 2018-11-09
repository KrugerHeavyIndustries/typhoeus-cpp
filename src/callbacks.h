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
#ifndef _TYPHOEUS_CALLBACKS_H_
#define _TYPHOEUS_CALLBACKS_H_

#include "typhoeus/typhoeus.h"

#include <cstring>

namespace typhoeus {
namespace callbacks {

  inline std::string& trimLeft(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
          std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }

  inline std::string& trimRight(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
          std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }

  inline std::string& trim(std::string& s) {
    return trimLeft(trimRight(s));
  }

  inline size_t body(void* data, size_t size, size_t num, void* userdata) {
    Response* response; 
    response = reinterpret_cast<Response*>(userdata);
    response->body.append(reinterpret_cast<char*>(data), size * num);
    return (size * num);
  }

  inline size_t header(void* data, size_t size, size_t num, void* userdata) {
    Response* response;
    response = reinterpret_cast<Response*>(userdata); 
    std::string header(reinterpret_cast<char*>(data), size * num);
    size_t seperator = header.find_first_of(':');
    if (std::string::npos == seperator) {
      trim(header);
      if (0 == header.length()) {
        return (size * num);  // blank line;
      }
      response->headers[header] = "present";
    } else {
      std::string key = header.substr(0, seperator);
      trim(key);
      std::string value = header.substr(seperator + 1);
      trim(value);
      response->headers[key] = value;
    }
    return (size * num); 
  }

  inline size_t debug(void* handle, curl_infotype type, char* data, size_t size, void* userdata) {
    std::string msg(data, size);
    if (type == CURLINFO_DATA_IN || type == CURLINFO_DATA_OUT) {
      printf("%s\n", msg.c_str());
    }
    return 0;
  }
} /* namespace callbacks */
} /* namespace typhoeus */

#endif /* _TYPHOEUS_CALLBACKS_H_ */
