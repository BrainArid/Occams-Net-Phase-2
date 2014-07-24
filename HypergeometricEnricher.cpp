#include "HypergeometricEnricher.h"

HypergeometricEnricher::HypergeometricEnricher(const Graph & network, const VertexSet & steiner)
  :network(network),steiner(steiner),N(network.vertCount()),n(steiner.inGraphSize())
{
  //this->N = network.vert_count();
  //this->n = steiner.inGraphSize();
}


float HypergeometricEnricher::enrich(const VertexSet & ontology)
{
  this->K = ontology.inGraphSize();
  this->k = ontology.intersectionInGraphCount(steiner);

  //cout << "PossibleSuccesses: " << K << ", sampleSize: " << n << ", PopulationSize: " << N << ", #SampledSuccesses: " << k << endl;
  boost::math::hypergeometric_distribution<> dist(K, n, N);

  return cdf(complement(dist, k)) + pdf(dist, k); // cdf gives the probability of seeing strictly less than k successes so we add the pdf at k to get P(X>=k);
}
