#pragma once

#include "base_point.h"

template<uint dimension, typename T>
struct kd_tree_node {
  kd_tree_node( bool isLeaf ) : isLeaf(isLeaf) {}

  kd_tree_node( point<dimension, T> const &point ) : point(point) {}

  point_component<T> operator[]( uint idx ) const {
    return point[idx];
  }

  point<dimension, T> point;
  point_component<T> hyperplaneEquation;
  shared_ptr<kd_tree_node<dimension, T>> left, right;
  bool isLeaf;
};

template<uint dimension, typename T>
class kd_tree {
public:
  kd_tree( void ) :
    root(make_shared<kd_tree_node<dimension, T>>(false)) {}

  kd_tree & add( point<dimension, T> const & p ) {
    uint level = 0;

    if (root->left == nullptr && root->right == nullptr && !root->isLeaf) { // empty tree
      root = make_shared<kd_tree_node<dimension, T>>(p);
      root->isLeaf = true;
      return *this;
    }

    auto cur = root;

    search(p, cur, make_unique<uint>(level));

    auto save = make_shared<kd_tree_node<dimension, T>>(*cur);
    cur->isLeaf = false;
    cur->point = p;
    cur->hyperplaneEquation = save->point(level % dimension)->getMedian((*cur)[level % dimension]);

    if ((*save)[level % dimension] < cur->hyperplaneEquation) {
      cur->left = save;
      cur->right = make_shared<kd_tree_node<dimension, T>>(kd_tree_node<dimension, T>(p));
      cur->right->isLeaf = true;
    }
    else {
      cur->left = make_shared<kd_tree_node<dimension, T>>(kd_tree_node<dimension, T>(p));
      cur->right = save;
      cur->left->isLeaf = true;
    }
    return *this;
  }

  kd_tree & remove( shared_ptr<point<dimension, T>> p ) {
    auto cur = root, prev = cur;

    while (!cur->isLeaf) {
      prev = cur
      if (p[level % dimension] < cur->hyperplaneEquation)
        cur = cur->left;
      else
        cur = cur->right;
      level++;
    }

    if (!(*cur == *p))
      return *this;

    prev->isLeaf = true;
    if (*prev->left == *cur)
      prev->point = prev->right->point;
    else
      prev->point = prev->left->point;

    delete cur;
    return *this;
  }

  bool search( point<dimension, T> const &p,
    shared_ptr<kd_tree_node<dimension, T>> cur, std::unique_ptr<uint> level = 0 ) const {
    cur = root;

    while (!cur->isLeaf) {
      if (p[*level % dimension] < cur->hyperplaneEquation)
        cur = cur->left;
      else
        cur = cur->right;
      (*level)++;
    }

    return cur->point == p;
  }

private:
  shared_ptr<kd_tree_node<dimension, T>> root;
};
