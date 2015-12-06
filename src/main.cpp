
//#include "ArtificogHelper.h"
#include "Network.h"
#include <fstream>
#include <armadillo>
#include <iomanip>
using namespace std;

int main(int argc, char **argv){

  string json_addr ="../Network.json";	
  if (argc ==2)
  	json_addr = argv[1];
  Network network(json_addr);
  network.run();

  return 0;
}