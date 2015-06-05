// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "stdio.h"
#include "stdlib.h"
#include "timer.h"
#include "utils.h"

#include <algorithm>
#include <future>
#include <list>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <sys/time.h>
#include <thread>
#include <vector>

template<typename TestData, typename Output>
class Benchmark {

  struct Method {
    std::function<void(TestData const &, Output *)> invoker;
    std::string name;
  };

  struct Column {
    float x;
    std::map<const Method *, long long> results;
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

  // Returns an error string if the output is definitely invalid for the supplied test data.
  // Returns a blank string if the output is not yet known to be invalid.
  virtual std::string isValid(const TestData &testData, const Output &output) const {
    // Override in individual benchmarks where relevant.
    return std::string();
  }

  virtual long long yAxisValue(long long microseconds, const Output &output) {
    return microseconds;
  }

  void run(int samples, float min, float max, float distribution, bool rounded, long cap,
      const char *xlabel) {
    std::vector<Column> columns;
    std::set<const Method *> capped;
    std::unique_ptr<Timer> timer(Timer::newTimer());
    int previous = 0;
    for (int s = 0; s != samples; s++) {

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
      TestData testData(1, value);
      printf("\n%d / %d: %g\n", s + 1, samples, value);
      column.x = value;

      std::multimap<std::unique_ptr<const Output>, const Method *, Comparator> results;
      std::vector<std::shared_ptr<const Method>> shuffled_methods(methods);
      std::random_shuffle(shuffled_methods.begin(), shuffled_methods.end());
      for (auto method : shuffled_methods) {
        if (capped.find(method.get()) != capped.end())
          continue;

        Output *output = new Output(&testData);

        auto invoker = method->invoker;

        auto before = timer->getTime();
        invoker(testData, output);
        auto after = timer->getTime();

        auto y = yAxisValue(after - before, *output);
        std::string error = isValid(testData, *output);
        if (error.empty()) {
          results.insert(std::make_pair(std::unique_ptr<const Output>(output), method.get()));
          column.results[method.get()] = y;
          if (y > cap) {
            capped.insert(method.get());
            printf("%s capped at %qi (> %li)\n", method->name.c_str(), y, cap);
          }
        } else {
          printf("Method %s produced invalid output: %s\n", method->name.c_str(), error.c_str());
          capped.insert(method.get());
        }
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
            printf("%s%s", separator, it2->second->name.c_str());
            separator = ", ";
          }
          printf("\n");
        }
      } else {
        for (auto result : column.results) {
          auto method = result.first;
          printf("%s:\t%qi\n", method->name.c_str(), result.second);
        }
        columns.push_back(column);
      }
    }

    printf("Done.\n");

    /* Output Gnuplot graph */
    bool svgMode = false;

    FILE *gp;
    if (svgMode) {
      gp = popen("/opt/local/bin/gnuplot >out/chart.svg", "w");
      fprintf(gp, "set term svg size 1200,800 mouse jsdir \"http://gnuplot.sourceforge.net/demo_svg_4.6/\"\n");
    } else {
      gp = popen("/opt/local/bin/gnuplot", "w");
      fprintf(gp, "set term aqua size 1200,800\n");
    }
    int m = 0;

    fprintf(gp, "set xrange [%f:%f]\n", min, columns.back().x);
    fprintf(gp, "set yrange [0:%li]\n", cap);
    fprintf(gp, "set xlabel \"%s\"\n", xlabel);

    fprintf(gp, "plot ");
    const char *separator = "";
    for (auto method : methods) {
      fprintf(gp, "%s'-' using 1:2 title '%s' with linespoints pointsize 0.1", separator,
              method->name.c_str());
      separator = ", ";
    }
    fprintf(gp, "\n");
    for (auto method : methods) {
      for (Column &column: columns) {
        float x = column.x;
        if (column.results.find(method.get()) == column.results.end())
          continue;

        long long y = column.results[method.get()];
        fprintf(gp, "%f %qi\n", x, y);
      }
      fprintf(gp, "e\n");
      m++;
    }
    pclose(gp);

    /* Output HTML table */
    FILE *h = fopen("out/mytable.html", "w");
    if (h) {
      fprintf(h, "<head><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"></head>");
      fprintf(h, "<table><tr><td></td>");
      for (Column &column: columns) {
        fprintf(h, "<td>%g</td>", column.x);
      }
      fprintf(h, "<td>Total</td><td></td></tr>");
      for (auto method : methods) {
        std::string name;
        encodeHtml(method->name, name);
        fprintf(h, "<tr title=\"%s\"><td>%s</td>", name.c_str(), name.c_str());
        bool all = true;
        long long total = 0;
        for (Column &column: columns) {

          // Find winner
          long long winner = std::numeric_limits<long long>::max();
          for (auto pair: column.results) {
            winner = std::min(winner, pair.second);
          }

          if (column.results.find(method.get()) != column.results.end()) {
            long long y = column.results[method.get()];
            total += y;
            if (y == winner) {
              fprintf(h, "<td class='winner'>");
            } else {
              fprintf(h, "<td>");
            }

            fprintf(h, "%qi</td>", y);

          } else {
            all = false;
            fprintf(h, "<td></td>");
          }
        }
        if (all) {
          fprintf(h, "<td>%qi</td>", total);
        } else {
          fprintf(h, "<td></td>");
        }
        fprintf(h, "<td>%s</td>", name.c_str());
        fprintf(h, "</tr>");
        m++;
      }
      fprintf(h, "</table>");
      fclose(h);
    }
  }
};
