#include "curl.h"

namespace typhoeus {

std::mutex Curl::ms_mutex;
bool Curl::ms_initialized = false;

bool Curl::init() {
   std::lock_guard<std::mutex> lock(ms_mutex); 
   if (!ms_initialized) {
      if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
         return false;
      }
      ms_initialized = true;
   }
   return true;
}

bool Curl::cleanup() {
   std::lock_guard<std::mutex> lock(ms_mutex); 
   if (ms_initialized) {
      curl_global_cleanup();
      ms_initialized = false;
   }
   return true;
}

} /* namespace typhoeus */
