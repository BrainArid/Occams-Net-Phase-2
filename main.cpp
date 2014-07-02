#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "Graph.h"

using namespace std;
using namespace boost;

int main(int,char*[])
{
  string name = "PPI";
  vector<Graph::BaseEdge> edges;
  edges.push_back(make_pair(1,2));
  Graph IntrNet(name,edges,3); //Interaction Network
  cout << IntrNet << endl;

  //get parameters

  //construct IntrNet

  //construct steinerTree as subgraph (may not be able to assume subgraph)

  //construct ontology

  //calculate inner score and outer and middle scores

  //output scores per gene set

  return 0;
}
