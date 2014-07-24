#ifndef HYPERGEOMETRICENRICHER_H
#define HYPERGEOMETRICENRICHER_H

#include <string>
#include <vector>
#include <utility>
//#include <tuple>
#include <algorithm>

#include "Graph.h"
#include "VertexSet.h"

#include <boost/math/distributions/hypergeometric.hpp>

using namespace std;
using namespace boost;

class HypergeometricEnricher
{
private:
const Graph & network;
const VertexSet & steiner;

const unsigned int N; //population size
unsigned int K; //possible successes
const unsigned int n; //sample size (steiner size)
unsigned int k; //number successes in sample
public:
  HypergeometricEnricher(const Graph & network, const VertexSet & steiner);
float enrich(const VertexSet & ontology);
};

#endif
