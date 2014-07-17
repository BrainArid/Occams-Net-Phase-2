#include <boost/graph/adjacency_list.hpp>
#include "subgraph.hpp"

using namespace boost;
 
struct VertexProperty
{
  // ...
};
 
struct EdgeProperty
{
  double length_;
  // ...
};
 
typedef adjacency_list<vecS, vecS, bidirectionalS,
                 property<vertex_index_t, int, VertexProperty>,
                 property<edge_index_t, int, EdgeProperty> >  Graph;


typedef subgraph<Graph> Subgraph;

int main ()
{
  Subgraph sg(10);
  typedef property_map<Subgraph, edge_bundle_t>::type ewm_t;
  ewm_t ewm = get (edge_bundle, sg);
  typedef property_map<Subgraph, edge_index_t>::type ewm1_t;
  typedef property_map<Subgraph, double EdgeProperty::*>::type ewm2_t;
  ewm2_t ewm2 = get (&EdgeProperty::length_, sg);
  // use ewm in an algorithm ...
  return 0;
}
