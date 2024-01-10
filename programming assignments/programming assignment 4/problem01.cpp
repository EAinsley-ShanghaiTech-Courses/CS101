#include <iostream>

void Initialize(int n, int*& ptr) {
  ptr = new int[n + 1];
  for (int i = 1; i <= n; i++) std::cin >> ptr[i];
}

void Finalize(int*& ptr) {
  delete[] ptr;
  ptr = nullptr;
}

struct DisjointSet {
  int* table;
  int length;
  DisjointSet(int l) : length(l + 1) {
    table = new int[length];
    for (int i = 0; i < length; ++i) {
      table[i] = i;
    }
  }
  ~DisjointSet() {
    delete[] table;
    table = nullptr;
  }
  int Find(int i) {
    if (table[i] == i) {
      return i;
    } else {
      table[i] = Find(table[i]);
      return table[i];
    }
  }
  void Join(int a, int b) {
    int a_parent = Find(a - 1);
    int b_parent = Find(b - 1);
    table[b_parent] = a_parent;
  }
  int Parent(int i) { return Find(i - 1) + 1; }
};

struct ListNodeDatum {
  int index, count;
  ListNodeDatum(int i, int c) : index(i), count(c) {}
};

struct ListNode {
  ListNodeDatum datum;
  ListNode* next;
  ListNode(int i = 0, int c = 0, ListNode* n = nullptr)
      : datum(i, c), next(n) {}
};

struct Queue {
  ListNode *head, *tail;
  Queue() : head(nullptr), tail(nullptr) {}
  bool IsEmpty() { return head == nullptr; }
  void Push(int index, int count) {
    ListNode* newnode = new ListNode(index, count);
    if (tail != nullptr) {
      tail->next = newnode;
    }
    tail = newnode;
    if (head == nullptr) {
      head = newnode;
    }
  }
  ListNodeDatum Pop() {
    if (head == nullptr) std::cerr << "Underflow\n";
    ListNodeDatum d = head->datum;
    ListNode* dummy = head;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    delete dummy;
    dummy = nullptr;
    return d;
  }
  ~Queue() {
    ListNode* current = head;
    while (current != nullptr) {
      ListNode* next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
    tail = nullptr;
  }
};

int BFS(int* a, int* b, DisjointSet& disjoint_set, Queue& queue) {
  while (!queue.IsEmpty()) {
    ListNodeDatum current_level = queue.Pop();
    int current_index = current_level.index,
        current_count = current_level.count;
    if (current_index - a[current_index] <= 0) return current_count + 1;
    int top = disjoint_set.Parent(current_index - a[current_index]);
    int element_after_range = disjoint_set.Parent(current_index + 1);
    while (top < current_index) {
      queue.Push(top + b[top], current_count + 1);
      disjoint_set.Join(element_after_range, top);
      top = disjoint_set.Parent(top + 1);
    }
  }
  return -1;
}

int main() {
  std::ios::sync_with_stdio(false);
  int n;
  int *a, *b;
  std::cin >> n;
  if (n == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }
  Initialize(n, a);
  Initialize(n, b);
  DisjointSet disjoint_set(n);
  Queue queue;
  queue.Push(n, 0);
  disjoint_set.Join(n + 1, n);
  std::cout << BFS(a, b, disjoint_set, queue) << std::endl;
  Finalize(a);
  Finalize(b);
}