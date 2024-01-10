#include <iomanip>
#include <iostream>

const int kMergeSortN = 16;

inline int Abs(const int a) { return a > 0 ? a : -a; }
inline int Min(const int a, const int b) { return a < b ? a : b; }

struct ListNode {
  int index;
  int weight;
  ListNode* next;
  ListNode(int i = 0, int w = 0, ListNode* n = nullptr)
      : index(i), weight(w), next(n) {}
};

struct SinglyLinkedList {
  ListNode* head;
  SinglyLinkedList() : head(nullptr) {}
  void Insert(int index, int weight) {
    ListNode* newnode = new ListNode(index, weight, head);
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
};

struct AdjacentList {
  SinglyLinkedList* list_table;
  int length;
  AdjacentList(int l) : length(l) { list_table = new SinglyLinkedList[length]; }
  ~AdjacentList() {
    delete[] list_table;
    list_table = nullptr;
  }
  void Connect(int a, int b, int weight) {
    list_table[a].Insert(b, weight);
    list_table[b].Insert(a, weight);
  }
};

struct Edge {
  int from;
  int to;
  int weight;
};

bool comp(const Edge& a, const Edge& b) { return a.weight < b.weight; }

void InsertionSort(Edge arr[], const int left, const int right) {
  int length = right - left + 1;
  for (int i = 1; i < length; ++i) {
    Edge tmp = arr[left + i];
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

void Merge(Edge arr[], const int left, const int middle, const int right) {
  const int kLeftLength = middle - left + 1;
  const int kRightLength = right - middle;
  Edge* leftSubArray = new Edge[kLeftLength];
  Edge* rightSubArray = new Edge[kRightLength];
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

void MergeSort(Edge arr[], const int left, const int right) {
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

struct DisjointSet {
  int* parent;
  int length;
  DisjointSet(int l) : length(l) {
    parent = new int[length];
    for (int i = 0; i < length; ++i) {
      parent[i] = i;
    }
  }
  ~DisjointSet() {
    delete[] parent;
    parent = nullptr;
  }
  int Find(int i) {
    if (parent[i] == i) {
      return i;
    } else {
      parent[i] = Find(parent[i]);
      return parent[i];
    }
  }
  void Join(int a, int b) {
    int a_parent = Find(a);
    int b_parent = Find(b);
    if (a_parent == b_parent) {
    } else {
      parent[b_parent] = a_parent;
    }
  }
};

struct MinmalSpanningTree {
  int num_vertices;
  AdjacentList minimal_spanning_tree;
  DisjointSet disjoint_set;
  int minimal_weight;
  int delta_weight;
  int** max_edge_weight;
  MinmalSpanningTree(int num_v)
      : num_vertices(num_v),
        disjoint_set(num_vertices),
        minimal_spanning_tree(num_vertices),
        minimal_weight(0),
        delta_weight(1e5) {
    max_edge_weight = new int*[num_vertices];
    for (int i = 0; i < num_vertices; ++i) {
      max_edge_weight[i] = new int[num_vertices];
      for (int j = 0; j < num_vertices; ++j) {
        max_edge_weight[i][j] = -1;
      }
    }
  }
  ~MinmalSpanningTree() {
    for (int i = 0; i < num_vertices; ++i) {
      delete[] max_edge_weight[i];
      max_edge_weight[i] = nullptr;
    }
    delete[] max_edge_weight;
  }
  void KruskalBuildTree(Edge* edges, int num_edges) {
    for (int i = 0; i < num_edges; ++i) {
      int node_a = edges[i].from, node_b = edges[i].to;
      int weight = edges[i].weight;
      if (disjoint_set.Find(node_a) != disjoint_set.Find(node_b)) {
        minimal_spanning_tree.Connect(node_a, node_b, weight);
        disjoint_set.Join(node_a, node_b);
        UpdateMaxEdgeWeight(node_a, node_b, node_b, weight);
        UpdateMaxEdgeWeight(node_b, node_a, node_a, weight);
        minimal_weight += weight;
      } else {
        delta_weight =
            Min(delta_weight, Abs(weight - max_edge_weight[node_a][node_b]));
      }
    }
  }
  void UpdateMaxEdgeWeight(int current, int parent, int root, int weight) {
    max_edge_weight[root][current] = max_edge_weight[current][root] = weight;
    for (ListNode* child_node = minimal_spanning_tree.list_table[current].head;
         child_node != nullptr; child_node = child_node->next) {
      if (child_node->index == parent) continue;
      UpdateMaxEdgeWeight(child_node->index, current, root, weight);
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  int num_cities;
  int num_edges;
  std::cin >> num_cities >> num_edges;
  int* firness = new int[num_cities];
  for (int i = 0; i < num_cities; ++i) {
    std::cin >> firness[i];
    firness[i] = -firness[i];
  }
  Edge* edges = new Edge[num_edges];
  for (int i = 0; i < num_edges; ++i) {
    std::cin >> edges[i].from >> edges[i].to;
    --edges[i].from;
    --edges[i].to;
    edges[i].weight = firness[edges[i].from] + firness[edges[i].to];
  }
  MergeSort(edges, 0, num_edges - 1);
  MinmalSpanningTree mst(num_cities);
  mst.KruskalBuildTree(edges, num_edges);

  int ans = mst.minimal_weight * 2 + mst.delta_weight;
  std::cout << (-ans) / 2 << (ans % 2 == 0 ? ".0" : ".5") << std::endl;
  delete[] firness;
  firness = nullptr;
  return 0;
}