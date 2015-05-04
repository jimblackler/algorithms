#include <list>
#include "utils.h"

template <typename Signature, typename TestData, typename Output>
class Benchmark {
  struct Method {
    const char *name;
    Signature *signature;
  };

  std::list<Method> methods;

  virtual void generateTestData(unsigned int seed, TestData* testData) = 0;
  virtual void prepareOutput(TestData* testData, Output* output) = 0;
  virtual void invoke(Signature& signature, const TestData& data, Output* output) = 0;

public:
  void addMethod(char const *name, Signature *signature) {
    Method method {name, signature};
    methods.push_back(method);
  }

  void run() {
    TestData testData;
    generateTestData(0, &testData);
    for (Method& method : methods) {
      Output output;
      prepareOutput(&testData, &output);
      long long int before = getMicroseconds();
      invoke(*method.signature, testData, &output);
      long long int after = getMicroseconds();
    }
  }
};
