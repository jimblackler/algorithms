// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <math.h>

#include "stdio.h"
#include "stdlib.h"
#include "utils.h"

template<typename TestData, typename Output>
class Benchmark {

  struct Method {
    std::function<void(TestData const &, Output *)> invoker;
    const char *name;
  };

  struct Column {
    float x;
    std::map<const Method *, int> results;
  };

  std::vector<std::shared_ptr<const Method>> methods;

public:
  void addMethod(char const *name, std::function<void(TestData const &, Output *)> invoker) {
    Method *m = new Method();
    m->invoker = invoker;
    m->name = name;
    methods.push_back(std::shared_ptr<const Method>(m));
  }

  struct Comparator {
    bool operator()(const std::unique_ptr<const Output> &s1,
        const std::unique_ptr<const Output> &s2) const {
      return *s1 < *s2;
    }
  };

  void run(int samples, float min, float max, float distribution, bool rounded, int cap, const char *label) {
    std::vector<Column> columns;
    std::set<const Method *> capped;
    int previous = 0;
    for (int s = 0; s != samples; s++) {
      printf("%d / %d\r", s, samples);
      fflush(stdout);
      Column column;
      float value = (float) (pow((float) s / (samples - 1), distribution) * (max - min) + min);
      if (rounded) {
        int target = (int) value;

        while (target <= previous)
          target++;
        int round = 1;
        value = target;

        while (true) {
          int prospect = target - target % round;
          if (prospect <= previous)
            break;
          round *= 5;
          prospect = target - target % round;
          if (prospect <= previous)
            break;
          round *= 2;
          value = prospect;
        }
        previous = (int) value;
      }
      TestData testData(0, value);
      column.x = value;

      std::multimap<std::unique_ptr<const Output>, const Method *, Comparator> results;
      int m = 0;
      std::vector<std::shared_ptr<const Method>> shuffled_methods(methods);
      std::random_shuffle(shuffled_methods.begin(), shuffled_methods.end());
      for (auto method : shuffled_methods) {
        if (capped.find(method.get()) == capped.end()) {
          Output *output = new Output(&testData);
          long long int before = getMicroseconds();
          method->invoker(testData, output);
          long long int after = getMicroseconds();
          int y = (int const &) (after - before);
          results.insert(std::make_pair(std::unique_ptr<const Output>(output), method.get()));
          column.results[method.get()] = y;
          if (y > cap) {
            capped.insert(method.get());
          }
        }
        m++;
      }

      int groupCount = 0;
      for (auto it = results.begin(); it != results.end(); it = results.upper_bound(it->first)) {
        groupCount++;
      }

      if (groupCount > 1) {
        printf("%f: Multiple groups of results where one was expected:\n", column.x);
        for (auto it = results.begin(); it != results.end(); it = results.upper_bound(it->first)) {
          auto groupEnd = results.upper_bound(it->first);
          const char *separator = "Group: ";
          for (auto it2 = it; it2 != groupEnd; it2++) {
            printf("%s%s", separator, it2->second->name);
            separator = ", ";
          }
          printf("\n");
        }
      } else {
        columns.push_back(column);
      }
    }

    /* Output Gnuplot graph */
    FILE *gp = popen("/opt/local/bin/gnuplot", "w");
    int m = 0;
    fprintf(gp, "set xrange [0:%f]\n", columns.back().x);
    fprintf(gp, "set yrange [0:%d]\n", cap);
    fprintf(gp, "set ylabel \"%s\"\n", label);

    fprintf(gp, "plot ");
    const char *separator = "";
    for (auto method : methods) {
      fprintf(gp, "%s'-' using 1:2 title '%s' with linespoints pointsize 0.1", separator, method->name);
      separator = ", ";
    }
    fprintf(gp, "\n");
    for (auto method : methods) {
      for (Column &column: columns) {
        float x = column.x;
        if (column.results.find(method.get()) == column.results.end())
          continue;

        int y = column.results[method.get()];
        fprintf(gp, "%f %d\n", x, y);
      }
      fprintf(gp, "e\n");
      m++;
    }
    pclose(gp);

    /* Output HTML table */
    FILE *h = fopen("out/mytable.html", "w");

    fprintf(h, "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"></head>");
    fprintf(h, "<table><tr><td></td>");
    for (Column &column: columns) {
      fprintf(h, "<td>%f</td>", column.x);
    }
    fprintf(h, "</tr>");
    for (auto method : methods) {
      fprintf(h, "<tr><td>%s</td>", method->name);

      for (Column &column: columns) {

        fprintf(h, "<td>");
        if (column.results.find(method.get()) != column.results.end()) {
          int y = column.results[method.get()];
          fprintf(h, "%d", y);
        }
        fprintf(h, "</td>");
      }
      fprintf(h, "</tr>");
      m++;
    }
    fprintf(h, "</table>");
    fclose(h);

  }


};
