/* 
 * Artificog - Artificial Intelligence Framework
 * Author: amin@artificog.com
 */

#include <vector>
#include <armadillo>
#include "json.hpp"

#ifndef LAYER_H
#define	LAYER_H


 using namespace std;
 using namespace arma;
 using json = nlohmann::json;

 class Layer{
 public:
 	enum LayerType { Input=0,FC,ConvNet, Pooling } ; 
 	enum FunctionType {sigmoid =0, tan_h , relu , softmax };
 	LayerType layerType;
 	FunctionType functionType;
 	Layer(){};
 	Layer(string layer_params);
 	int neurons_count;
 	mat value;
 	mat bias;

 private:
 	

 };
 Layer::Layer(string layer_params){

 	auto layer_params_json = json::parse(layer_params);

 	string layer_type_str= layer_params_json["Type"].get<std::string>();
 	string activation_type_str= layer_params_json["Function"].get<std::string>();
 	this->neurons_count = layer_params_json["Neurons_Count"].get<int>();

 	//cout <<"Layer Type = " << layer_type_str << ", Func = " << activation_type_str << ", layer_neurons_count_str " << this->neurons_count<< endl;
 			if (layer_type_str== "Input" ) 
 				this->layerType = LayerType::Input; 	
 			 if (layer_type_str== "FC"  ){ 	
 			 	this->layerType = LayerType::FC;
 			 }
 			if (layer_type_str== "ConvNet")
 			    this->layerType = LayerType::ConvNet;   
 			if (layer_type_str==  "Pooling" )
 				this->layerType = LayerType::Pooling;      
 			/*if (layer_type_str==  "NonLinearity")
 				 this->layerType = LayerType::NonLinearity; */
 			
 	
 	
 	//if (this->layerType == LayerType::NonLinearity){

 			if (activation_type_str =="sigmoid" )
 				this->functionType  = FunctionType::sigmoid;   
 			if (activation_type_str == "tanh")
 			 	this->functionType  = FunctionType::tan_h;   
 			if (activation_type_str == "relu" 	)
 			   this->functionType  = FunctionType::relu;    
 			if (activation_type_str == "softmax")
 			   this->functionType = FunctionType::softmax;  
 		
 	//}
 
 	// 1 . Allocate memory for neuron values on GPU except for the input layer and NonLinearity , ... ?
 	//if (this->layerType != LayerType::Pooling &&  this->layerType != LayerType::Input){
 		this->bias.zeros(this->neurons_count,1);
 		this->value.zeros(this->neurons_count,1);

 	
 }
#endif	/* LAYER_H */

