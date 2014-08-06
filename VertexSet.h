#ifndef VERTEXSET_H
#define VERTEXSET_H

#include <string>
#include <vector>
#include <utility>
//#include <tuple>
#include <algorithm>

#include "Graph.h"

using namespace std;
using namespace boost;

class VertexSet
{
public:
//static Graph::Vertex nullVert; //if a vertex doesn't exist within graph

private:
  string name;
  vector<Graph::Vertex> setInGraph;
  vector<string> setNotInGraph;
  Graph & graph; //almost const, but manipulation of Vertex.visited prevents this

  void buildSet (const vector<string> & vertNames);

public:


  VertexSet(const string & name, Graph & graph);
  VertexSet(const string & name, const vector<string> & vertNames, Graph & graph);
  VertexSet(const string & name, const vector<Graph::Vertex> & setInGraph, const vector<string> & setNotInGraph, Graph & graph);
  int intersectionCount(const VertexSet & other) const;
  int intersectionInGraphCount(const VertexSet & other) const; //must share same graph..check this
  pair<vector<Graph::Vertex>::const_iterator, vector<Graph::Vertex>::const_iterator > verticesInGraph() const;
  pair<vector<string>::const_iterator, vector<string>::const_iterator > verticesNotInGraph() const;
  const Graph & getGraph() const;
  void unionAdd(const string & name);
  void addInGraph(const Graph::Vertex & v);
  string getName() const;
  void printInVertices(std::ostream& os) const;
  void printNotInVertices(std::ostream& os) const;
  unsigned int inGraphSize() const;
  void getHalo(VertexSet & halo) const; //halo is expected to be an empty VertexSet sharing the same graph with this
  void fadeVertexWeights();
  VertexSet& operator=(const VertexSet& other);
};
 

std::ostream& operator<<(std::ostream& os, const VertexSet& obj);

#endif
