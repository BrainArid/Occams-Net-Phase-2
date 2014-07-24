#ifndef SUBGRAPHLINKER_H
#define SUBGRAPHLINKER_H

//Created this class as a workaround for the BGL's problematic simultaneous handling of bundled properties and subgraphs. We can maintain the simplicity of bundled properties by handling the subgraph information externally. This will of course require more memory than the BGL's own subgraph implementation...something to consider down the line.

#include <vector>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "subgraph.hpp"
//#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

class Graph
{

private:
    Graph & parent, child;
vector<int> p2c_VertMap;//parent to child vertex map
vector<int> c2p_VertMap;//child to parent vertex map

    Graph::Vertex getVertex(const Graph::Vertex v, bool from1) const;

public:

    SubgraphLinker(const Graph & parent, const Graph & child);
    void linkVertexIndices();
    Graph::Vertex getParentVertex(const Graph::Vertex v) const;
    Graph::Vertex getChildVertex(const Graph::Vertex v) const;
};
 
#endif
