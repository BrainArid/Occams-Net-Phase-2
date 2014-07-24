#include "Graph.h"
//Constructors
Graph::Graph(string & name)
{
  this->name = name;
}

Graph::Graph(string & name, std::vector<BaseEdge> edges, int numberNodes) //doesn't add edges yet
{
  this->name = name;
	  this->graph = BaseGraph(numberNodes);
	  // this->graph = BaseGraph(edges.begin(), edges.end(), numberNodes);
	  this->index = get(vertex_index, graph);
}

std::ostream& operator<<(std::ostream& os, const Graph& obj)
{
  // write obj to stream
  os << "Graph: " << obj.getName() << std::endl;
  obj.printVertices(os);
  obj.printEdges(os);
  os << std::endl;
  return os;
}

void Graph::printVertices(std::ostream& os) const
{
  //boost::vertex_bundle_type<Graph>::type vertexProp = get(vertex_bundle, graph);
  //typedef property_map<BaseGraph, edge_bundle_t>::type ewm_t;
  //ewm_t ewm = get (edge_bundle, graph);
  
  //Write out vertices
  os << "Vertices: ";
  Vertex_i v_i, v_end;
  int i = 0;
  
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    {
      os << "[" << i++ << "] " << graph[*v_i].name << ", ";
    }
  os << std::endl;
}

void Graph::printEdges(std::ostream& os) const
{
  
  //Write out edges
  os << "Edges: ";
  Edge_i e_i, e_end;
  for( tie(e_i, e_end) = edges(graph); e_i != e_end; e_i++)
    {
      os << "(" << index[source(*e_i, graph)] 
	 << "," << index[target(*e_i, graph)] << ") ";
    }
  os << std::endl;
}

string Graph::getName() const
{
  return this->name;
}

int Graph::vertCount() const
{
  return num_vertices(graph); 
}


int Graph::edgeCount() const
{
  return num_edges(graph);
}

string Graph::getVertexName( const Graph::Vertex & v) const
{
  return graph[v].name;
}

void Graph::addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2)
{
  //if graph doesn't contain a node for vert1, add it and save index. If so, get it's index.
  Vertex v1; 
  Vertex v2;
  
  //if not found
  if(!findVertexByName(vert1.name, v1))
    {
      v1 = add_vertex(vert1, graph);
      cout << "Added vertex " << graph[v1].name << " to graph " << this->name << endl;
    }

  //if not found
  if(!findVertexByName(vert2.name, v2))
    {
      v2 = add_vertex(vert2, graph);
      cout << "Added vertex " << graph[v2].name << " to graph " << this->name << endl;
    }  
  
  //add new edge to graph with appropriate edge indecies
  //graph.add_edge(make_pair(index1, index2));
  add_edge(v1, v2, graph);
  cout << "Added edge <" << graph[v1].name << ", " << graph[v2].name << "> to graph " << this->name << endl;

}

bool Graph::findVertexByName(const string & name, Vertex & vert) const
{
  //int index = -1;
  Vertex_i v_i, v_end;
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    {
      if(graph[*v_i].name.compare(name) == 0)
	{
	  vert = *v_i;
	  return true;
	}
    }

  return false;
}

std::pair< Graph::Adjacency_i, Graph::Adjacency_i> Graph::getAdjacentVertices(const Graph::Vertex & v) const
{
  return adjacent_vertices(v, graph);
}

bool Graph::getVisitedVertex(const Graph::Vertex & v) const
{
  return graph[v].visited;
}

void Graph::setVisitedVertex(const Graph::Vertex & v, bool value)
{
   graph[v].visited = value;
}

void Graph::resetVisitedVertices()
{
  Vertex_i v_i,v_end;
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    graph[*v_i].visited = false;
}
