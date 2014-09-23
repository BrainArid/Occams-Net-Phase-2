#include "Graph.h"
#include "VertexSet.h"

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
      os << "[" << i++ << "] " << graph[*v_i].name << " = " << graph[*v_i].weight << ", ";
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
	 << "," << index[target(*e_i, graph)] << ") = "
	 << graph[*e_i].weight << " ";
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

float Graph::getVertexWeight( const Graph::Vertex & v) const
{
  return graph[v].weight;
}

void Graph::setVertexWeight( const Graph::Vertex & v, float value)
{
  graph[v].weight = value;
}

float Graph::getVertexReweight( const Graph::Vertex & v) const
{
  return graph[v].reWeight;
}

void Graph::setVertexReweight( const Graph::Vertex & v, float value)
{
  graph[v].reWeight = value;
}

void Graph::fadeVertexWeights(Vertex & v, float fade, int jumps, const int MAX_JUMPS, const float MIN_FADE)
{
  for(int i = 0; i < jumps; i++)
     cout << "\t";
  cout << graph[v].name << "\t" << graph[v].weight << "\t" << graph[v].reWeight << "\t" << fade << endl;

  //for each neighbor of v
  Out_Edge_i ne_i, ne_e;
  for(tie(ne_i, ne_e) = out_edges(v, graph); ne_i != ne_e; ne_i++)
    {
      Vertex neighbor = target(*ne_i, graph);
      //calculate faded weight
      float newFade = fade * graph[*ne_i].weight;
      float newWeight = graph[neighbor].weight * newFade;

      //if calculated weight of neighbor is larger than existing weight and not visited (meaning not in original focus set) then recurse
      if(!graph[neighbor].flag && (MAX_JUMPS == -1 ||jumps < MAX_JUMPS) && (MIN_FADE == -1.0 || newFade > MIN_FADE))
	{
	  if(newWeight > graph[neighbor].reWeight)
	    {
	      graph[neighbor].reWeight = newWeight;
	      graph[neighbor].reweighted = true;
	    }

	  graph[neighbor].flag = true;
	  graph[neighbor].visited = true;
	  fadeVertexWeights(neighbor, newFade, jumps + 1, MAX_JUMPS, MIN_FADE);
	  graph[neighbor].flag = false;
	}
    }

  //graph[v].visited = true;
}

void Graph::addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2, EdgeProp & edge)
{
  addEdgeByNamedVertices(vert1, vert2, true, edge);
}

void Graph::addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2)
{
  EdgeProp edge = EdgeProp();
  addEdgeByNamedVertices(vert1, vert2, false, edge);
}

void Graph::addEdgeByNamedVertices(const VertexProp & vert1, const VertexProp & vert2, bool useEdgeProp, EdgeProp & edge)
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
  if(!useEdgeProp)
    add_edge(v1, v2, graph);
  else
    add_edge(v1, v2, edge, graph);

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

bool Graph::getFlagVertex(const Graph::Vertex & v) const
{
  return graph[v].flag;
}

void Graph::setFlagVertex(const Graph::Vertex & v, bool value)
{
   graph[v].flag = value;
}

void Graph::resetVisitedVertices()
{
  Vertex_i v_i,v_end;
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    graph[*v_i].visited = false;
}

void Graph::resetFlagVertices()
{
  Vertex_i v_i,v_end;
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    graph[*v_i].flag = false;
}

void Graph::resetReweightedFlagVertices()
{
  Vertex_i v_i,v_end;
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    graph[*v_i].reweighted = false;
}

void Graph::finalizeReweightedVertices()
{
  Vertex_i v_i,v_end;
  for( tie(v_i, v_end) = vertices(graph); v_i != v_end; v_i++)
    {
      if(graph[*v_i].reweighted && graph[*v_i].visited)
	{
	  graph[*v_i].weight = graph[*v_i].reWeight;
	  graph[*v_i].reWeight = 0;
	  graph[*v_i].reweighted = false;
	  graph[*v_i].visited = false;
	}
      else if(!graph[*v_i].reweighted && !graph[*v_i].visited)
	{
	  graph[*v_i].weight = -1;
	  graph[*v_i].visited = false;
	}
      else if(!graph[*v_i].reweighted && graph[*v_i].visited)
	{
	  graph[*v_i].visited = false;
	}
      else
	cout << "Error: Vertex was reweighted without being visited!" << endl;
    }
}
