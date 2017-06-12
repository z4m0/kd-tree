#include "kd_tree.hpp"
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <limits>

template <class T>
KDTree<T>::KDTree(int nd){
  this->num_dims = nd;
  srand (time(NULL));
}

template <class T>
KDTree<T>::~KDTree(){
  destroy_node(root);
}

template <class T>
void KDTree<T>::destroy_node(Node *n){
  if(n == NULL){
    return;
  }
  destroy_node(n->left);
  destroy_node(n->right);
  delete n;
}

template <class T>
void KDTree<T>::insert(const std::vector<T>& element){
  root = insert(root, element);
}

template <class T>
void KDTree<T>::del(const std::vector<T>& element){
  del(root, element);
}

template <class T>
typename KDTree<T>::Node* KDTree<T>::insert(Node *n, const std::vector<T>& e){
  if(n == NULL){
    n = new Node();
    n->disc_dim = rand() % num_dims;
    n->val = e;
  }else if(n->val[n->disc_dim] <= e[n->disc_dim]){
    n->left = insert(n->left, e);
  }else{
    n->right = insert(n->right, e);
  }
  return n;
}

template <class T>
typename KDTree<T>::Node* KDTree<T>::del(Node *n, const std::vector<T>& e){
  if(n == NULL){
    return n;
  }else if(n->val[n->disc_dim] < e[n->disc_dim]){
    n->left = del(n->left, e);
  }else if(n->val[n->disc_dim] > e[n->disc_dim]){
    n->right = del(n->right, e);
    return n;
  }
  for(int i=0; i<num_dims; ++i){
    if(n->val[i] != e[i]){
      n->left = del(n->left, e);
      return n;
    }
  }
  Node* left = n->left;
  Node* right = n->right;
  delete n;
  if(left == NULL && right == NULL){
    return NULL;
  }
  if(left != NULL && right == NULL){
    return left;
  }
  if(left == NULL && right != NULL){
    return right;
  }
  Node * new_v = find_min(right);
  new_v->right = del(right, new_v);
  new_v->left = left;
  return new_v;
}

template <class T>
typename KDTree<T>::Node* KDTree<T>::find_min(Node *n){
  if(n->left == NULL){
    return n;
  }
  return find_min(n->left);
}

template <class T>
bool KDTree<T>::exists(const std::vector<T>& element){
  return exists(root, element);
}

template <class T>
bool KDTree<T>::exists(Node* n, const std::vector<T>& e){
  if(n == NULL){
    return false;
  }
  if(n->val[n->disc_dim] < e[n->disc_dim]){
    return exists(n->left, e);
  }else if(n->val[n->disc_dim] > e[n->disc_dim]){
    return exists(n->right, e);
  }
  for(int i=0; i<num_dims; ++i){
    if(n->val[i] != e[i]){
      return exists(n->left, e);
    }
  }
  return true;
}

template <class T>
std::vector<std::vector<T> > KDTree<T>::orthogonal_search(const std::vector<T>& mins, const std::vector<T>& maxs){
  return orthogonal_search(root, mins, maxs);
}

template <class T>
std::vector<std::vector<T> > KDTree<T>::orthogonal_search(Node *n, const std::vector<T>& mins, const std::vector<T>& maxs){
  if(n == NULL){
    return std::vector<std::vector<T>>(0);
  }
  if(n->val[n->disc_dim] < mins[n->disc_dim]){
    return orthogonal_search(n->left, mins, maxs);
  }
  if(n->val[n->disc_dim] > maxs[n->disc_dim]){
    return orthogonal_search(n->right, mins, maxs);
  }

  std::vector<std::vector<T>> left_match = orthogonal_search(n->left, mins, maxs);
  std::vector<std::vector<T>> right_match = orthogonal_search(n->right, mins, maxs);
  left_match.insert(left_match.end(), right_match.begin(), right_match.end());
  for(int i=0; i<num_dims; ++i){
    if(n->val[i] < mins[i] || n->val[i] > maxs[i]){
      return left_match;
    }
  }
  left_match.push_back(n->val);
  return left_match;
}

template <class T>
std::vector<T> KDTree<T>::nearest_neighbor(const std::vector<T>& e){
  std::pair<std::vector<T>,T> best(std::vector<T>(0), std::numeric_limits<int>::max());
  nearest_neighbor(root, e, best);
  return best.first;
}
template <class T>
T  sq_dist(const std::vector<T>& e1, const std::vector<T>& e2){
  T sumsq = 0;
  for(int i=0; i<e1.size(); ++i){
    T d = e1[i] - e2[i];
    sumsq += d*d;
  }
  return sumsq;
}

template <class T>
void KDTree<T>::nearest_neighbor(Node*n, const std::vector<T>& e, std::pair<std::vector<T>,T>& best){
  if(n == NULL){
    return;
  }
  if(n->val[n->disc_dim] <= e[n->disc_dim]){
    nearest_neighbor(n->left, e, best);
    T candidate_dist = sq_dist(n->val,e);
    if(candidate_dist < best.second){
      best.first = n->val;
      best.second = candidate_dist;
    }
    T b_dist = 2*e[n->disc_dim] - best.first[n->disc_dim];
    if(b_dist < best.first[n->disc_dim] && b_dist <= n->val[n->disc_dim] && n->val[n->disc_dim] <= best.first[n->disc_dim]){
      nearest_neighbor(n->right, e, best);
    }else if(b_dist >= best.first[n->disc_dim] && b_dist >= n->val[n->disc_dim] && n->val[n->disc_dim] >= best.first[n->disc_dim]){
      nearest_neighbor(n->right, e, best);
    }
  }else{
    nearest_neighbor(n->right, e, best);
    T candidate_dist = sq_dist(n->val,e);
    if(candidate_dist < best.second){
      best.first = n->val;
      best.second = candidate_dist;
    }
    T b_dist = 2*e[n->disc_dim] - best.first[n->disc_dim];
    if(b_dist < best.first[n->disc_dim] && b_dist <= n->val[n->disc_dim] && n->val[n->disc_dim] <= best.first[n->disc_dim]){
      nearest_neighbor(n->left, e, best);
    }else if(b_dist >= best.first[n->disc_dim] && b_dist >= n->val[n->disc_dim] && n->val[n->disc_dim] >= best.first[n->disc_dim]){
      nearest_neighbor(n->left, e, best);
    }
  }
}

template <class T>
std::vector<std::vector<T> > KDTree<T>::partial_match(const std::vector<T>& dims, const std::vector<T>& values){
  std::vector<T> mins(num_dims);
  std::vector<T> maxs(num_dims);
  for(int i=0; i<num_dims; ++i){
    mins[i] = std::numeric_limits<int>::min();
    maxs[i] = std::numeric_limits<int>::max();
    for(int j=0; j<dims.size(); ++j){
      if(dims[j] == i){
        mins[i] = values[j];
        maxs[i] = values[j];
        break;
      }
    }
  }
  return orthogonal_search(mins,maxs);
}
