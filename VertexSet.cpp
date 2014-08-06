#include "VertexSet.h"

void VertexSet::buildSet (const vector<string> & vertNames)
{
  
  for(vector<string>::const_iterator it = vertNames.begin(); it != vertNames.end(); it++)
    {
      Graph::Vertex vert;
      if(this->graph.findVertexByName(*it, vert))
	{
	  this->setInGraph.push_back(vert);
	}
      else
	{
	  this->setNotInGraph.push_back(*it);
	}
    }
}

VertexSet::VertexSet(const string & name, const vector<Graph::Vertex> & setInGraph, const vector<string> & setNotInGraph, Graph & graph)
  : graph(graph)
{
  this->name = name;
  this->setInGraph = setInGraph;
  this->setNotInGraph = setNotInGraph;
}

VertexSet::VertexSet(const string & name, const vector<string> & vertNames, Graph & graph)
  : graph(graph)
{
  this->name = name;
  this->buildSet(vertNames);
}

VertexSet::VertexSet(const string & name, Graph & graph)
  : graph(graph)
{
  this->name = name;
}

int VertexSet::intersectionCount(const VertexSet & other) const
{
  return -2;
}

int VertexSet::intersectionInGraphCount(const VertexSet & other) const
{
  //must share same graph..check this
  int num = 0;
  //cout << "TEST:" << &this->graph << " ==? " << &other.getGraph() << endl;
  if(!((&this->graph)==(&other.getGraph())))
    num = -1;
  else
    {
      vector<Graph::Vertex>::const_iterator vi, ve;
      for(tie(vi, ve) = verticesInGraph(); vi != ve; vi++)
	{
	  bool found = false;
	  vector<Graph::Vertex>::const_iterator ovi, ove;
	  for(tie(ovi, ove) = other.verticesInGraph(); ovi != ove && !found; ovi++)
	    {
	      if(*ovi == *vi)
		{
		  num++;
		  found = true;
		}
	    }
	}
    }

  return num;
}

pair<vector<Graph::Vertex>::const_iterator, vector<Graph::Vertex>::const_iterator >   VertexSet::verticesInGraph() const
  {
    return make_pair(setInGraph.begin(), setInGraph.end());
  }

pair<vector<string>::const_iterator, vector<string>::const_iterator > VertexSet::verticesNotInGraph() const
  {
    return make_pair(setNotInGraph.begin(), setNotInGraph.end());
  }

const Graph & VertexSet::getGraph() const
{
  return this->graph;
}

void VertexSet::unionAdd(const string & name)
{

  //exit if in setInGraph, because there's no need to add name
  for(vector<Graph::Vertex>::const_iterator it = setInGraph.begin(); it !=  setInGraph.end(); it++)
    {
      if(name.compare(graph.getVertexName(*it)) == 0)
	return;
    }

  //exit if in setNotInGraph, because there's no need to add name
  for(vector<string>::const_iterator it = setNotInGraph.begin(); it !=  setNotInGraph.end(); it++)
    {
      if(name.compare(*it) == 0)
	return;
    }

  Graph::Vertex vert;
  //otherwise add name to the appropriate set
  if(this->graph.findVertexByName(name, vert))
    {
      this->setInGraph.push_back(vert);
    }
  else
    {
      this->setNotInGraph.push_back(name);
    }
}

void VertexSet::addInGraph(const Graph::Vertex & v)
{
  this->setInGraph.push_back(v);
}

//Graph& VertexSet::getGraph() const;
string VertexSet::getName() const
{
  return name;
}

std::ostream& operator<<(std::ostream& os, const VertexSet& obj)
	{
	  // write obj to stream
	  os << "For graph: " << obj.getGraph().getName() << std::endl;
	  os << "VertexSet: " << obj.getName() << std::endl;
	  obj.printInVertices(os);
	  obj.printNotInVertices(os);
	  os << std::endl;
	  return os;
	}

void VertexSet::printInVertices(std::ostream& os) const
	{
	  //Write out vertices
	  os << "Vertices in graph: ";
	  Graph::Vertex_i v_i, v_end;
	  int i = 0;
	  
	  for(vector<Graph::Vertex>::const_iterator it = setInGraph.begin(); it !=  setInGraph.end(); it++){
	    os << "[" << i++ << "] " << graph.getVertexName(*it) << " = " << graph.getVertexWeight(*it) << ", ";
	    }
	  os << std::endl;
	}

void VertexSet::printNotInVertices(std::ostream& os) const
	{
	  //Write out vertices
	  os << "Vertices not in graph: ";
	  int i = 0;
	  
	  for(vector<string>::const_iterator it = setNotInGraph.begin(); it !=  setNotInGraph.end(); it++)
    {
	      os << "[" << i++ << "] " << *it << ", ";
	    }
	  os << std::endl;
	}
unsigned int VertexSet::inGraphSize() const
{
  return this->setInGraph.size();
}

void VertexSet::getHalo(VertexSet & halo) const
{
  //must share same graph..check this
  if(!((&this->graph)==(&halo.getGraph())))
    return;

  //set all vertices visited bool to false
  graph.resetVisitedVertices();

  //mark all original set vertices as already visited so they won't be included in the halo
  for(vector<Graph::Vertex>::const_iterator it = setInGraph.begin(); it != setInGraph.end(); it++)
    {
      graph.setVisitedVertex(*it, true);
    }

  //add all halo vertices to halo
  for(vector<Graph::Vertex>::const_iterator it = setInGraph.begin(); it != setInGraph.end(); it++)
    {
      Graph::Adjacency_i n_i, n_end;
      for(tie(n_i, n_end) = graph.getAdjacentVertices(*it); n_i != n_end; n_i++)
	  {
	    //if not marked as visited, mark as visited and unionAdd
	    if(!graph.getVisitedVertex(*n_i))
	      {
		graph.setVisitedVertex(*n_i, true);
		halo.addInGraph(*n_i);
	      }
	  }
    }
}

void VertexSet::fadeVertexWeights()
{
  //set all visited = false. Visited will be used to represent focusSet nodes (faster than searching through the set at every recursion)
  graph.resetVisitedVertices();
  graph.resetReweightedFlagVertices();
  
  //Set focusSet visited = true
   for(vector<Graph::Vertex>::iterator v_i = setInGraph.begin(); v_i != setInGraph.end(); v_i++)
     graph.setVisitedVertex(*v_i, true);
  
  for(vector<Graph::Vertex>::iterator v_i = setInGraph.begin(); v_i != setInGraph.end(); v_i++)
    {
      cout << "Fading initiated from " << graph.getVertexName(*v_i) << "..." << endl;
      graph.fadeVertexWeights(*v_i, 1.0, 0);
    }

  graph.finalizeReweightedVertices();
}
