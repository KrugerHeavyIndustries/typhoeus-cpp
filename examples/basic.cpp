#include "typhoeus/typhoeus.h" 

int main(int argc, char* argv[]) {

  using namespace typhoeus;

  Curl::init();
  
  {
    Response response = Typhoeus::get("https://httpbin.org/get");

    printf("Response from typhoeus::get\n");
    printf("%s", response.body.c_str());
  }

  Curl::cleanup();

  return 0;
}
