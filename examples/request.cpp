#include "typhoeus/typhoeus.h"

int main(int argc, char* argv[]) {

  using typhoeus::Request;
  using typhoeus::Response;
  using typhoeus::Options;

  typhoeus::Curl::init();
  
  {
    Request request("https://httpbin.org/get");
    Response response = request.run();

    printf("A basic get request\n");
    printf("%s", response.body.c_str());
  }

  { 
    Options options = Request::DEFAULT_OPTIONS;
    options.method = typhoeus::POST;

    Request request("https://httpbin.org/post", options);

    request.onComplete = [](const Response& response) { 
      printf("A basic post request\n"); 
      printf("%s", response.body.c_str());
    }; 

    request.run();
  }

  {
    Options options = Request::DEFAULT_OPTIONS;
    options.method = typhoeus::DELETE;

    Request request("https://httpbin.org/delete", options);

    Response response = request.run();

    printf("Response from delete Request\n");
    printf("%s", response.body.c_str());
  }

  typhoeus::Curl::cleanup();

  return 0;
}
