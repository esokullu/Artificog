/* 
 * Artificog - Artificial Intelligence Framework
 * Author: amin@artificog.com
 */
#include <vector>
#include "Layer.h"
#include "Weight.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "json.hpp"
#include <cstdlib>
#include <armadillo>
#include "ArtificogHelper.h"
#ifndef NETWORK_H
#define NETWORK_H

 using namespace std;
 using namespace arma;
 using json = nlohmann::json;

 class Network
 {
 private:
 public:
 	double learning_rate;
 	std::vector<Layer> layers;
 	std::vector<Weight> weights;
 	void updateLayerParams(int, const mat,const mat);

 	string train_data_address;
 	string test_data_address;
 	std::vector<int> reference_labels;

 	int getData(string ,mat &,int &);
 	void reset();

	void feedForward( mat  ,  int ); // or predict for data (single sample mini-batch learning)
	void learn();

	Network(){};
	Network(string);
	mat output_layer;

	int current_target;
	int current_prediction;
	int missed_sample_count;
 	int trained_sample_count;

 	int epoch_count=0;
 	void run();

 	
};


Network::Network(string file_address)
{
	this->missed_sample_count=0;

	ifstream inFile;
	inFile.open(file_address);//open the input file

	stringstream strStream;
	strStream << inFile.rdbuf();//read the file
	string network_json_str = strStream.str();

	this->trained_sample_count = 0;

	// TODO : Structure validity checker ! if params are not valid exit with error. (consistency)

	auto network_json = json::parse(network_json_str);
	this->train_data_address = network_json["Network"]["Train_Data"].get<std::string>();
	this->test_data_address = network_json["Network"]["Test_Data"].get<std::string>();

    this->learning_rate= network_json["Network"]["Learning_Rate"]; // as a double

    this->epoch_count = network_json["Network"]["Epochs"]; 

     // Layers Initialization
    auto layers_json = network_json["Network"]["Layers"];


    for (int i=0;i<layers_json.size();i++){
    	 Layer current_layer(layers_json.at(i).dump());
    	this->layers.push_back(current_layer);
    }

    auto labels_json = network_json["Network"]["Labels"];
    for (int i=0;i<labels_json.size();i++){
    	int current_label = labels_json.at(i);
    	this->reference_labels.push_back(current_label);
    }

    // Weights Initialization
    for (int i=0;i<this->layers.size()-1;i++) {// Count of weights matrices is one less than the count of layers
    	int height =this->layers.at(i+1).neurons_count;
    	int width =this->layers.at(i).neurons_count;
    	Weight current_weight(height,width);
    	this->weights.push_back(current_weight);
    }
}

void Network::reset(){
	this->trained_sample_count = 0 ;
	this->missed_sample_count = 0;
}
mat normalize(mat A){
  float mean = accu(A)/A.n_rows;
  mat X = A - mean;
  mat tmp =stddev(X); 
  return X/tmp(0,0);
}

mat softmax(mat X){
	double den=0;
	for (int i=0;i<X.n_rows;i++)
		den+=exp(X(i,0));
	return exp(X)/den;

}


void Network::feedForward(mat data ,  int label){
    this->current_target = label;
    this->layers.at(0).value = normalize(data);
	for (int i=0;i<this->layers.size() - 1 ;i++){ 
		     
		     switch (this->layers.at(i+1).layerType){
		     	case Layer::LayerType::Input : 
		     	case Layer::LayerType::FC :{

		     		this->layers.at(i+1).value = this->weights.at(i).value * this->layers.at(i).value + this->layers.at(i+1).bias ;
		     		
		     		switch(this->layers.at(i+1).functionType){
		     			case Layer::FunctionType::tan_h:
		     				this->layers.at(i+1).value=   tanh( this->layers.at(i+1).value);
		     			break;
		     			case Layer::FunctionType::softmax:
		     				this->layers.at(i+1).value= softmax(this->layers.at(i+1).value);

		     			break;
		     			case Layer::FunctionType::sigmoid:

		     			break;
		     			case Layer::FunctionType::relu:
		     			break;

		     		}

		     		break;
		     		}
		     		case Layer::LayerType::ConvNet : break;
		     		case Layer::LayerType::Pooling : break;

		     }
	}

	this->output_layer = this->layers.at(this->layers.size() - 1).value;
	
    uvec sorted_indices = sort_index(this->output_layer , "descend");
    this->current_prediction = sorted_indices(0);

    if ( this->current_prediction != this->current_target)
    	this->missed_sample_count++;

}

