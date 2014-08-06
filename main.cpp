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
#include "HypergeometricEnricher.h"

using namespace std;
using namespace boost;

//Arguments
string AddrOnt;
string AddrNet;
string AddrStein;
string oDir;
string AddrScoreOut;
string AddrGeneWeights;
string AddrReweight;
bool reweightFlag = false;

const string AddrScoreOutSuffix = "scores.txt";
const string AddrReweightSuffix = "reweight.txt";

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
  bool oDirProvided = false;
  AddrScoreOut = AddrScoreOutSuffix;
  AddrReweight = AddrReweightSuffix;
  
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
      else if(strcmp(argv[i], "--in-geneWeights")==0 || strcmp(argv[i], "-ig") == 0)
        {
          if(++i >= argc)
            {
              cout << "Expected gene weights file address after flag" << endl;
              printHelp = true;
            }
          else
            AddrGeneWeights = argv[i];
        }
      else if(strcmp(argv[i], "--out-score")==0 || strcmp(argv[i], "-os") == 0)
        {
          if(++i >= argc)
            {
              cout << "Expected score file output address after flag." << endl;
              printHelp = true;
            }
          else
	    AddrScoreOut = argv[i];
        }
      else if(strcmp(argv[i], "--out-reweight")==0 || strcmp(argv[i], "-ow") == 0)
        {
          if(++i >= argc)
            {
              cout << "Expected reweight file output address after flag." << endl;
              printHelp = true;
            }
          else
	    {
	      AddrReweight = argv[i];
	      reweightFlag = true;
	    }
	  
        }
      else if(strcmp(argv[i], "--out-dir")==0 || strcmp(argv[i], "-od")==0 || strcmp(argv[i], "-o")==0)
        {
          if(++i >= argc)
            {
              cout << "Expected output directory address after flag." << endl;
              printHelp = true;
            }
          else
	    {
	      oDir = argv[i];
	      oDirProvided = true;
	      cout << "Output directory set to: " << oDir << endl;
	    }
        }
    }

  //determine output file addresses based on combination of inputs
  if(oDirProvided)
    {
      AddrScoreOut = oDir + "/" + AddrScoreOut;
      AddrReweight = oDir + "/" + AddrReweight;
    }

  if(printHelp)
    printHelpMessage();
}

void openIFStream(ifstream & inStream, const string & addr)
{
  inStream.open(addr, std::ifstream::in);
  if(!inStream.is_open())
    {
      cout << "Could not open " << addr << " for reading." << endl;
      exit(-1);
    }
}

void openOFStream(ofstream & outStream, const string & addr)
{
  outStream.open(addr, std::ifstream::out);
  if(!outStream.is_open())
    {
      cout << "Could not open " << addr << " for writing." << endl;
      exit(-1);
    }
}

//Can handle files with lines of any of the following format:
//A	pp	B	0.5
//B	C
//C	pp	D
//C	pp	E	=	0.5
void constructIntrNet(Graph & intrNet, ifstream & ifs)
{
  char cline[1024];
  string nodeName1, nodeName2;
  float edgeWeight;

  while(ifs.good())
    {
      ifs.getline(cline, 1024);
      string line(cline);
      stringstream ss(line);
      ss >> nodeName1;
      ss >> nodeName2;
      if(nodeName2.compare("pp") == 0 || nodeName2.compare("pd") == 0 ||nodeName2.compare("(pp)") == 0 ||nodeName2.compare("(pd)") == 0)
	ss >> nodeName2;

      bool hasWeight = false;
      if(ss.good())
	{
	  ss.get();
	  if(char(ss.peek()) == '=')
	    {
	      ss.get();
	      ss.get();
	    }
	  ss >> edgeWeight; //first could be an '='
	  
	  hasWeight = true;
	}
      
      //create verticies 
      Graph::VertexProp prop1;
      prop1.name = nodeName1;
      Graph::VertexProp prop2;
      prop2.name = nodeName2;

      Graph::EdgeProp eProp;
      if(hasWeight)
	eProp.weight = edgeWeight;
      else
	eProp.weight = -1;
      
      intrNet.addEdgeByNamedVertices(prop1, prop2, eProp);
    }
  
  //cout << intrNet << endl;
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
  
  //cout << steinerSet << endl;
}

