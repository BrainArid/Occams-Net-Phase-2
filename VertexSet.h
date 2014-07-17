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
  const Graph & graph;

  void buildSet (const vector<string> & vertNames);

public:


  VertexSet(const string & name, const Graph & graph);
  VertexSet(const string & name, const vector<string> & vertNames, const Graph & graph);
  int intersectionCount(const VertexSet & other) const;
  int intersectionInGraphCount(const VertexSet & other) const; //must share same graph..check this
  pair<vector<Graph::Vertex>::const_iterator, vector<Graph::Vertex>::const_iterator > verticesInGraph() const;
  pair<vector<string>::const_iterator, vector<string>::const_iterator > verticesNotInGraph() const;
  const Graph & getGraph() const;
  void unionAdd(const string & name);
  string getName() const;
  void printInVertices(std::ostream& os) const;
  void printNotInVertices(std::ostream& os) const;
};
 

std::ostream& operator<<(std::ostream& os, const VertexSet& obj);

#endif