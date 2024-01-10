#include <iostream>
#include <string>

const int kAlphabetSize = 26;

void reverse(std::string& str) {
  char ch;
  int length = str.length();
  for (int i = 0; i < length / 2; i++) {
    ch = str[i];
    str[i] = str[length - i - 1];
    str[length - i - 1] = ch;
  }
}

struct TreeNode {
  TreeNode* children[kAlphabetSize];
  int count;
  TreeNode() : count(0) {
    for (int i = 0; i < kAlphabetSize; ++i) {
      children[i] = nullptr;
    }
  }
};

struct PrefixTree {
  TreeNode* root;
  TreeNode* prefixNode;
  std::string longest_preffix;
  bool is_reverse;
  PrefixTree(bool rev = false)
      : is_reverse(rev), root(new TreeNode), prefixNode(new TreeNode) {}
  void UpdateLongest(std::string str, TreeNode* current_node) {
    if (is_reverse) reverse(str);
    if (str == longest_preffix) return;
    bool canupdate = false;
    if (str.length() > longest_preffix.length()) {
      canupdate = true;
    } else if (current_node->count > prefixNode->count) {
      canupdate = true;
    } else if (current_node->count == prefixNode->count &&
               str < longest_preffix) {
      canupdate = true;
    }
    if (canupdate) {
      longest_preffix = str;
      prefixNode = current_node;
    }
  }
  void Insert(std::string str) {
    TreeNode* current_node = root;
    bool is_common_prefix = true;
    if (is_reverse) reverse(str);
    for (int i = 0; i < str.length(); i++) {
      int index = str[i] - 'a';
      if (is_common_prefix && i >= longest_preffix.length()) {
        UpdateLongest(str.substr(0, i), current_node);
      }
      if (current_node->children[index] == nullptr) {
        current_node->children[index] = new TreeNode;
        if (is_common_prefix) is_common_prefix = false;
      }
      current_node = current_node->children[index];
      current_node->count++;
    }
    if (is_common_prefix && str.length() >= longest_preffix.length())
      UpdateLongest(str, current_node);
  }
  void GetLongestPreffix(std::string& results, int& length) {
    results = longest_preffix;
    length = prefixNode->count;
  }
};

int main() {
  int n;
  PrefixTree prefix_tree, suffix_tree(true);
  std::cin >> n;
  for (int i = 0; i < n; i++) {
    std::string str;
    std::cin >> str;
    prefix_tree.Insert(str);
    suffix_tree.Insert(str);
  }
  std::string results;
  int length = 0;
  prefix_tree.GetLongestPreffix(results, length);
  std::cout << results << " " << length;
  std::cout << std::endl;
  suffix_tree.GetLongestPreffix(results, length);
  std::cout << results << " " << length;
  return 0;
}