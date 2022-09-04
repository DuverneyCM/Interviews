//https://github.com/crashoz/uuid_v4
//g++ -Wall -Wextra -pthread -o main -I../src/ ../src/uuid4.c main.cpp

#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <typeinfo>

//using namespace std;
using namespace std::chrono;

#include "uuid4.h" //https://github.com/rxi/uuid4

class OrdersManager{
  int local_quantity;
  char ** orders;
  int orders_processed = 0;
  time_t last_printed_log = std::time(NULL);
  //auto last_printed_log = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::ostringstream oss;

  void generate_fake_orders(int quantity){
    char id[UUID4_LEN];
    char * orderss[quantity];
    log("Generating fake orders");
    //std::cout << typeid(orders).name() << std::endl;
    for (int i=0; i<quantity; i++) {
      uuid4_generate(id); 
      orders[i] = new char[UUID4_LEN];
      orderss[i] = new char[UUID4_LEN];
      std::copy(id, id + UUID4_LEN, orders[i]);
      //orders[i] = order; //Id was no saved
      //orders[i] = orderss[i];
      //std::cout << orderss[i] << std::endl;
    }
    //for (int i=0; i<quantity; i++) {
    //  std::cout << orders[i] << std::endl;
    //}
    //int num_orders = sizeof(orders)/sizeof(orders[0]);
    int num_orders = sizeof(orderss)/sizeof(orderss[0]);
    //std::cout << sizeof(orderss) << std::endl;
    oss << num_orders << " generated...";
    log(oss.str());
  }

  void log(std::string message){
    auto timenow = system_clock::to_time_t(system_clock::now());
    //auto timenow = system_clock::now();
    std::cout << ctime(&timenow) << " > " << message << std::endl;
    //std::cout << ctime(&timenow) << std::endl;
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
      int num_orders = sizeof(orders)/sizeof(orders[0]); //no work for the global array
      for (int i=0; i<local_quantity; i++) { //num_orders
        //order = orders[i];
        //std::cout << num_orders << std::endl;
        fake_save_on_db(orders[i], i);
        orders_processed++;

        auto timenow = system_clock::to_time_t(system_clock::now());
        if (timenow > last_printed_log) {

          last_printed_log = timenow + 5; //timedelta
          oss.str("");
          oss.clear();
          oss << "Total orders executed: " << orders_processed << "/" << local_quantity;
          log(oss.str());
        }
      }
    }
};


//int main(int argc, char* argv[]) {
int main() {

  const int len = 10;
  //char buf[UUID4_LEN];
  //char * array[len];
  uuid4_init();
  
  OrdersManager om(len);
  auto start_time = high_resolution_clock::now();
  om.process_orders();
  auto end_time = high_resolution_clock::now();
  auto delay = duration_cast<microseconds>(end_time - start_time);
  auto timenow = system_clock::to_time_t(system_clock::now());
  std::cout << ctime(&timenow) << " Tiempo de ejecución: " << (double)delay.count()/1'000'000 << " segundos..." << std::endl;

  /*
  for (int i=0; i<len; i++) {
    uuid4_generate(buf);
    array[i] = buf;
    printf("%s\n", array[i]);
  }*/
  //uuid4_generate(buf);
  //printf("%s\n", buf);
  return(0);
}