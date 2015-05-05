#include <list>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "stdio.h"
#include "utils.h"

template<typename Signature, typename TestData, typename Output>
class Benchmark {
  struct Method {
    const char *name;
    Signature *signature;
  };

  struct Column {
    int x;
    std::map<Method *, int> results;
  };

  std::list<Method> methods;

  virtual int generateTestData(unsigned int seed, float t, TestData *testData) = 0;

  virtual void prepareOutput(TestData *testData, Output *output) = 0;

  virtual void invoke(Signature &signature, const TestData &data, Output *output) = 0;

public:
  void addMethod(char const *name, Signature *signature) {
    Method method{name, signature};
    methods.push_back(method);
  }

  void run(int samples, int cap) {
    TestData testData;
    std::vector<Column> columns;
    std::set<Method *> capped;
    for (int s = 0; s != samples; s++) {
      Column column;
      float t = (float) s / (samples - 1);
      column.x = generateTestData(0, t, &testData);

      int m = 0;
      for (Method &method : methods) {
        if (capped.find(&method) == capped.end() ) {
          Output output;
          prepareOutput(&testData, &output);
          long long int before = getMicroseconds();
          invoke(*method.signature, testData, &output);
          long long int after = getMicroseconds();
          int y = (int const &) (after - before);
          column.results[&method] = y;
          if (y > cap) {
            capped.insert(&method);
          }
        }
        m++;
      }

      columns.push_back(column);
    }

    FILE *gp = popen("/opt/local/bin/gnuplot", "w");
    int m = 0;
    fprintf(gp, "plot ");
    const char * separator = "";
    for (Method method : methods) {
      fprintf(gp, "%s'-' using 1:2 title '%s' with linespoints", separator, method.name);
      separator = ", ";
    }
    fprintf(gp, "\n");
    for (Method &method : methods) {
      for (Column &column: columns) {
        int x = column.x;
        if (column.results.find(&method) == column.results.end())
          continue;

        int y = column.results[&method];
        fprintf(gp, "%d %d\n", x, y);
      }
      fprintf(gp, "e\n");
      m++;
    }
    pclose(gp);

  }


};
