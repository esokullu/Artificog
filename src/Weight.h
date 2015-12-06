/* 
 * Artificog - Artificial Intelligence Framework
 * Author: amin@artificog.com
 */
 
#include <cmath>
#include "Layer.h"
#ifndef WEIGHT_H
#define WEIGHT_H
using namespace std;
using namespace arma;

class Weight
{
public:
    Weight(int,int);
    mat value;
     int width;
    int height;
private:
   
};
Weight::Weight(int height,int width)
{
    this->width = width;
    this->height =height;
   
    

    int fan_in = height;
    int fan_out = width;
    double min = -sqrt(6 / (double) (fan_in + fan_out));
    double max = sqrt(6 / (double) (fan_in + fan_out));
    this->value =  (max - min) *  randu<mat>(height,width)  - max;
}


#endif /* WEIGHT_H */
