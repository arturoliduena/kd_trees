### standard kd-trees
The kd-tree is a structure proposed by Bentley that generalizes the
binary search tree for multiple dimensions. Consider a set of k-dimensional
keys (say, arrays of size k starting at 0) that we want to store. Each node
of the kd-tree has one of the keys and one discriminant associated to it.
Every discriminant is an integer between 0 and k 􀀀 1. Initially, the root
represents the whole space. Let x be the key at the root and let i be its
discriminant. Then, the space is partitioned in two regions with respect to
x[i]: All the keys y with y[i] < x[i] go into the left subtree, and all the keys
with y[i]   x[i] go into the right subtree. The same method of partitioning
the space is recursively applied to all subtrees, until empty trees are reached.
The discriminant at every node is chosen alternating the coordinates of each
level, starting with 0: we use dimension 0 at the root, then dimension 1,
then dimension 2, . . . , then dimension k 􀀀 1, then dimension 0 again, etc.
