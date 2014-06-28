typedef adjacency_list<vecS, vecS, undirectedS> UndirectedGraph;

class Graph()
{

	private:
		typedef adjacency_list<vecS, vecS, bidirectionalS> BaseGraph;
		typedef std::pair<int, int> BaseEdge;
		typedef property_map<BaseGraph, vertex_index_t>::type IndexMap;
		typedef graph_traits<BaseGraph>::vertex_descriptor Vertex;
		typedef graph_traits<BaseGraph>::edge_descriptor Edge;	
		
		string name;
	
	public:

	
	/*
	typedef graph_traits<Graph> GraphTraits;
	typename property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);
	typename GraphTraits::out_edge_iterator out_i, out_end;
	typename GraphTraits::edge_descriptor e;
	*/

	Graph(string & name)
	{
		this.name = name;
	};
	
	std::ostream& operator<<(std::ostream& os, const T& obj)
	{
	  // write obj to stream
	  return os;
	}
}
