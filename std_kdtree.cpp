#include <vector>
#include <iostream>
#include <tuple>
#include <fstream>
#include <cstdlib>
#include <random>

using namespace std;

// Structure to represent a k-dimensional point
struct Point
{
  vector<double> coordinates;
  Point(int k) : coordinates(k, -1.0) {} // Initialize with -1 to indicate unspecified coordinates
};

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
    if (type == Relaxed)
    {
      d = rand() % dim;
    }

    if (p == nullptr)
      return new node(key, d);

    if (key[d] < p->key[d])
      p->left = insert(p->left, key, d + 1);
    else
      p->right = insert(p->right, key, d + 1);

    return p;
  }

  bool match(const T &key, vector<double> q) const
  {
    for (int i = 0; i < dim; i++)
    {
      if (q[i] == -1.0)
        continue;
      else if (q[i] != key[i])
        return false;
    }

    return true;
  }

  int partial_match(node *p, vector<double> &q, vector<T> &L) const
  {
    if (p == nullptr)
      return 0;

    int visitedNodes = 1;

    // checks whether the current key matches the restrictions imposed by q. In that case, the key is added to the result list L.
    if (match(p->key, q))
    {
      L.push_back(p->key);
    }

    if (q[p->discr] == -1.0)
    {
      visitedNodes += partial_match(p->left, q, L);
      visitedNodes += partial_match(p->right, q, L);
    }
    else
    {
      if (q[p->discr] < p->key[p->discr])
        visitedNodes += partial_match(p->left, q, L);
      else
        visitedNodes += partial_match(p->right, q, L);
    }

    return visitedNodes;
  }

public:
  // Define an enum for the type property
  enum Type
  {
    Standard,
    Relaxed
  };

  Type type;

  kdtree(int K, Type t) : root(nullptr), dim(K), type(t)
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
  tuple<int, vector<T>> partial_match(vector<double> q)
  {
    vector<T> L;
    int visitedNodes = partial_match(root, q, L);

    return make_tuple(visitedNodes, L);
  }
  void printNode(const string &prefix, const node *n, bool isLeft)
  {
    if (n != nullptr)
    {
      cout << prefix;

      cout << (isLeft ? "├──" : "└──");

      // print the value of the node
      cout << n->discr << " - "
           << "(" << n->key[0] << " , " << n->key[1] << ", ... )" << endl;

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

// Function to generate random points in [0, 1]^k
vector<Point> generateRandomPoints(int n, int k)
{
  vector<Point> points;

  // Initialize random number generator
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0.0, 1.0);

  for (int i = 0; i < n; ++i)
  {
    Point point(k);
    for (int j = 0; j < k; ++j)
    {
      point.coordinates[j] = dis(gen);
    }
    points.push_back(point);
  }

  return points;
}

// Function to generate partial match queries with unspecified points
vector<Point> generatePartialMatchQueries(int q, int k, int s)
{
  vector<Point> queries = generateRandomPoints(q, k);
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, k - 1);
  for (auto &query : queries)
  {
    // Set k - s coordinates to -1 (unspecified)
    int unspecified = 0;
    while (unspecified < k - s)
    {
      int index = dis(gen);
      if (query.coordinates[index] != -1.0)
      {
        query.coordinates[index] = -1.0;
        unspecified++;
      }
    }
  }

  return queries;
}

double calculateAverage(const vector<int> &data)
{
  return accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double calculateVariance(const vector<int> &data, double mean)
{
  double variance = 0.0;
  for (int value : data)
  {
    variance += (value - mean) * (value - mean);
  }
  return variance / data.size();
}

int main(int argc, char *argv[])
{
  if (argc != 6)
  {
    cerr << "Usage: " << argv[0] << " <t> <n> <k> <q> <s>" << endl;
    return 1;
  }

  string t = argv[1];    // Type of the tree (standard or relaxed)
  int n = stoi(argv[2]); // Number of points
  int k = stoi(argv[3]); // Dimensionality
  int q = stoi(argv[4]); // Number of partial match queries
  int s = stoi(argv[5]); // Number of specified coordinates in the query

  vector<int> visitedNodesResults;
  // Generate random points
  vector<Point> points = generateRandomPoints(n, k);

  kdtree<vector<double>> kd_tree(k, kdtree<vector<double>>::Standard);

  if (t == "relaxed")
  {
    kd_tree.type = kdtree<vector<double>>::Relaxed;
  }

  for (const auto &point : points)
    kd_tree.insert(point.coordinates);

  // kd_tree.printTree();

  // Generate q partial match queries with unspecified points
  vector<Point> queries = generatePartialMatchQueries(q, k, s);

  // Perform each query and count visited nodes
  for (const auto &query : queries)
  {
    // Perform the partial match query
    auto result = kd_tree.partial_match(query.coordinates);
    int visitedNodes = get<0>(result);
    visitedNodesResults.push_back(visitedNodes);
    vector<vector<double>> matches = get<1>(result);
  }

  // Calculate statistical measures
  double average = calculateAverage(visitedNodesResults);
  double variance = calculateVariance(visitedNodesResults, average);

  // Output the results in a format that can be parsed by the shell script
  cout << "Average visited nodes: " << average << endl;
  cout << "Variance: " << variance << endl;

  return 0;
}