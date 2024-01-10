#include <iostream>

struct ListNode {
  int height, count;
  ListNode* next;
  ListNode(int n) : height(n), count(1), next(nullptr) {}
};

int push(ListNode*& head, int height) {
  if (!head) {
    head = new ListNode(height);
    return 0;
  } else {
    if (height == head->height) {
      head->count++;
      return head->count;
    } else {
      ListNode* newnode = new ListNode(height);
      newnode->next = head;
      head = newnode;
      return 1;
    }
  }
}

int pop(ListNode*& head) {
  ListNode* nextnode = head->next;
  int count = head->count;
  delete head;
  head = nextnode;
  return count;
}

int main() {
  int count = 0, n = 0;
  ListNode* head = nullptr;
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    int height;
    std::cin >> height;
    while (head && height > head->height) {
      count += pop(head);
    }
    count += push(head, height);
  }
  std::cout << count << std::endl;
}