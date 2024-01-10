#include <iostream>

struct DisjointSetNode {
  int parent;
  int count;
};

struct DisjointSet {
  DisjointSetNode *table;
  int length;
  DisjointSet(int l) : length(l) {
    table = new DisjointSetNode[length];
    for (int i = 0; i < length; ++i) {
      table[i].parent = i;
      table[i].count = -1;
    }
  }
  ~DisjointSet() {
    delete[] table;
    table = nullptr;
  }
  int Find(int i) {
    if (table[i].parent == i) {
      return i;
    } else {
      table[i].parent = Find(table[i].parent);
      return table[i].parent;
    }
  }
  void Join(int a, int b) {
    int a_parent = Find(a - 1);
    int b_parent = Find(b - 1);
    if (a_parent == b_parent) {
      ++table[a_parent].count;
    } else {
      table[b_parent].parent = a_parent;
      table[a_parent].count += table[b_parent].count + 1;
    }
  }
  int Traverse() {
    int ans = 0;
    for (int i = 0; i < length; ++i) {
      if (Find(i) == i && table[i].count > 0) {
        ans += table[i].count;
      }
    }
    return ans;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  int num_cities;
  int num_dependencies;
  std::cin >> num_cities >> num_dependencies;
  DisjointSet disjoint_set(num_cities);
  for (int i = 0; i < num_dependencies; ++i) {
    int a, b;
    std::cin >> a >> b;
    disjoint_set.Join(a, b);
  }
  std::cout << disjoint_set.Traverse() << std::endl;
  return 0;
}