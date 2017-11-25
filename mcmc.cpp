// normal_distribution
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <functional>
#include <string>
#include <algorithm>
#include <fstream>
#include <thread>

#include "../matplotlib-cpp/matplotlibcpp.h"
namespace plt = matplotlibcpp;

const double pi = 3.1415926535897;


void printVector(std::vector<double> vec)
{
  for(std::vector<double>::iterator i = vec.begin(); i != vec.end(); i++)
    std::cout << *i << std::endl;
}


double gaussian(double mu, double sig, double x)
{
  return (1./sqrt(2.*pi*pow(sig,2.))) * exp(-pow((x-mu), 2.)/ (2. * pow(sig, 2.)));
}


std::vector<double> pdfList(double mu, double sig, std::vector<double> vec)
{
  std::vector<double> results;
  std::for_each (vec.begin(), vec.end(), [&](double d)
  {
    results.push_back(gaussian(mu, sig, d));
  });

  return results;
}


double prod(std::vector<double> vec)
{
  return std::accumulate(vec.begin(), vec.end(), 1., std::multiplies<double>());
}


// Fix for sigma
// double fracBetween(std::vector<double> stats)
// {
//   for(int i = 0; i < stats.size(); i++)
//   {
//     int count;
//     double number2 = stats[i];
//     if((number2 > -1) && (number2 < 1))
//       count ++;
//   }
//   return ((double) count)/ ((double) stats.size());
// }


void saveData(std::string filename, std::vector<double> & data)
{
  std::ofstream output_file(filename);
  std::ostream_iterator<double> output_iterator(output_file, "\n");
  std::copy(data.begin(), data.end(), output_iterator);
}


std::vector<double> sampler(std::vector<double> dats, int samples, double mu_init, double proposal_width, double mu_prior_mu, double mu_prior_sd)
{
  double mu_current = mu_init;
  double mu_proposal, likelihood_current, likelihood_proposal;
  double prior_current, prior_proposal, p_current, p_proposal, p_accept, accept;
  std::vector<double> posterior = {mu_current};
  std::default_random_engine generator;


  for(int i = 0; i < samples; i++)
  {
    // Suggest new position
    std::normal_distribution<double> distribution(mu_current, proposal_width);
    mu_proposal = distribution(generator);

    // Compute likelihood by multiplying probabilities of each data point
    likelihood_current = prod(pdfList(mu_current, 1., dats));
    likelihood_proposal = prod(pdfList(mu_proposal, 1., dats));

    // Compute prior probability of current and proposed mu
    prior_current = gaussian(mu_prior_mu, mu_prior_sd, mu_current);
    prior_proposal = gaussian(mu_prior_mu, mu_prior_sd, mu_proposal);

    p_current = likelihood_current * prior_current;
    p_proposal = likelihood_proposal * prior_proposal;

    // Accept proposal?
    p_accept = p_proposal / p_current;

    accept = rand() < p_accept;
    if(accept) mu_current = mu_proposal;

    posterior.push_back(mu_current);
  }

  return posterior;
}


void plotCurves(std::vector<double> equity, std::string filename)
{
// Prepare data.
int n = equity.size();
std::vector<double> x(n);
for(int i=0; i<n; ++i)
  x.at(i) = i;

// Plot line from given x and y data. Color is selected automatically.
plt::plot(x, equity, "bo");

// Set x-axis to interval [0,1000000]
plt::xlim(0, n);

// Add graph title
plt::title("Equity Curve");
// Enable legend.
plt::legend();
// save figure
plt::save(filename);
// plt::show();
}


int main()
{
  srand (time(NULL));

  std::default_random_engine gen1;
  std::normal_distribution<double> distribution(0.0, 1.0);
  std::vector<double> data;

  for (int i=0; i<100; i++) {
    data.push_back(distribution(gen1));
  }

  std::vector<double> post = sampler(data, 1000000, -1., .5, 0, 1);

  saveData("Data/result.txt", post);

  return 0;
}
