#include "typhoeus/typhoeus.h"
#include "typhoeus/factory.h"

int main(int argc, char* argv[]) {

  using typhoeus::Request;
  using typhoeus::Response;
  using typhoeus::Options;
  using typhoeus::Typhoeus;
  using typhoeus::Hydra;
  using typhoeus::Factory;

  typhoeus::Curl::init();

  Request q1("https://httpbin.org/get");
  q1.onComplete = [](const Response& r) {
    printf("Hydra get is working\n");
    printf("%s\n", r.body.c_str());
  };

  Options options = Request::DEFAULT_OPTIONS;
  options.method = typhoeus::DELETE;

  Request q2("https://httpbin.org/delete", options);
  q2.onComplete = [](const Response& r) { 
    printf("Hydra delete is working\n");
    printf("%s\n", r.body.c_str());
  };

  Request q3("https://httpbin.org/get");
  q3.onComplete = [](const Response& r) { 
    printf("Hydra get #2 is working\n");
    printf("%s\n", r.body.c_str());
  };

  Factory<200> factory;

  const Request* q4 = factory.acquire("https://httpbin.org/get", [](Options& options) { 
    printf("Configuring options from lambda\n");
    options.verbose = true;
  });

  Hydra hydra;

  hydra.queue(&q1);
  hydra.queue(&q2);
  hydra.queue(&q3);
  hydra.queue(q4);

  hydra.run();

  typhoeus::Curl::cleanup();

  return 0;
}
