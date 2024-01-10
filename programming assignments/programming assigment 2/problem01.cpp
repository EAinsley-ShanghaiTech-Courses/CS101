#include <iostream>

const int kStateNumber = 2;
struct ListNode {
  int value;
  ListNode* next;
  ListNode(int v = 0, ListNode* n = nullptr) : value(v), next(n) {}
};

struct SinglyLinkedList {
  ListNode* head;
  SinglyLinkedList() : head(nullptr) {}
  void Insert(int value) {
    ListNode* newnode = new ListNode(value, head);
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

int state_count[kStateNumber] = {0, 0};
int ans = 0;

void SearchEdge(int index, int parent, SinglyLinkedList*& tree,
                int*& node_state, int& out_state1, int& out_state2) {
  int s1 = node_state[index] == 1 ? 1 : 0;
  int s2 = node_state[index] == 2 ? 1 : 0;
  for (ListNode* current_node = tree[index].head; current_node != nullptr;
       current_node = current_node->next) {
    if (current_node->value == parent) continue;
    SearchEdge(current_node->value, index, tree, node_state, s1, s2);
  }
  if ((s1 == 0 || s2 == 0) &&
      (state_count[0] - s1 == 0 || state_count[1] - s2 == 0)) {
    ++ans;
  }
  out_state1 += s1;
  out_state2 += s2;
}

int main() {
  int n;
  std::cin >> n;
  int* node_state = new int[n];
  for (int i = 0; i < n; ++i) {
    int state = 0;
    std::cin >> state;
    if (state != 0) state_count[state - 1]++;
    node_state[i] = state;
  }
  SinglyLinkedList* adjacency_list = new SinglyLinkedList[n];
  for (int i = 0; i < n - 1; ++i) {
    int x, y;
    std::cin >> x >> y;
    adjacency_list[x - 1].Insert(y - 1);
    adjacency_list[y - 1].Insert(x - 1);
  }
  int state1 = 0, state2 = 0;
  SearchEdge(0, -1, adjacency_list, node_state, state1, state2);
  std::cout << ans << std::endl;
  delete[] adjacency_list;
  adjacency_list = nullptr;
  delete[] node_state;
  node_state = nullptr;
}
