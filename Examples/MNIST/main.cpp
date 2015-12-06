#include <string>
#include <stdlib.h>
#include "idx.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

char train_labels_file_name[] = "StandardMNIST/train-labels-idx1-ubyte";
char train_images_file_name[] = "StandardMNIST/train-images-idx3-ubyte";
char test_labels_file_name[] = "StandardMNIST/t10k-labels-idx1-ubyte";
char test_images_file_name[] = "StandardMNIST/t10k-images-idx3-ubyte";
IDX1_DATA idxdata;
IDX3_DATA idxdata3;
IDX1_DATA  idxdata_test;
IDX3_DATA idxdata3_test;

void loadMNIST(){
  idx idx_o;
  idx_o.fread_idx1_file (train_labels_file_name, &idxdata);
  idx_o.fread_idx3_file (train_images_file_name, &idxdata3);
  idx_o.fread_idx1_file (test_labels_file_name, &idxdata_test);
  idx_o.fread_idx3_file (test_images_file_name, &idxdata3_test);
}

int main(){
  loadMNIST();
  unsigned char pxl;
  ofstream mnist_file("../../Data/mnist_train.art");
  ofstream mnist_file_test("../../Data/mnist_test.art");

   for (int i = 0; i < idxdata3.nimages; i++)
        {
          int ibase = i * idxdata3.nrows * idxdata3.ncols;
          int label = idxdata.data[i];
          mnist_file << label ;
        int tmp=0;
          for (int y = 0; y < idxdata3.nrows; y++)
            {
              int rbase = ibase + y * idxdata3.ncols;
              for (int x = 0; x < idxdata3.ncols; x++)
                {
                  pxl = idxdata3.data[rbase + x];
                  if (pxl !=0){
                    int b = pxl;
                    mnist_file<< " " << y*28 +  x << ":"<< patch::to_string(b);
                    //tmp++;
                  }
                }
            }
            cout <<"Writing Train Sample #"<<i<<"\r" << std::flush;
            mnist_file << "\n";

    
        }

mnist_file.close();


  cout << endl;
   for (int i = 0; i < idxdata3_test.nimages; i++)
        {
          int ibase = i * idxdata3_test.nrows * idxdata3_test.ncols;
          int label = idxdata_test.data[i];
          mnist_file_test << label ;
        int tmp=0;
          for (int y = 0; y < idxdata3_test.nrows; y++)
            {
              int rbase = ibase + y * idxdata3_test.ncols;
              for (int x = 0; x < idxdata3_test.ncols; x++)
                {
                  pxl = idxdata3_test.data[rbase + x];
                  if (pxl !=0){
                     int b = pxl;
                    mnist_file_test<< " " << y*28 +  x << ":"<< patch::to_string(b);
                    //tmp++;
                  }
                }
            }
            cout <<"Writing Test Sample #"<<i<<"\r" << std::flush;
            mnist_file_test << "\n";

    
        }
mnist_file_test.close();

  return 0;
}