void addGeneWeights(VertexSet & weightedGenes, ifstream & ifs, Graph & graph)
{
  //vector<Graph::BaseEdge> edges
  
  char cline[1024];
  string name, line;
 
  ifs.getline(cline, 1024);
  line = string(cline);

  while(!ifs.eof() && line.compare("")!=0)
    {
      stringstream ss(line);
      ss >> name;
 
      Graph::Vertex v;
      if(graph.findVertexByName(name, v))
	{
	  weightedGenes.addInGraph(v);
	  
	  float weight;
	  ss >> weight;
	  graph.setVertexWeight(v, weight); 
	}
      //can't add into notInGraph set because there's no way to keep track of weight this way.
      
      //get next line
      ifs.getline(cline, 1024);
      line = string(cline);
    }

  //  cout << weightedGenes << endl;
}

void constructOntologySets(vector<VertexSet> & ontSets, ifstream & ifsOnt, Graph & graph)
{
  //vector<Graph::BaseEdge> edges
  
  char cline[1024];
  string name, line;

  ifsOnt.getline(cline, 1024);
  line = string(cline);

  while(!ifsOnt.eof() && line.compare("")!=0)
    {
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
      // cout << set << endl;

      ontSets.push_back(set);

      //get next line
      ifsOnt.getline(cline, 1024);
      line = string(cline);
    }

}

void outputScoreFile(VertexSet & steiner, vector<VertexSet> & ontology, ofstream & ofsScore, Graph & graph)
{
 //calculate inner score and outer and middle scores
  HypergeometricEnricher innerEnricher(graph, steiner);
  VertexSet halo("halo", graph);
  steiner.getHalo(halo);
  HypergeometricEnricher outerEnricher(graph, halo);

  ofsScore << "Ontology\tInner\tOuter\tMiddle" << endl;
  for(vector<VertexSet>::const_iterator it = ontology.begin(); it != ontology.end(); it++)
    {
      ofsScore << it->getName() << "\t" << innerEnricher.enrich(*it) << "\t" << outerEnricher.enrich(*it) << endl;
    }
}

void outputNodeWeightFile(Graph & graph, VertexSet & nodes, ofstream & ofs)
{
 //calculate inner score and outer and middle scores

  //ofs << "Gene\tWeight" << endl;
  vector<Graph::Vertex>::const_iterator it, e_it;
  for(tie(it, e_it)  = nodes.verticesInGraph(); it != e_it; it++)
    {
      ofs << graph.getVertexName(*it) << "\t" << graph.getVertexWeight(*it) << endl;
    }
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
  cout << "Main: Constructing IntrNet..." << endl;
  ifstream ifsNet;
  openIFStream(ifsNet, AddrNet);
  constructIntrNet(IntrNet, ifsNet);
  ifsNet.close();
  cout << "Main: Constructing IntrNet Complete." << endl;

  //construct steinerTree as vertexSet of IntrNet
  cout << "Main: Constructing Steiner Network..." << endl;
  ifstream ifsStein;
  openIFStream(ifsStein, AddrStein);
  constructSteinerSet(SteinerSet, ifsStein);
  ifsStein.close();
  cout << "Main: Constructing Steiner Network Complete." << endl;

  //construct ontology
  cout << "Main: Constructing Ontology..." << endl;
  ifstream ifsOnt;
  openIFStream(ifsOnt, AddrOnt);
  constructOntologySets(OntologySets, ifsOnt, IntrNet);
  ifsOnt.close();
  cout << "Main: Constructing Ontology Complete." << endl;

  //output scores per gene set
  cout << "Main: Outputing Score File..." << endl;
  ofstream ofsScore;
  openOFStream(ofsScore, AddrScoreOut);
  outputScoreFile(SteinerSet, OntologySets, ofsScore, IntrNet);
  ofsScore.close();
  cout << "Main: Outputing Score File Complete." << endl;

  //if reweighting...
  if(reweightFlag)
    {
      //add weights to genes
      cout << "Main: Adding Vertex Weights..." << endl;
      VertexSet weightedGenes("WeightedGenes", IntrNet);
      
      ifstream ifsGeneWeights;
      openIFStream(ifsGeneWeights, AddrGeneWeights);
      addGeneWeights(weightedGenes, ifsGeneWeights, IntrNet);
      ifsGeneWeights.close();
      cout << "Main: Adding Vertex Weights Complete." << endl;
      
      cout << "Main: Fading Vertex Weights..." << endl;
      SteinerSet.fadeVertexWeights();
      cout << "Main: Fading Vertex Weights Complete." << endl;
      
      //output reweights
      cout << "Main: Outputing Reweighted Vertex Weight File." << endl;
      ofstream ofsReweight;
      openOFStream(ofsReweight, AddrReweight);
      outputNodeWeightFile(IntrNet, weightedGenes, ofsReweight);
      ofsReweight.close();
      cout << "Main: Outputing Reweighted Vertex Weight File Complete." << endl;
    }
  
  cout << "Program complete!" << endl;
  return 0;
}
