#include <iostream>

using namespace std;
const int kMergeSortN = 16;
struct Vector2 {
  int x;
  int y;
  int SquareDistance(Vector2 other) {
    return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
  }
};

struct Receiver {
  int squaredistance1, squaredistance2;
};

inline int max(const int& left, const int& right) {
  return left > right ? left : right;
}

inline int min(const int& left, const int& right) {
  return left < right ? left : right;
}

bool comp(const Receiver& first, const Receiver& second) {
  return first.squaredistance1 < second.squaredistance1;
}

void InsertionSort(Receiver arr[], const int left, const int right) {
  int length = right - left + 1;
  for (int i = 1; i < length; ++i) {
    Receiver tmp = arr[left + i];
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

void Merge(Receiver arr[], const int left, const int middle, const int right) {
  const int kLeftLength = middle - left + 1;
  const int kRightLength = right - middle;
  Receiver* leftSubArray = new Receiver[kLeftLength];
  Receiver* rightSubArray = new Receiver[kRightLength];
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

void MergeSort(Receiver arr[], const int left, const int right) {
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

int main() {
  Vector2 tower1, tower2;
  std::cin >> tower1.x >> tower1.y >> tower2.x >> tower2.y;
  int n = 0;
  std::cin >> n;
  Receiver* receivers_arr = new Receiver[n];
  for (int i = 0; i < n; ++i) {
    Vector2 position;
    std::cin >> position.x >> position.y;
    receivers_arr[i].squaredistance1 = tower1.SquareDistance(position);
    receivers_arr[i].squaredistance2 = tower2.SquareDistance(position);
  }
  MergeSort(receivers_arr, 0, n - 1);
  int results = receivers_arr[n - 1].squaredistance1;
  int max_square_radius_tower2 = 0;
  for (int i = n - 1; i > 0; --i) {
    max_square_radius_tower2 =
        max(max_square_radius_tower2, receivers_arr[i].squaredistance2);
    results = min(results, max_square_radius_tower2 +
                               receivers_arr[i - 1].squaredistance1);
  }
  results = min(
      results, max(max_square_radius_tower2, receivers_arr[0].squaredistance2));
  std::cout << results << std::endl;

  delete[] receivers_arr;
  return 0;
}