#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <utility>
//#include <tuple>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
//#include "subgraph.hpp"
//#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

class Graph
{
public:

    struct VertexProp{
      string name;
      float weight;
      float reWeight; //this should be an external property
      bool visited; //this should probably be an external property
      bool reweighted;
      VertexProp(string name = "", float weight = -1, bool visited = false)
      {
	this->name = name;
	this->weight = weight;
	this->visited = visited;
	reWeight = 0;
	reweighted = false;
      }
    };

    struct EdgeProp{
      float weight;

      EdgeProp(float weight = -1)
      {
	this->weight = weight;
      }
    };


//typedef subgraph <adjacency_list<vecS, vecS, undirectedS, property<vertex_index_t,int,VertexProp>, property<edge_index_t,int,EdgeProp> > > BaseGraph;
typedef adjacency_list<vecS, vecS, undirectedS, VertexProp, EdgeProp > BaseGraph;
    typedef std::pair<int, int> BaseEdge;
    typedef property_map<BaseGraph, vertex_index_t>::type IndexMap;
    typedef graph_traits<BaseGraph>::vertex_descriptor Vertex;		
    typedef graph_traits<BaseGraph>::edge_descriptor Edge;

    typedef graph_traits<BaseGraph>::vertex_iterator Vertex_i;
    typedef graph_traits<BaseGraph>::edge_iterator Edge_i;
    typedef graph_traits<BaseGraph>::adjacency_iterator Adjacency_i;
    typedef graph_traits<BaseGraph>::out_edge_iterator Out_Edge_i;

private:
    string name;
    BaseGraph graph;
//subgraph < BaseGraph > steinerGraph;
    IndexMap index;

    void addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2, bool useEdgeProp, EdgeProp & edge);

public:

    Graph(string & name);
    Graph(string & name, std::vector<BaseEdge> edges, int numberNodes);
    void printVertices(std::ostream& os) const;
    void printEdges(std::ostream& os) const;
    string getName() const;
    int vertCount() const;
    int edgeCount() const;
    string getVertexName( const Graph::Vertex & v) const;
    float getVertexWeight( const Graph::Vertex & v) const;
    void setVertexWeight( const Graph::Vertex & v, float value);
    float getVertexReweight( const Graph::Vertex & v) const;
    void setVertexReweight( const Graph::Vertex & v, float value);
    void fadeVertexWeights(Graph::Vertex & v, float fade, int tabs);
    void addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2);
    void addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2, EdgeProp & edge);
    bool findVertexByName(const string & name, Vertex & vert) const;
   std::pair< Adjacency_i, Adjacency_i> getAdjacentVertices(const Graph::Vertex & v) const;
    bool getVisitedVertex(const Graph::Vertex & v) const;
    void setVisitedVertex(const Graph::Vertex & v, bool value);
    void resetVisitedVertices();
    void resetReweightedFlagVertices();
    void finalizeReweightedVertices();
};
 

std::ostream& operator<<(std::ostream& os, const Graph& obj); //, const Graph& obj);

#endif
