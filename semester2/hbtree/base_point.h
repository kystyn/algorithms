#pragma once

#include "def.h"

template<typename T>
class point_component {
public:
  point_component( T val = T() ) : value(val) {}

  bool operator<( point_component<T> const &p ) {
    return value < p.value;
  }

  bool operator==( point_component<T> const &p ) const {
    return value == p.value;
  }

  virtual T getMedian( point_component<T> const &p ) const { return T(); }

  T getValue( void ) const { return value; }

protected:
  T value;
};

template<uint dimension, typename T>
class point {
public:
  point( array<shared_ptr<const point_component<T>>, dimension> const &c = {} ) : coordinates(c) {}

  point_component<T> operator[]( uint idx ) const {
    if (idx < dimension)
      return *coordinates[idx];
    return *coordinates.back();
  }

  shared_ptr<const point_component<T>> operator()( uint idx ) const {
    return coordinates[idx];
  }

  bool operator==( point const &p ) const {
    for (uint i = 0; i < coordinates.size(); i++)
      if (!(*coordinates[i] == p[i]))
        return false;
    return true;
  }

private:
  array<shared_ptr<const point_component<T>>, dimension> coordinates;
};