void Network::learn()
{
	this->trained_sample_count++;
	mat y = this->layers.at(this->layers.size() - 1).value;
	mat y_d;
    y_d.zeros(y.n_rows,y.n_cols);
    y_d(this->current_target, 0)=1;
	mat A,B,C,delta_weight;

	// Gradient Descent
	switch(this->layers.at(this->layers.size() - 1).functionType){
		case Layer::FunctionType::softmax:
			 A = y;
			 A(this->current_target,0)=y(this->current_target,0) -1;
		break;
		case Layer::FunctionType::tan_h:
			 A = 2 * (1 - y % y) % (y - y_d);
		break;
		case Layer::FunctionType::sigmoid:break;
		case Layer::FunctionType::relu:break;
	}

    for (unsigned int layerNum = this->layers.size() - 1; layerNum >= 1; layerNum--)
    {
        delta_weight = A * this->layers.at(layerNum - 1).value.t();
        this->updateLayerParams(layerNum,delta_weight,A);
		B =  this->weights.at(layerNum - 1).value.t() * A;

        switch(this->layers.at(layerNum - 1).functionType){
			case Layer::FunctionType::softmax:
			 	
			break;
			case Layer::FunctionType::tan_h:
				C = 1 -  this->layers.at(layerNum - 1).value %  this->layers.at(layerNum - 1).value;
			break;
			case Layer::FunctionType::sigmoid:break;
			case Layer::FunctionType::relu:break;
		}
		A= B % C;

    }

}

void Network::updateLayerParams(int layerNum,const mat delta_weight,const mat delta_bias){
  this->weights.at(layerNum - 1).value = this->weights.at(layerNum - 1).value -
                this->learning_rate * delta_weight;

  this->layers.at(layerNum).bias = this->layers.at(layerNum).bias -
                this->learning_rate * delta_bias;

}


int Network::getData(string line,mat &data,int &label){
    int found = 0;
    data.zeros(784,1);
	istringstream iss(line);
    	vector<string> splitted_line = split (line, ' ');
    	label = std::atoi(splitted_line.at(0).c_str()); 
    	
    	for (int i=0;i<this->reference_labels.size();i++){
    		if (this->reference_labels.at(i) == label){
    			found = 1;
    			break;
    		}
    	}
    	if (!found)
    		return 0;

    	for (int i=1;i<splitted_line.size();i++){
    		vector<string> splitted_pair = split (splitted_line.at(i), ':');
    		data(std::atoi(splitted_pair.at(0).c_str()),0)=std::atoi(splitted_pair.at(1).c_str());
    	}
    	return 1;
   
}

void Network::run(){
	int label;
	mat data;
	ifstream train_file(this->train_data_address);
	ifstream test_file(this->test_data_address);


	std::string line;
    for (int epoch=1; epoch <= this->epoch_count;epoch++){
		while (getline(train_file, line)){
			if (!getData(line,data,label))
				continue;
    		feedForward(data,label);
    		learn();
    		printf("Epoch %d, Sample #%4d - Error rate = %5.3f\r",epoch,this->trained_sample_count,100*this->missed_sample_count/(double)this->trained_sample_count);
    		cout << std::flush;
		}
		train_file.clear();
		train_file.seekg(0, ios::beg);
		cout << endl;

		reset();

		int counter =1;
		while (getline(test_file, line)){
			if (!getData(line,data,label))
				continue;
    		feedForward(data,label);
    		printf("Test Set, Sample #%4d - Missed = %d \r",counter++,this->missed_sample_count);
    		cout << std::flush;
		}
		test_file.clear();
		test_file.seekg(0, ios::beg);
		reset();
		cout << endl;

		this->learning_rate = this->learning_rate / 1.1;

	}
	cout << endl;
}


#endif /* NETWORK_H */















