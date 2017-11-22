// normal_distribution
#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <algorithm>

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


// void sampler(std::vector<double> dats, int samples, double mu_init)
// {
//   double mu_current = mu_init, likelihood_current, likelihood_proposal;
//   std::vector<double> posterior = {mu_current};
//
//   for(int i = 0; i < samples; i++)
//   {
//     // Suggest new position - norm().rvs()
//     mu_proposal = 1.;
//
//     // Compute likelihood by multiplying probabilities of each data point
//     // norm().pdf().prod()
//     likelihood_current = prod(pdfList(mu_current, 1., dats));
//     likelihood_proposal = prod(pdfList(mu_proposal, 1., dats));
//
//     // Compute prior probability of current and proposed mu
//     // norm().pdf()
//     prior_current = gaussian(mu_prior_mu, mu_prior_sd, mu_current);
//     prior_proposal = gaussian(mu_prior_mu, mu_prior_sd, mu_proposal);
//
//     p_current = likelihood_current * prior_current;
//     p_proposal = likelihood_proposal * prior_proposal;
//
//     // Accept proposal?
//     p_accept = p_proposal / p_current;
//
//     accept = rand() < p_accept;
//     if(accept) mu_current = mu_proposal;
//
//     posterior.push_back(mu_current);
//   }
// }


int main()
{
  srand (time(NULL));

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(5.0,2.0);

  for (int i=0; i<15; ++i) {
    std::cout << generator << std::endl;
    double number = distribution(generator);
    std::cout << number << std::endl;
  }

  std::vector<double> a = {-0.2, -0.1, 0.0, 0.1, 0.2};

  printVector(a);

  std::cout << std::accumulate(a.begin(), a.end(), 1., std::multiplies<double>()) << std::endl;

  std::for_each (a.begin(), a.end(), [&](double d)
  {
    std::cout << gaussian(0.0, 0.5, d) << std::endl;
  });

  return 0;
}
