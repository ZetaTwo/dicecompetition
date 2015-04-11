#include <vector>
#include <algorithm>

namespace knapsack {
  typedef int weight_type;
  typedef int value_type;

  struct item {
    value_type value;
    weight_type weight;
    size_t item_index;
  };

  std::vector<size_t> knapsack(const weight_type capacity, const std::vector<item>& items) {
    std::vector<std::vector<value_type> > optimal(items.size() + 1, std::vector<int>(capacity + 1, 0));

    //Calculate dynprog table
    for (size_t n = 1; n <= items.size(); n++) {
      for (int w = 1; w <= capacity; w++) {

        optimal[n][w] = optimal[n - 1][w];

        if (items[n - 1].weight <= w) {
          int candidate = items[n - 1].value + optimal[n - 1][w - items[n - 1].weight];
          if (candidate > optimal[n][w]) {
            optimal[n][w] = candidate;
          }
        }
      }
    }

    //Backtrack which items used
    std::vector<size_t> result;
    for (int nn = items.size(), ww = capacity; nn > 0 && ww > 0; nn--)
    {
      if (ww - items[nn - 1].weight >= 0 && optimal[nn][ww] - items[nn - 1].value == optimal[nn-1][ww - items[nn - 1].weight]) {
        result.push_back(nn-1);
        ww -= items[nn-1].weight;
      }
    }
    std::reverse(result.begin(), result.end());

    return result;
  }
}