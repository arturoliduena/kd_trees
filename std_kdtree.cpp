#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class kdtree
{
  struct node
  {
    T key;
    int discr;
    node *left;
    node *right;
    node(const T &k, int d) : key(k), discr(d), left(nullptr), right(nullptr)
    {
    }
    ~node()
    {
      delete left;
      delete right;
    }
  };

  node *root;

  int dim;

  kdtree(const kdtree &t){};

  kdtree<T> &operator=(const kdtree &t)
  {
    return *this;
  }

  node *insert(node *p, const T &key, int d)
  {
    d = d % dim;

    if (p == nullptr)
      return new node(key, d);

    if (key[d] < p->key[d])
      p->left = insert(p->left, key, d + 1);
    else
      p->right = insert(p->right, key, d + 1);

    return p;
  }

  bool match(const T &key, vector<int *> q) const
  {
    for (int i = 0; i < dim; i++)
    {
      if (q[i] == nullptr)
        continue;
      else if (*q[i] != key[i])
        return false;
    }

    return true;
  }

  void partial_match(node *p, vector<int *> &q, vector<T> &L) const
  {
    if (p == nullptr)
      return;
    // checks whether the current key matches the restrictions imposed by q. In that case, the key is added to the result list L.
    if (match(p->key, q))
    {
      L.push_back(p->key);
    }

    if (q[p->discr] == nullptr)
    {
      partial_match(p->left, q, L);
      partial_match(p->right, q, L);
    }
    else
    {
      if (*q[p->discr] < p->key[p->discr])
        partial_match(p->left, q, L);
      else
        partial_match(p->right, q, L);
    }
  }

public:
  kdtree(int K) : root(nullptr), dim(K)
  {
  }
  ~kdtree()
  {
    delete root;
  }
  void insert(const T &key)
  {
    root = insert(root, key, 0);
  }
  vector<T> partial_match(vector<int *> q)
  {
    vector<T> L;
    partial_match(root, q, L);

    return L;
  }
  void printNode(const string &prefix, const node *n, bool isLeft)
  {
    if (n != nullptr)
    {
      std::cout << prefix;

      std::cout << (isLeft ? "├──" : "└──");

      // print the value of the node
      std::cout << n->discr << " - "
                << "(" << n->key[0] << " , " << n->key[1] << ")" << std::endl;

      // enter the next tree level - left and right branch
      printNode(prefix + (isLeft ? "│   " : "    "), n->left, true);
      printNode(prefix + (isLeft ? "│   " : "    "), n->right, false);
    }
  }

  void printTree()
  {
    printNode("", root, false);
  }
};

int main()
{
  // The following code creates a 2D k-d tree and inserts the points
  vector<vector<int>> points = {{6, 4}, {5, 2}, {4, 7}, {8, 6}, {2, 1}, {9, 3}, {2, 8}, {5, 4}, {5, 8}};
  kdtree<vector<int>> k(2);

  for (auto p : points)
    k.insert(p);

  k.printTree();

  vector<int *> q;
  // Adding integers and nulls to the vector
  q.push_back(new int(5)); // Adding an integer
  q.push_back(nullptr);    // Adding a null pointer

  cout << "Partial match: " << endl;

  vector<vector<int>> L = k.partial_match(q);
  for (auto p : L)
  {
    cout << "(" << p[0] << ", " << p[1] << ")" << endl;
  }

  return 0;
}