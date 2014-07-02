#include "Graph.h"
	//Constructors
Graph::Graph(string & name)
	{
	  this->name = name;
	}

Graph::Graph(string & name, std::vector<BaseEdge> edges, int numberNodes)
	{
	  this->name = name;
	  this->graph = BaseGraph(edges.begin(), edges.end(), numberNodes);
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
	  //Write out vertices
	  os << "Vertices: ";
	  Vertex_i v_i, v_end;
	  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
	    {
	      os << index[*v_i] << ", ";
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
