#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

class Graph
{
public:
    typedef adjacency_list<vecS, vecS, undirectedS> BaseGraph;
    typedef std::pair<int, int> BaseEdge;
    typedef property_map<BaseGraph, vertex_index_t>::type IndexMap;
    typedef graph_traits<BaseGraph>::vertex_descriptor Vertex;		
    typedef graph_traits<BaseGraph>::edge_descriptor Edge;

    typedef graph_traits<BaseGraph>::vertex_iterator Vertex_i;
    typedef graph_traits<BaseGraph>::edge_iterator Edge_i;

private:
    string name;
    BaseGraph graph;
    IndexMap index;

public:

    Graph(string & name);
    Graph(string & name, std::vector<BaseEdge> edges, int numberNodes);
    void printVertices(std::ostream& os) const;
    void printEdges(std::ostream& os) const;
    string getName() const;
};
 

std::ostream& operator<<(std::ostream& os, const Graph& obj); //, const Graph& obj);

#endif
