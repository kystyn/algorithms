#include "kdtree.h"
#include "base_point.h"

class uint_point_component : public point_component<uint> {
public:
  uint_point_component( uint x ) : point_component<uint>(x) {}

  uint getMedian( point_component<uint> const &p ) const override {
    return (value + p.getValue()) / 2;
  }
};

int main( void ) {
  kd_tree<2, uint> t;
  array<shared_ptr<const point_component<uint>>, 2> pt {
    make_shared<uint_point_component>(2),
    make_shared<uint_point_component>(3)
  };

  point<2, uint> p(pt);
  t.add(p);

  pt[0] = make_shared<uint_point_component>(4);
  p = {pt};
  t.add(p);
  return 0;
}