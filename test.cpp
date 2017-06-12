#include "kd_tree.cpp"

#include <iostream>
#include <limits>

using namespace std;


vector<vector<int> > rand_elements(unsigned int seed, int k, int size, int max, int min){
  vector<vector<int> >  res(size, vector<int>(k));
  srand(seed);
  for (int i=0; i <size; i++){
    for (int j=0; j<k; ++j){
      res[i][j] = rand() % max + min;
    }
  }
  return res;
}

void print_dim_vector(const vector<vector<int> > &vec){
  for(vector<int> v : vec){
    cout << '{';
    for(int e : v){
      cout << e << ',';
    }
    cout << "\b}" << endl;
  }
  cout << endl;
}

void print_element(const vector<int> & vec){
  cout << "{";
  for(int e : vec){
    cout << e << ',';
  }
  cout << "\b}" << endl;
}

bool aInb(const vector<int>& a, const vector<vector<int> >& b){
  for(auto e: b){
    if(e == a){
      return true;
    }
  }
  return false;
}

bool check_results(const vector<vector<int> >& elements, const vector<vector<int> >& results, const std::vector<int>& min, const std::vector<int>& max){
  for(vector<int> e: elements){
    bool shouldBe = true;
    for(int i=0; i<e.size(); ++i){
      if(min[i] > e[i] || max[i] < e[i]){
        shouldBe = false;
      }
    }
    if(shouldBe && !aInb(e, results)){
      cout << "\t Failed test in element ";
      print_element(e);
      cout << endl;
      return false;
    }else if(!shouldBe && aInb(e, results)){
      cout << "\t Failed test in element: ";
      print_element(e);
      cout << endl;
      return false;
    }
  }
  return true;
}

int  sq_dist(const std::vector<int>& e1, const std::vector<int>& e2){
  int sumsq = 0;
  for(int i=0; i<e1.size(); ++i){
    int d = e1[i] - e2[i];
    sumsq += d*d;
  }
  return sumsq;
}

bool check_resultsNN(const vector<vector<int> >& elements, const vector<int>& results, const std::vector<int>& e){
  vector<int> NN = elements[0];
  int sqdist = sq_dist(NN, e);
  for(int i=1; i<elements.size(); ++i){
    int cand_dist = sq_dist(elements[i], e);
    if(sqdist > cand_dist){
      sqdist = cand_dist;
      NN = elements[i];
    }
  }
  if(NN == results){
    return true;
  }
  cout << "Test failed got: ";
  print_element(results);
  cout << "Should get: " << sqdist;
  print_element(NN);
  return false;
}

bool do_test(int testid, const vector<vector<int> >& elements, const vector<vector<int> >& results, const std::vector<int>& min, const std::vector<int>& max){
  cout << "Starting test " << testid << ":" << endl;
  bool result = false;
  if(check_results(elements, results, min, max)){
    cout << "\t Test successful!" << endl;
    result = true;
  }
  cout << endl;
  return result;
}

bool do_testNN(int testid, const vector<vector<int> >& elements, const vector<int>& results, const std::vector<int>& e){
  cout << "Starting test " << testid << ":" << endl;
  bool result = false;
  if(check_resultsNN(elements, results, e)){
    cout << "\t Test successful!" << endl;
    result = true;
  }
  cout << endl;
  return result;
}

void test1(){
  KDTree<int> tree(1);
  vector<vector<int> > elements = rand_elements(11234, 1, 1000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({5});
  vector<int> max({10});
  do_test(1,elements, tree.orthogonal_search(min, max), min, max);
}

void test2(){
  KDTree<int> tree(2);
  vector<vector<int> > elements = rand_elements(21234, 2, 1000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({5,5});
  vector<int> max({10,15});
  do_test(2,elements, tree.orthogonal_search(min, max), min, max);
}

void test3(){
  KDTree<int> tree(3);
  vector<vector<int> > elements = rand_elements(31234, 3, 1000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({5,5, 2});
  vector<int> max({10,15, 14});

  do_test(3,elements, tree.orthogonal_search(min, max), min, max);
}

void test4(){
  KDTree<int> tree(4);
  vector<vector<int> > elements = rand_elements(41234, 4, 1000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({5,5, 2, 9});
  vector<int> max({10,15, 14, 20});

  do_test(4,elements, tree.orthogonal_search(min, max), min, max);
}

void test5(){
  KDTree<int> tree(5);
  vector<vector<int> > elements = rand_elements(51234, 5, 1000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({5,5, 2, 9, 4});
  vector<int> max({10,15, 14, 20, 18});

  do_test(5, elements, tree.orthogonal_search(min, max), min, max);
}

void test1NN(){
  KDTree<int> tree(1);
  vector<vector<int> > elements = rand_elements(111234, 1, 30, 500, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> element({50});
  do_testNN(6, elements, tree.nearest_neighbor(element), element);
}

void test2NN(){
  KDTree<int> tree(2);
  vector<vector<int> > elements = rand_elements(211234, 2, 30, 500, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> element({50, 100});
  do_testNN(7, elements, tree.nearest_neighbor(element), element);
}

void test3NN(){
  KDTree<int> tree(3);
  vector<vector<int> > elements = rand_elements(211234, 3, 30, 500, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> element({50, 100, 150});
  do_testNN(8, elements, tree.nearest_neighbor(element), element);
}

void test4NN(){
  KDTree<int> tree(4);
  vector<vector<int> > elements = rand_elements(211234, 4, 30, 500, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> element({50, 100, 150, 200});
  do_testNN(9, elements, tree.nearest_neighbor(element), element);
}

void test5NN(){
  KDTree<int> tree(5);
  vector<vector<int> > elements = rand_elements(211234, 5, 30, 500, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> element({50, 100, 150, 200, 250});
  do_testNN(10, elements, tree.nearest_neighbor(element), element);
}

void testPM2(){
  KDTree<int> tree(2);
  vector<vector<int> > elements = rand_elements(521234, 2, 1000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({10,std::numeric_limits<int>::min()});
  vector<int> max({10,std::numeric_limits<int>::max()});
  //print_dim_vector(tree.partial_match({0}, {10}));
  do_test(11,elements, tree.partial_match({0}, {10}), min, max);
}

void testPM5(){
  KDTree<int> tree(5);
  vector<vector<int> > elements = rand_elements(521234, 5, 3000, 20, 0);
  for(vector<int> e : elements){
    tree.insert(e);
  }
  vector<int> min({10,std::numeric_limits<int>::min(),5, std::numeric_limits<int>::min(), std::numeric_limits<int>::min() });
  vector<int> max({10,std::numeric_limits<int>::max(),5, std::numeric_limits<int>::max(), std::numeric_limits<int>::max()  });
  //print_dim_vector(tree.partial_match({0}, {10}));
  do_test(12,elements, tree.partial_match({0,2}, {10, 5}), min, max);
}

int main(){
  //Test orthogonal search in 1-5 dimensions
  test1();
  test2();
  test3();
  test4();
  test5();

  //Test nearest neighbor
  test1NN();
  test2NN();
  test3NN();
  test4NN();
  test5NN();

  //Test partial match
  testPM2();
  testPM5();
}
