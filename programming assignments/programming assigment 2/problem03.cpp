#include <iostream>

const int kMergeSortN = 16;
struct Pair {
  int first, second;
  Pair(int f = 0, int s = 0) : first(f), second(s){};
};
void swap(int &a, int &b) {
  int t = a;
  a = b;
  b = t;
}

bool comp(const Pair &a, const Pair &b) {
  return a.first == b.first ? a.second < b.second : a.first < b.first;
}

void InsertionSort(Pair arr[], const int left, const int right) {
  int length = right - left + 1;
  for (int i = 1; i < length; ++i) {
    Pair tmp = arr[left + i];
    bool ismin = true;
    for (int j = i; j > 0; --j) {
      if (comp(tmp, arr[left + j - 1])) {
        arr[left + j] = arr[left + j - 1];
      } else {
        arr[left + j] = tmp;
        ismin = false;
        break;
      }
    }
    if (ismin) arr[left] = tmp;
  }
}

void Merge(Pair arr[], const int left, const int middle, const int right) {
  const int kLeftLength = middle - left + 1;
  const int kRightLength = right - middle;
  Pair *leftSubArray = new Pair[kLeftLength];
  Pair *rightSubArray = new Pair[kRightLength];
  for (int i = 0; i < kLeftLength; ++i) {
    leftSubArray[i] = arr[left + i];
  }
  for (int i = 0; i < kRightLength; ++i) {
    rightSubArray[i] = arr[middle + 1 + i];
  }
  int leftpointer = 0, rightpointer = 0, position = left;
  while (leftpointer < kLeftLength && rightpointer < kRightLength) {
    if (comp(leftSubArray[leftpointer], rightSubArray[rightpointer])) {
      arr[position++] = leftSubArray[leftpointer++];
    } else {
      arr[position++] = rightSubArray[rightpointer++];
    }
  }
  while (leftpointer < kLeftLength) {
    arr[position++] = leftSubArray[leftpointer++];
  }
  while (rightpointer < kRightLength) {
    arr[position++] = rightSubArray[rightpointer++];
  }
  delete[] leftSubArray;
  delete[] rightSubArray;
}

void MergeSort(Pair arr[], const int left, const int right) {
  if (left >= right) return;
  if (right - left < kMergeSortN)
    InsertionSort(arr, left, right);
  else {
    int middle = left + (right - left) / 2;
    MergeSort(arr, left, middle);
    MergeSort(arr, middle + 1, right);
    Merge(arr, left, middle, right);
  }
}

struct BinaryHeap {
  bool (*cmp)(const int &, const int &);
  int length;
  int *heap_table;
  BinaryHeap(Pair *h, int l, int start, bool (*c)(const int &, const int &))
      : length(l), cmp(c) {
    heap_table = new int[l + 1];
    for (int i = 0; i < l; ++i) {
      heap_table[i + 1] = h[start + i].second;
    }
    FloyedBuildHeap();
  }

  void FloyedBuildHeap() {
    for (int i = length / 2; i > 0; --i) {
      PercolateDown(i);
    }
  }
  void PercolateDown(int index) {
    int left = index * 2;
    int right = index * 2 + 1;
    while (right <= length && (cmp(heap_table[left], heap_table[index]) ||
                               cmp(heap_table[right], heap_table[index]))) {
      if (cmp(heap_table[left], heap_table[right])) {
        swap(heap_table[left], heap_table[index]);
        index = left;
      } else {
        swap(heap_table[right], heap_table[index]);
        index = right;
      }
      left = index * 2;
      right = index * 2 + 1;
    }
    if (left <= length && cmp(heap_table[left], heap_table[index])) {
      swap(heap_table[left], heap_table[index]);
    }
  }
  void PercolateUp(int index) {
    int parent = index / 2;
    while (index > 1 && cmp(heap_table[index], heap_table[parent])) {
      swap(heap_table[parent], heap_table[index]);
      index = parent;
      parent = index / 2;
    }
  }
  int Pop() {
    swap(heap_table[1], heap_table[length]);
    --length;
    PercolateDown(1);
    return heap_table[length + 1];
  }
  void Push(const int &value) {
    ++length;
    heap_table[length] = value;
    PercolateUp(length);
  }
  int Front() { return heap_table[1]; }
};

bool HeapComp(const int &a, const int &b) { return a > b; }

int main() {
  std::ios::sync_with_stdio(false);
  int n, c, f;
  std::cin >> n >> c >> f;
  Pair *all_data = new Pair[c];
  for (int i = 0; i < c; i++) {
    std::cin >> all_data[i].first >> all_data[i].second;
  }
  MergeSort(all_data, 0, c - 1);
  int half_size = n / 2;
  long long current_sum = 0;
  long long *prefix_sum = new long long[c], *suffix_sum = new long long[c];
  BinaryHeap left_heap = BinaryHeap(all_data, half_size, 0, HeapComp);
  BinaryHeap right_heap =
      BinaryHeap(all_data, half_size, c - half_size, HeapComp);

  for (int i = 0; i < half_size; ++i) {
    current_sum += all_data[i].second;
  }
  prefix_sum[half_size - 1] = current_sum;
  for (int i = half_size; i < c - half_size - 1; ++i) {
    int feed = all_data[i].second;
    if (feed < left_heap.Front()) {
      current_sum += feed;
      current_sum -= left_heap.Pop();
      left_heap.Push(feed);
    }
    prefix_sum[i] = current_sum;
  }
  current_sum = 0;
  for (int i = c - half_size; i < c; ++i) {
    current_sum += all_data[i].second;
  }
  suffix_sum[c - half_size] = current_sum;
  for (int i = c - half_size - 1; i > half_size; --i) {
    int feed = all_data[i].second;
    if (feed < right_heap.Front()) {
      current_sum += feed;
      current_sum -= right_heap.Pop();
      right_heap.Push(feed);
    }
    suffix_sum[i] = current_sum;
  }
  int ans = -1;
  for (int i = half_size; i < c - half_size; ++i) {
    if (prefix_sum[i - 1] + all_data[i].second + suffix_sum[i + 1] <= f)
      ans = all_data[i].first;
  }
  std::cout << ans << std::endl;
}
