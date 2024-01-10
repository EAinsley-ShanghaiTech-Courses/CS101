#include <iostream>

const int kMaxN = 2e3 + 5;
const int kMaxLogN = 12;
int log2_ceiling[kMaxN];

void PreProcess() {
  log2_ceiling[1] = 0;
  for (int i = 2; i < kMaxN; ++i) {
    log2_ceiling[i] = log2_ceiling[i / 2] + 1;
  }
}
int sparse_table[kMaxN][kMaxLogN];

int Gcd(int a, int b) {
  if (a == 1 || b == 1) return 1;
  int tmp = 0;
  if (a > b) {
    tmp = a;
    a = b;
    b = tmp;
  }
  while (a != 0) {
    tmp = a;
    a = b % a;
    b = tmp;
  }
  return b;
}
inline int Abs(int a) { return a > 0 ? a : -a; }

void BuildST(const int &length) {
  for (int j = 1; j <= log2_ceiling[length]; ++j) {
    for (int i = 0; i + (1 << j) - 1 < length; ++i) {
      sparse_table[i][j] =
          Gcd(sparse_table[i][j - 1], sparse_table[i + (1 << (j - 1))][j - 1]);
    }
  }
}

bool isHamonious(int &left, int &right) {
  int log_length = log2_ceiling[right - left + 1];
  return (Gcd(sparse_table[left][log_length],
              sparse_table[right - (1 << log_length) + 1][log_length]) != 1);
}

int FindHamoGroupLength() {
  int t;
  std::cin >> t;
  int first_element, second_element;
  std::cin >> first_element;
  if (t == 1) return 1;
  bool hasTwo = false;

  for (int i = 0; i < t - 1; ++i) {
    std::cin >> second_element;
    sparse_table[i][0] = Abs(second_element - first_element);
    if (sparse_table[i][0] != 1) hasTwo = true;
    first_element = second_element;
  }
  BuildST(t - 1);
  int left = 0, right = 1;
  while (right < t - 1) {
    if (!isHamonious(left, right)) ++left;

    ++right;
  }
  if (!hasTwo && right - left == 1) return 1;
  return right - left + 1;
};

int main() {
  std::ios::sync_with_stdio(false);
  PreProcess();
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::cout << FindHamoGroupLength() << std::endl;
  }
  return 0;
}