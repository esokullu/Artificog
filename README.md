# Artificog Machine Learning Framework
[Artificog](http://artificog.com) is a simple Machine Learning Framework consists of implementation of various algorithms including **Deep Learning** methods.


## Building 
In order to build the code you can use `cmake` ,
```shell
$ mkdir build 
$ cd build 
$ cmake ..
$ make
```

## Usage
### JSON File
First you should have a JSON file which stores the topology and parameters of the network.
```json
{
  "Network": {
    "Train_Data" : "../Data/mnist_train.art",
    "Test_Data"  : "../Data/mnist_test.art",
    "GPU" : true,
    "Epochs" : 2,
    "Learning_Rate": 0.004,
    "Layers": [
      {
        "Type": "FC",
        "Neurons_Count": 784,
        "Function": "tanh"
      },
      {
        "Type": "FC",
        "Neurons_Count": 470,
        "Function": "tanh"
      },
      {
        "Type": "FC",
        "Neurons_Count": 10,
        "Function": "softmax"
      }
    ],
    "Labels":[0,1,2,3,4,5,6,7,8,9]
  }
}
```
We need to specify the path for train data and test data using `Train_Data` and `Test_Data` fields.
### Data Format
The data format that is used by **Artificog** is similar to [LIBSVM](https://www.csie.ntu.edu.tw/~cjlin/libsvmtools/datasets/) input format.

## MNIST Example
In order to convert MNIST standard data format to **Artificog** compatible data format you can use the script `Examples/MNIST/convert.sh`. It stores compatible dataset into `Data/` directory and now you can run `./artificog` inside `build` direectory (or `./artificog ../Network.json`)and see how it learns to classify MNIST dataset.

For More information see : http://artificog.com







