#include <iostream>

const int kInfinity = 1e3 + 5;
const int kEdgeWeightThreshold = 100;

inline void Swap(int*& pa, int*& pb) {
  int* pt = pa;
  pa = pb;
  pb = pt;
}
int** initilize_matrix(int n) {
  int** matrix = new int*[n];
  for (int i = 0; i < n; i++) {
    matrix[i] = new int[n];
    for (int j = 0; j < n; j++) std::cin >> matrix[i][j];
  }
  return matrix;
}

void finalize_matrix(int**& matrix, int n) {
  for (int i = 0; i < n; i++) {
    delete[] matrix[i];
    matrix[i] = nullptr;
  }
  delete[] matrix;
  matrix = nullptr;
}

void reset_visit(int n, bool*& visited) {
  for (int i = 0; i < n; i++) visited[i] = false;
}

void GreedyDFS(int current, int n, bool* visited, int** matrix, int* trial,
               int* path, int ans, int count, int& record_opt) {
  if (ans >= record_opt) return;
  trial[count] = current;
  if (count == n - 1) {
    ans += matrix[current][trial[0]];
    if (ans >= record_opt) return;
    record_opt = ans;
    for (int i = 0; i < n; i++) path[i] = trial[i];
    return;
  }

  visited[current] = true;

  int min_w = kEdgeWeightThreshold;
  for (int i = 0; i < n; i++) {
    if (visited[i]) continue;
    int w = matrix[current][i];
    if (w < min_w) min_w = matrix[current][i];
  }
  for (int i = 0, j = 0; i < n && j < 2; i++) {
    if (visited[i]) continue;
    int w = matrix[current][i];
    if (w <= min_w)
      j++, GreedyDFS(i, n, visited, matrix, trial, path, ans + w, count + 1,
                     record_opt);
  }

  visited[current] = false;
}

int GreedySearch(int start, int n, bool* visited, int** matrix, int* path) {
  visited[start] = true;
  int current_node = start;
  int ans = 0;
  path[0] = start;
  for (int count = 1; count < n; count++) {
    int min_weight = kInfinity;
    int min_node = -1;
    for (int i = 0; i < n; i++) {
      if (!visited[i] && matrix[current_node][i] < min_weight) {
        min_weight = matrix[current_node][i];
        min_node = i;
      }
    }
    ans += min_weight;
    current_node = min_node;
    path[count] = current_node;
    visited[current_node] = true;
  }
  return ans + matrix[current_node][start];
}

void MethodLarge(int n, int** adjacent_matrix, bool* visited, int*& ans_path,
                 int*& trial_path) {
  int ans = kInfinity;
  for (int i = 0; i < n; i++) {
    reset_visit(n, visited);
    int trial = GreedySearch(i, n, visited, adjacent_matrix, trial_path);
    if (trial < ans) {
      ans = trial;
      Swap(ans_path, trial_path);
    }
  }
}

void MethodSmall(int n, int** adjacent_matrix, bool* visited, int* ans_path,
                 int* trial_path) {
  reset_visit(n, visited);
  int recorded_optimal = 2e3;
  for (int i = 0; i < n; i++) {
    GreedyDFS(i, n, visited, adjacent_matrix, trial_path, ans_path, 0, 0,
              recorded_optimal);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  int** adjacent_matrix = initilize_matrix(n);
  bool* visited = new bool[n];
  int *ans_path = new int[n], *trial_path = new int[n];
  if (n > 50) {
    MethodLarge(n, adjacent_matrix, visited, ans_path, trial_path);
  } else {
    MethodSmall(n, adjacent_matrix, visited, ans_path, trial_path);
  }
  for (int i = 0; i < n; i++) {
    std::cout << ans_path[i] + 1 << " ";
  }
  finalize_matrix(adjacent_matrix, n);
  delete[] visited;
  delete[] ans_path;
  delete[] trial_path;
}