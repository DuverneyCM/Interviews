//https://github.com/crashoz/uuid_v4
//g++ -Wall -Wextra -pthread -o main -I../src/ ../src/uuid4.c main_optimized.cpp -fopenmp

//OPTIMIZATIONS
// 1. Replace char arrays by structures of char arrays, solving data size and id issues
// 2. Replace for loop in process_orders() by for_each loop with parallel options

#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
//#include <typeinfo>
#include <vector>
#include <algorithm>
#include <omp.h>
//#include <execution>

//using namespace std;
using namespace std::chrono;

#include "uuid4.h" //https://github.com/rxi/uuid4

class OrdersManager{
  int local_quantity;
  char ** orders;
  struct UUID { 
    char id[UUID4_LEN];
    int no_order;
  };
  std::vector<UUID> v_orders;
  int orders_processed = 0;
  time_t last_printed_log = std::time(NULL);
  //auto last_printed_log = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::ostringstream oss;

  void generate_fake_orders(int quantity){
    char id[UUID4_LEN];
    UUID vid;
    log("Generating fake orders");
    //std::cout << typeid(orders).name() << std::endl;
    for (int i=0; i<quantity; i++) {
      uuid4_generate(id);
      std::copy(id, id + UUID4_LEN, vid.id);
      vid.no_order = i;
      v_orders.push_back(vid);
    }

    //std::cout << "Output of begin and end: " << std::endl;
    //for (auto i : v_orders )
    //  std::cout << i.id << std::endl;
    int num_orders = v_orders.size();
    oss << num_orders << " generated...";
    log(oss.str());
  }

  void log(std::string message){
    auto timenow = system_clock::to_time_t(system_clock::now());
    std::cout << ctime(&timenow) << " > " << message << std::endl;
  }

  void fake_save_on_db(char* id, int no_order){
    oss.str("");
    oss.clear();
    oss << "Order [" << id << "] " << no_order << " was successfully prosecuted.";
    log(oss.str());
    int random_ms = round(1000.0*std::rand()/RAND_MAX);
    //std::cout << random_ms << std::endl;
    std::this_thread::sleep_for(milliseconds(random_ms));
  }

  public:
    OrdersManager(int quantity = 10) : orders(new char*[quantity] ) {
      local_quantity = quantity;
      generate_fake_orders(quantity);
    }

    void process_orders(){
      int num_orders = v_orders.size();
      //#pragma omp parallel
      //#pragma omp for
      #pragma omp parallel for
      for (int i=0; i<num_orders; i++) {
      //for (auto order : v_orders ) {
      //std::for( std::execution::par, v_orders.begin(), v_orders.end(), [](auto&& order) ) {
      //std::for_each(auto order : v_orders ) {
        auto order = v_orders[i];
        fake_save_on_db(order.id, order.no_order);
        orders_processed++;

        auto timenow = system_clock::to_time_t(system_clock::now());
        if (timenow > last_printed_log) {

          last_printed_log = timenow + 5; //timedelta
          oss.str("");
          oss.clear();
          oss << "Total orders executed: " << orders_processed << "/" << num_orders;
          log(oss.str());
        }
      }
      
      
    }
};


//int main(int argc, char* argv[]) {
int main() {
  //export OMP_NUM_THREADS=5
  const int len = 10;
  //char buf[UUID4_LEN];
  //char * array[len];
  uuid4_init();
  
  //omp_set_num_threads(4);
  OrdersManager om(len);
  
  auto start_time = high_resolution_clock::now();
  om.process_orders();
  auto end_time = high_resolution_clock::now();
  auto delay = duration_cast<microseconds>(end_time - start_time);
  auto timenow = system_clock::to_time_t(system_clock::now());
  std::cout << ctime(&timenow) << " Tiempo de ejecución: " << (double)delay.count()/1'000'000 << " segundos..." << std::endl;

  /*
  for (int i=0; i<len; i++) {
    credit
    array[i] = buf;
    printf("%s\n", array[i]);
  }*/
  //uuid4_generate(buf);
  //printf("%s\n", buf);
  return(0);
}