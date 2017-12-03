#include "mcmc.cpp"

int main()
{
  srand (time(NULL));

  std::default_random_engine gen1;
  std::normal_distribution<double> distribution(0.0, 1.0);
  std::vector<double> data;

  for (int i = 0; i < 20; i++) {
    data.push_back(distribution(gen1));
  }

  std::cout << "The data is: " << std::endl;
  printVector(data);

  std::cout << "-----------------------------------" << std::endl;
  std::vector<double> post = sampler(data, 8000, -1., .5, 0., 1.);

  saveData("Data/result.txt", post);
  plotCurves(post, "post.jpg", "Trace");

  std::cout << "The fraction between the standard deviation limits are: " << fracBetween(data) << std::endl;

  return 0;
}
