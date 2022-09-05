//Compile with g++ main.cpp -fopenmp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <omp.h>

int main(){
  std::cout << "Enter the Size of the Array (N): ";
  int N;
  std::cin >> N;

  std::vector<int> array(N);

  //WARNING: This is a poor way of choosing a seed
  srand(time(0));

  std::cout << "Populating Array...\n";
  for(int i =0; i < N; i++)
    array[i] =  (rand() % 1000) + 1; //WARNING: This is a poor way to choose random numbers

  int largest_number = 0;
  int sum = 0;

  // Finding the largest value and calculating sum of the array
  #pragma omp parallel for reduction(+:sum) reduction(max:largest_number)
  for( int j  = 0; j < N; j++){
    sum += array[j];

    if(array[j] > largest_number)
      largest_number = array[j];
  }

  std::cout << "Output: \n";
  std::cout << "Maximum:  " <<  largest_number << ";" <<  "Sum: " <<  sum;
  std::cout << "\n";
}