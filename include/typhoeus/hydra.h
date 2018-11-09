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
#ifndef _TYPHOEUS_HYDRA_H_
#define _TYPHOEUS_HYDRA_H_

#include <deque>

namespace typhoeus {

class Request;
class MultiConnection;

class Hydra {
public:
   Hydra(int maxConcurrency = DEFAULT_MAX_CONCURRENCY);
   ~Hydra();

   int queueCount() const;

   void queue(const Request* request);

   void dequeueMany();

   void run();

   void abort();

private:

   static const int DEFAULT_MAX_CONCURRENCY;

   void add(const Request* request);

   void dequeue();

   int m_maxConcurrency;
   MultiConnection* m_multi; 
   std::deque<const Request*> m_queue;

};

} /* namespace typhoeus */

#endif /* _TYPHOEUS_HYDRA_H_ */
