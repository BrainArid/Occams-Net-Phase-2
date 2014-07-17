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

VertexSet::VertexSet(const string & name, const vector<string> & vertNames, const Graph & graph)
  : graph(graph)
{
  this->name = name;
  this->buildSet(vertNames);
}

VertexSet::VertexSet(const string & name, const Graph & graph)
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
  cout << "TEST:" << &this->graph << " ==? " << &other.getGraph() << endl;
  if(!((&this->graph)==(&other.getGraph())))
    num = -1;
  else
    {
      vector<Graph::Vertex>::const_iterator vi, ve;
      for(tie(vi, ve) = verticesInGraph(); vi != ve; vi++)
	{
	  bool found = false;
	  vector<Graph::Vertex>::const_iterator ovi, ove;
	  for(tie(ovi, ove) = verticesInGraph(); ovi != ove && !found; ovi++)
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
	      os << "[" << i++ << "] " << graph.getVertexName(*it) << ", ";
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
