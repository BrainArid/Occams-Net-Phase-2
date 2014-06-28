#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;
typedef std::pair<int, int> BaseEdge;
typedef property_map<Graph, vertex_index_t>::type IndexMap;
typedef graph_traits<Graph> GraphTraits;
typedef GraphTraits::out_edge_iterator Edge_outItr;
typedef GraphTraits::in_edge_iterator Edge_inItr;
typedef Graph_traits<Graph>::adjacency_iterator Vertex_adjItr;
typedef Graph_traits<Graph>::vertex_descriptor Vertex;
typedef Graph_traits<Graph>::edge_descriptor Edge;
		
template <class Graph> struct exercise_vertex 
{
	exercise_vertex(Graph& g_) : g(g_) {}
    
	typedef typename graph_traits<Graph>::vertex_descriptor Vertex;

	void operator()(const Vertex& v) const
	{
		typedef graph_traits<Graph> GraphTraits;
		typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);

		std::cout << "out-edges: ";
		typename GraphTraits::out_edge_iterator out_i, out_end;
		typename GraphTraits::edge_descriptor e;
		for (tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i)
		{
			e = *out_i;
			Vertex src = source(e, g), targ = target(e, g);
			std::cout << "(" << index[src] << "," << index[targ] << ") ";
      	}
    	std::cout << std::endl;
    }

    Graph& g;
};

int main(int,char*[])
{

    // Make convenient labels for the vertices
    enum { A, B, C, D, E, N };
    const int num_vertices = N;
    const char* name = "ABCDE";

    // writing out the edges in the graph
    typedef std::pair<int, int> Edge;
    Edge edge_array[] = 
    { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
      Edge(C,E), Edge(B,D), Edge(D,E) };
    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

    // declare a graph object. Pass in pointers to begining and end of edge list
    Graph g(edge_array, edge_array + sizeof(edge_array) / sizeof(Edge), num_vertices);

    // get the property map for vertex indices
    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    IndexMap index = get(vertex_index, g);

    //Output Vertex index list
    std::cout << "vertices(g) = ";
    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    std::pair<vertex_iter, vertex_iter> vp; //iterator as a pair where '.second' is one past the end of the list.
    for (vp = vertices(g); vp.first != vp.second; ++vp.first)
      std::cout << index[*vp.first] <<  " ";
    std::cout << std::endl;

    //Output Edge list
    std::cout << "edges(g) = ";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)//use tie() function to capture each element of a pair<>
        std::cout << "(" << index[source(*ei, g)] 
                  << "," << index[target(*ei, g)] << ") ";
    std::cout << std::endl;

    //Exercise each vertex
    std::for_each(vertices(g).first, vertices(g).second,
                  exercise_vertex<Graph>(g));
    return 0;
}
