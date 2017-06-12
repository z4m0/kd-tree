#include <vector>

template <class T>
class KDTree{
public:
  KDTree(int nd = 2);
  ~KDTree();
  void insert(const std::vector<T>& element);
  void del(const std::vector<T>& element);
  bool exists(const std::vector<T>& element);
  std::vector<std::vector<T> > orthogonal_search(const std::vector<T>& mins, const std::vector<T>& maxs);
  std::vector<T> nearest_neighbor(const std::vector<T>& e);
  std::vector<std::vector<T> > partial_match(const std::vector<T>& dims, const std::vector<T>& values);


private:
  struct Node {
    int disc_dim;
    std::vector<T> val;
    Node * left;
    Node * right;
  };

  Node* insert(Node *n, const std::vector<T>& e);
  Node* del(Node *n, const std::vector<T>& e);
  Node* find_min(Node *n);
  bool exists(Node *n, const std::vector<T>& e);
  std::vector<std::vector<T> > orthogonal_search(Node *n, const std::vector<T>& mins, const std::vector<T>& maxs);
  void nearest_neighbor(Node*n, const std::vector<T>& e, std::pair<std::vector<T>,T>& best);
  void destroy_node(Node *n);

  int num_dims;
  Node * root = NULL;
};
