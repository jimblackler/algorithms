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
    int x;
    std::map<const Method *, int> results;
  };

  std::list<Method> methods;

public:
  void addMethod(char const *name, std::function<void(TestData const &, Output *)> invoker) {
    Method m;
    m.invoker = invoker;
    m.name = name;
    methods.push_back(m);
  }

  struct Comparator {
    bool operator()(const std::unique_ptr<const Output> &s1,
        const std::unique_ptr<const Output> &s2) const {
      return *s1 < *s2;
    }
  };

  void run(int samples, int cap) {

    std::vector<Column> columns;
    std::set<const Method *> capped;
    int previous = 0;
    for (int s = 0; s != samples; s++) {
      Column column;
      int target = (int) (pow((float) s / (samples - 1), 4) * 200000 + 1);
      while (target <= previous)
        target++;
      int round = 1;
      int size = target;

      while(true) {
        int prospect = target - target % round;
        if (prospect <= previous)
          break;
        round *= 5;
        prospect = target - target % round;
        if (prospect <= previous)
          break;
        round *= 2;
        size = prospect;
      }
      previous = size;
      TestData testData(0, size);
      column.x = size;

      std::multimap<std::unique_ptr<const Output>, const Method *, Comparator> results;
      int m = 0;
      for (const Method &method : methods) {
        if (capped.find(&method) == capped.end()) {
          Output *output = new Output(&testData);
          long long int before = getMicroseconds();
          method.invoker(testData, output);
          long long int after = getMicroseconds();
          int y = (int const &) (after - before);
          results.insert(std::make_pair(std::unique_ptr<const Output>(output), &method));
          column.results[&method] = y;
          if (y > cap) {
            capped.insert(&method);
          }
        }
        m++;
      }

      int groupCount = 0;
      for (auto it = results.begin(); it != results.end(); it = results.upper_bound(it->first)) {
        groupCount++;
      }

      if (groupCount > 1) {
        printf("%d: Multiple groups of results where one was expected:\n", column.x);
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
    fprintf(gp, "set xrange [0:%d]\n", columns.back().x);
    fprintf(gp, "set yrange [0:%d]\n", cap);
    fprintf(gp, "set ylabel \"Microseconds\"\n");

    fprintf(gp, "plot ");
    const char *separator = "";
    for (const Method &method : methods) {
      fprintf(gp, "%s'-' using 1:2 title '%s' with linespoints pointsize 0.1", separator, method.name);
      separator = ", ";
    }
    fprintf(gp, "\n");
    for (const Method &method : methods) {
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

    /* Output HTML table */
    FILE *h = fopen("out/mytable.html", "w");

    fprintf(h, "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"></head>");
    fprintf(h, "<table><tr><td></td>");
    for (Column &column: columns) {
      fprintf(h, "<td>%d</td>", column.x);
    }
    fprintf(h, "</tr>");
    for (const Method &method : methods) {
      fprintf(h, "<tr><td>%s</td>", method.name);

      for (Column &column: columns) {

        fprintf(h, "<td>");
        if (column.results.find(&method) != column.results.end()) {
          int y = column.results[&method];
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
