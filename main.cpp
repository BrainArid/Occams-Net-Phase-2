#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "subgraph.hpp"
#include "Graph.h"
#include "VertexSet.h"

using namespace std;
using namespace boost;

//Arguments
string AddrOnt;
string AddrNet;
string AddrStein;
string oDir;

void printHelpMessage()
{
  cout << "Printing Help:" << endl << endl;
  cout << "FLAG\t\tDefault Value\tDescription" << endl;
  cout << "--num-reads1, -n1\t30000000\tSize of array used to store reads from first .bed file." << endl;
  cout << "--num-reads2, -n2\t30000000\tSize of array used to store reads from second .bed file." << endl;
  cout << "--largest-Chrom, -c\t250000000\tOverestimate of largest chromosome in bases." << endl;
  cout << "--bin-log, -l\t2\tThe base of the log scaling used to bin distance histogram output." << endl;
  cout << "--out-dir, -o\trequired\tAddress of directory to place output." << endl;
  cout << "--in-bed1,i1\trequired\tAddress of first name-sorted and multimapped-filtered .bed file." << endl;
  cout << "--in-bed2,i2\trequired\tAddress of second name-sorted and multimapped-filtered .bed file." << endl;
}

void extractArgs(int argc, char *argv[])
{
  bool printHelp = argc <= 1;

  cout << "argc = " << argc << endl;
  for(int i = 0; i < argc; i++)
    {
      cout << "argv[" << i << "] = " << argv[i] << endl;

      if(strcmp(argv[i], "--in-ppi")==0 || strcmp(argv[i], "-in") == 0)
        {
          if(++i >= argc)
            {
              cout << "Expected ppi file address after flag" << endl;
              printHelp = true;
            }
          else
            AddrNet = argv[i];
        }
      else if(strcmp(argv[i], "--in-steiner")==0 || strcmp(argv[i], "-is") == 0)
        {
          if(++i >= argc)
            {
              cout << "Expected steiner file address after flag" << endl;
              printHelp = true;
            }
          else
            AddrStein = argv[i];
        }
      else if(strcmp(argv[i], "--in-ont")==0 || strcmp(argv[i], "-io") == 0)
        {
          if(++i >= argc)
            {
              cout << "Expected ontology file address after flag" << endl;
              printHelp = true;
            }
          else
            AddrOnt = argv[i];
        }
    }

  if(printHelp)
    printHelpMessage();
}

void openFStream(ifstream & inStream, const string & addr)
{
  inStream.open(addr, std::ifstream::in);
  if(!inStream.is_open())
    {
      cout << "Could not open " << addr << " for reading." << endl;
      exit(-1);
    }
}

void constructIntrNet(Graph & intrNet, ifstream & ifsNet)
{
  //vector<Graph::BaseEdge> edges
  
  char cline[1024];
  string nodeName1, nodeName2;
 
  while(ifsNet.good())
    {
      ifsNet.getline(cline, 1024);
      string line(cline);
      stringstream ss(line);
      ss >> nodeName1;
      ss >> nodeName2; //skip 'pp'
      ss >> nodeName2;
      
      //create verticies 
      Graph::VertexProp prop1;
      prop1.name = nodeName1;
      Graph::VertexProp prop2;
      prop2.name = nodeName2;

      intrNet.addEdgeByNamedVertices(prop1, prop2);
    }
  
  cout << intrNet << endl;
}

void constructSteinerSet(VertexSet & steinerSet, ifstream & ifsStein)
{
  //vector<Graph::BaseEdge> edges
  
  char cline[1024];
  string nodeName1, nodeName2;
 
  while(ifsStein.good())
    {
      ifsStein.getline(cline, 1024);
      string line(cline);
      stringstream ss(line);
      ss >> nodeName1;
      ss >> nodeName2; //skip 'pp'
      ss >> nodeName2;
      
      //create verticies 
      Graph::VertexProp prop1;
      prop1.name = nodeName1;
      Graph::VertexProp prop2;
      prop2.name = nodeName2;

      steinerSet.unionAdd(nodeName1);
      steinerSet.unionAdd(nodeName2);
    }
  
  cout << steinerSet << endl;
}

void constructOntologySets(vector<VertexSet> & ontSets, ifstream & ifsOnt, Graph & graph)
{
  //vector<Graph::BaseEdge> edges
  
  char cline[1024];
  string name;
 
  cout << "Constructing Ontology..." << endl;
  while(ifsOnt.good())
    {
      ifsOnt.getline(cline, 1024);
      string line(cline);
      stringstream ss(line);
      ss >> name;

      VertexSet set(name, graph);
      
      //ignore source info
      ss >> name;

      //create elements 
      while(ss.good())
	{
	  ss >> name;
	  set.unionAdd(name);
	}
      cout << set << endl;
    }

  cout << "Ontology Construction complete." << endl;
}

int main(int argc, char *argv[])
{

  string name = "InterNet";
  //vector<Graph::BaseEdge> edges;
  Graph IntrNet(name); //Interaction Network
  VertexSet SteinerSet("SteinerSet", IntrNet);
  vector<VertexSet> OntologySets;

  //get parameters
  extractArgs(argc, argv);

  //construct IntrNet
  ifstream ifsNet;
  openFStream(ifsNet, AddrNet);
  constructIntrNet(IntrNet, ifsNet);
  ifsNet.close();

  //construct steinerTree as vertexSet of IntrNet
  ifstream ifsStein;
  openFStream(ifsStein, AddrStein);
  constructSteinerSet(SteinerSet, ifsStein);
  ifsStein.close();

  //construct ontology
  ifstream ifsOnt;
  openFStream(ifsOnt, AddrOnt);
  constructOntologySets(OntologySets, ifsOnt, IntrNet);
  ifsOnt.close();

  //calculate inner score and outer and middle scores

  //output scores per gene set

  return 0;
}
