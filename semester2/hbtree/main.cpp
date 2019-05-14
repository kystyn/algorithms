#include "kdtree.h"
#include "base_point.h"

class double_point_component : public point_component<double> {
public:
  double_point_component( uint x ) : point_component<double>(x) {}

  double getMedian( point_component<double> const &p ) const override {
    return (value + p.getValue()) / 2;
  }
};

int main( void ) {
  kd_tree<2, double> t;
  array<shared_ptr<point_component<double>>, 2> pt {
    make_shared<double_point_component>(2),
    make_shared<double_point_component>(3)
  };

  point<2, double> p(pt);
  t.add(p);

  pt[0] = make_shared<double_point_component>(4);
  p = {pt};
  t.add(p);

  pt[1] = make_shared<double_point_component>(4);
  p = {pt};
  t.add(p);

  pt[0] = make_shared<double_point_component>(5);
  p = {pt};
  t.add(p);

  t.remove(pt);
  return 0;
}