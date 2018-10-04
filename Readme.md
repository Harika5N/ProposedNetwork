# Warehouse Cluttered Objects Segmentation Net

This repository is in support to the paper by Kanika Mahajan, Dr. Anima Majumder, Harika Nanduri, Dr. Swagat Kumar from Tata Consultancy Services Ltd, India, published in BMVC,2018.

## Introduction

The network proposed is a deep convolutional neural network using FPN with a backbone of resnet for semantic segmentation. The network has shown good accuracy and precision and performs better than the state of the art techniques PSPNET and MaskRCNN for a limited training duration(30 minutes).

### Installing

The code is tested on Ubuntu 14.04 machine

1. Clone the repository

```
git clone https://github.com/Harika5N/ProposedNetwork.git
```

2. Build the caffe and matcaffe.

3. Training:
	a) Download the tcs\_arc dataset which was used for training this project from:[tcs\_arc](https://doi.org/10.6084/m9.figshare.6848738.v1)
	b) Download the datasets for pretraining the models:[ADE20K](https://drive.google.com/file/d/0BzaU285cX7TCN1R3QnUwQ0hoMTA/view)
							    [VOC2012](https://drive.google.com/file/d/0BzaU285cX7TCNVhETE5vVUdMYk0/view)
	b) Generate a .txt files as the ones present in models/txt_files for training set and validation set using the location where the 		downloaded dataset was saved.
	c) Edit the locations of the text files generated in the above step in the prototxt file for resnet 50 or resnet 101 backbone network.
	d) Edit the location prototx files in the solver file for corresponding network.
	e) To train:
		```
		cd ~/Pspnet-caffe/caffe-pspnet/
build/tools
		```
		resnet50:
		```
		./caffe train -gpu 0,1 -solver .... -weights /<download location of ade20k dataset>/ade20k
		```
		resnet101:
		```
   		./caffe train -gpu 0,1 -solver .... -weights /<download location of VOC2012 dataset>/voc2012
		```
4. Evaluation: 
	a) The evaluation code is available in evaluation folder.
	b) Build the code and run it as:
		```
		* cd evaluation/build
		* ./test
		```
		

## Results
The models will be saved at the location mentioned in the snapshot prefix argument in the solver file. The results for the tcs_arc dataset is observed as :
	* For resnet\_50: mIOU = 87.2529%	pixel accuracy = 96.7625%	F1 Score = 93.0816%
	* For resnet\_101: mIOU = 86.5832	pixel accuracy = 96.5422%	F1 Score = 91.0482%


### Acknowledgments

Inspiration for this network was taken from PSPNET @inproceedings{zhao2017pspnet,
  author = {Hengshuang Zhao and
            Jianping Shi and
            Xiaojuan Qi and
            Xiaogang Wang and
            Jiaya Jia},
  title = {Pyramid Scene Parsing Network},
  booktitle = {Proceedings of IEEE Conference on Computer Vision and Pattern Recognition (CVPR)},
  year = {2017}
}


