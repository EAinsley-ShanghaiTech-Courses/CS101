#include <iostream>

void swap(int& a, int& b) {
  int t = a;
  a = b;
  b = t;
}

struct ListNode {
  int index;
  ListNode* next;
  ListNode(int i = 0, ListNode* n = nullptr) : index(i), next(n) {}
};

struct SinglyLinkedList {
  ListNode* head;
  SinglyLinkedList() : head(nullptr) {}
  void Insert(int index) {
    ListNode* newnode = new ListNode(index, head);
    head = newnode;
  }
  ~SinglyLinkedList() {
    ListNode* current = head;
    while (current != nullptr) {
      ListNode* next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
  }
  void Print() {
    for (ListNode* it = head; it != nullptr; it = it->next) {
      std::cout << it->index << " ";
    }
    std::cout << std::endl;
  }
};

struct AdjacentList {
  SinglyLinkedList* list_table;
  int length;
  AdjacentList(int l) : length(l) { list_table = new SinglyLinkedList[length]; }
  ~AdjacentList() {
    delete[] list_table;
    list_table = nullptr;
  }
  void Connect(int a, int b) {
    list_table[a].Insert(b);
    list_table[b].Insert(a);
  }
  void Print() {
    for (int i = 0; i < length; i++) {
      std::cout << i << ": ";
      list_table[i].Print();
    }
  }
  SinglyLinkedList& operator[](int i) { return list_table[i]; }
};

void init(int n, int m, AdjacentList& adj_list, bool*& visit_ptr) {
  visit_ptr = new bool[n];
  for (int i = 0; i < n; i++) {
    visit_ptr[i] = false;
  }
  for (int i = 0; i < m; i++) {
    int a, b;
    std::cin >> a >> b;
    adj_list.Connect(a, b);
  }
}

void dfs(int current, int n, bool** table, bool* visited) {
  std::cout << current << " ";
  visited[current] = true;
  for (int i = 1; i < n; i++) {
    if (table[current][i] && !visited[i]) {
      dfs(i, n, table, visited);
    }
  }
}

struct BinaryHeap {
  bool (*cmp)(int, int);
  int length;
  int* heap_table;
  BinaryHeap(bool (*c)(int, int), int l) : cmp(c), length(0) {
    heap_table = new int[l];
    for (int i = 0; i < l; i++) heap_table[i] = 0;
  }

  ~BinaryHeap() {
    delete[] heap_table;
    heap_table = nullptr;
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
  void Push(const int& value) {
    ++length;
    heap_table[length] = value;
    PercolateUp(length);
  }
  int Front() { return heap_table[1]; }
};

bool HeapComp(int a, int b) { return a < b; }

int main() {
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  AdjacentList adjacent_list(n + 1);
  bool* visited = nullptr;
  init(n + 1, m, adjacent_list, visited);
  BinaryHeap min_heap(HeapComp, n);
  min_heap.Push(1);
  visited[1] = true;
  while (min_heap.length > 0) {
    int current = min_heap.Pop();
    std::cout << current << " ";
    for (ListNode* it = adjacent_list[current].head; it != nullptr;
         it = it->next) {
      if (!visited[it->index]) {
        visited[it->index] = true;
        min_heap.Push(it->index);
      }
    }
  }
  return 0;
}