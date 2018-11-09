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
#include "typhoeus/hydra.h"
#include "typhoeus/typhoeus.h"

#include "pool.h"
#include "connection.h"
#include "multiconnection.h"

namespace typhoeus {

const int Hydra::DEFAULT_MAX_CONCURRENCY = 20;

Hydra::Hydra(int maxConcurrency)
   :  m_maxConcurrency(maxConcurrency),
      m_multi(new MultiConnection()) {
}

Hydra::~Hydra() {
   delete m_multi;
}

int Hydra::queueCount() const {
   return m_queue.size();
}

void Hydra::queue(const Request* request) {
   m_queue.push_back(request);
}

void Hydra::run() {
   dequeueMany();
   m_multi->perform();
}

void Hydra::abort() {
   //m_queue.clear();
}

void Hydra::add(const Request* request) {
   Connection* connection = Pool::get();
   connection->setup(request->m_baseUrl, request->m_options.method, request->m_options);
   connection->m_hydra = this;
   connection->m_current = request;
   m_multi->add(connection);
}

void Hydra::dequeue() {
   if (m_queue.size() > 0) {
      add(m_queue.front());
      m_queue.pop_front();
   }
}

void Hydra::dequeueMany() {
   int number = m_multi->count();
   while (number < m_maxConcurrency && m_queue.size() > 0) {
      add(m_queue.front());
      m_queue.pop_front();
      number += 1;
   }
}

} /* namespace typhoeus */
