#include <iostream>
#include <tuple>

inline int Max(int a, int b) { return a > b ? a : b; }

struct AVLTreeNode {
  int height;
  int value;
  int size;
  int count;
  AVLTreeNode* left_child;
  AVLTreeNode* right_child;
  AVLTreeNode(int v)
      : height(0),
        value(v),
        size(1),
        count(1),
        left_child(nullptr),
        right_child(nullptr) {}
  AVLTreeNode* max() {
    AVLTreeNode* current_node = this;
    while (current_node->right_child != nullptr) {
      current_node = current_node->right_child;
    }
    return current_node;
  }
  AVLTreeNode* min() {
    AVLTreeNode* current_node = this;
    while (current_node->left_child != nullptr) {
      current_node = current_node->left_child;
    }
    return current_node;
  }
  inline int LeftSize() { return left_child ? left_child->size : 0; }
  inline int RightSize() { return right_child ? right_child->size : 0; }
  inline int LeftHeight() { return left_child ? left_child->height : -1; }
  inline int RightHeight() { return right_child ? right_child->height : -1; }
  int GetBalanceFactor() { return LeftHeight() - RightHeight(); }
};

struct AVL {
  AVLTreeNode* root;
  AVL() : root(nullptr) {}
  void insert(int v) { root = InsertNode(v, root); }
  void remove(int v) { root = DeleteNode(v, root); }
  int kth(int k) {
    if (k >= root->size) return root->max()->value;
    AVLTreeNode* current_node = root;
    while (k > 0) {
      if (k >= current_node->LeftSize() + current_node->count) {
        k -= current_node->LeftSize() + current_node->count;
        current_node = current_node->right_child;
      } else if (k >= current_node->LeftSize()) {
        return current_node->value;
      } else {
        current_node = current_node->left_child;
      }
    }
    return current_node->min()->value;
  };
  int rank(int v) {
    AVLTreeNode* current_node = root;
    int rank = 0;
    while (current_node != nullptr) {
      if (v == current_node->value)
        return rank + current_node->LeftSize();
      else if (v > current_node->value) {
        rank += current_node->LeftSize() + current_node->count;
        current_node = current_node->right_child;
      } else {
        current_node = current_node->left_child;
      }
    }
    return rank;
  };
  int size() { return root->size; }

 private:
  AVLTreeNode* RotateLeft(AVLTreeNode* node) {
    AVLTreeNode* right_node = node->right_child;
    AVLTreeNode* right_left_node = right_node->left_child;
    right_node->left_child = node;
    node->right_child = right_left_node;
    node->height = Max(node->LeftHeight(), node->RightHeight()) + 1;
    right_node->height =
        Max(right_node->LeftHeight(), right_node->RightHeight()) + 1;
    node->size = node->count + node->LeftSize() + node->RightSize();
    right_node->size =
        right_node->count + right_node->LeftSize() + right_node->RightSize();
    return right_node;
  };
  AVLTreeNode* RotateRight(AVLTreeNode* node) {
    AVLTreeNode* left_node = node->left_child;
    AVLTreeNode* left_right_node = left_node->right_child;
    left_node->right_child = node;
    node->left_child = left_right_node;
    node->height = Max(node->LeftHeight(), node->RightHeight()) + 1;
    left_node->height =
        Max(left_node->LeftHeight(), left_node->RightHeight()) + 1;
    node->size = node->count + node->LeftSize() + node->RightSize();
    left_node->size =
        left_node->count + left_node->LeftSize() + left_node->RightSize();
    return left_node;
  }
  AVLTreeNode* InsertNode(int v, AVLTreeNode* node) {
    if (node == nullptr) return new AVLTreeNode(v);
    if (v < node->value) {
      node->left_child = InsertNode(v, node->left_child);
    } else if (v > node->value) {
      node->right_child = InsertNode(v, node->right_child);
    } else {
      node->count++;
    }
    // update
    node->size = node->count + node->LeftSize() + node->RightSize();
    node->height = 1 + Max(node->LeftHeight(), node->RightHeight());
    int balance_factor = node->GetBalanceFactor();
    // rotation
    if (balance_factor > 1) {
      // left-*
      if (v >= node->left_child->value) {
        // left-right
        node->left_child = RotateLeft(node->left_child);
      }
      return RotateRight(node);
    } else if (balance_factor < -1) {
      // right-*
      if (v < node->right_child->value) {
        // right-left
        node->right_child = RotateRight(node->right_child);
      }
      return RotateLeft(node);
    }
    return node;
  }
  AVLTreeNode* DeleteNode(int v, AVLTreeNode* node) {
    if (node == nullptr) return node;
    if (v < node->value) {
      node->left_child = DeleteNode(v, node->left_child);
    } else if (v > node->value) {
      node->right_child = DeleteNode(v, node->right_child);
    } else {
      // find the node to be delete
      if (node->count > 1) {
        node->count--;
      } else if (node->left_child == nullptr && node->right_child == nullptr) {
        // have no child
        delete node;
        node = nullptr;
        return node;
      } else if (node->left_child == nullptr || node->right_child == nullptr) {
        // with one child
        AVLTreeNode* temp =
            node->left_child ? node->left_child : node->right_child;
        delete node;
        node = temp;
        return node;
      } else {
        // two children
        AVLTreeNode* successor = node->right_child->min();
        node->value = successor->value;
        node->count = successor->count;
        successor->count = 1;
        node->right_child = DeleteNode(successor->value, node->right_child);
      }
    }
    // update
    node->height = 1 + Max(node->LeftHeight(), node->RightHeight());
    node->size = node->count + node->LeftSize() + node->RightSize();
    int balance_factor = node->GetBalanceFactor();
    // rotation
    if (balance_factor > 1) {
      // left-*
      if (node->left_child->GetBalanceFactor() < 0) {
        // left-right
        node->left_child = RotateLeft(node->left_child);
      }
      return RotateRight(node);
    } else if (balance_factor < -1) {
      // right-*
      if (node->right_child->GetBalanceFactor() > 0) {
        node->right_child = RotateRight(node->right_child);
      }
      return RotateLeft(node);
    }
    return node;
  }
};

/* PLEASE DO NOT CHANGE BELOW*/
int A, B, C, lfsr;
double P[4][4];
int lfsr_generator() {
  auto ret = lfsr;
  return (lfsr ^= lfsr << 13, lfsr ^= lfsr >> 17, lfsr ^= lfsr << 5, ret);
}
std::tuple<int, int> command() {
  auto imm = lfsr_generator();
  static int state = 0;
  auto p = double(lfsr_generator() & 0x7fffffff) / INT32_MAX;
  for (int i = 0; i < 4; i++)
    if ((p -= P[state % 4][i]) < 0) {
      state += 4 - state % 4 + i;
      break;
    }
  return std::tuple<int, int>(state % 4,
                              (imm * A + state * B + C) & 0x7fffffff);
}
/* PLEASE DO NOT CHANGE ABOVE*/
int main() {
  AVL tree;
  int m;
  std::cin >> m >> lfsr >> A >> B >> C;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) std::cin >> P[i][j];
  int tans = 0;
  for (int i = 1; i <= m; i++) {
    int op, imm;
    std::tie(op, imm) = command();
    if (op == 0) tree.insert(imm);
    if (op == 1) tree.remove(tree.kth(imm % tree.size()));
    if (op == 2) tans ^= tree.rank(imm);
    if (op == 3) tans ^= tree.kth(imm % tree.size());
  }
  std::cout << tans << "\n";
}
