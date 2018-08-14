#include<opencv2/opencv.hpp>
#include<iostream>
#include<caffe/caffe.hpp>
#include<caffe/common.hpp>
#include<caffe/util/upgrade_proto.hpp>
//#include<string>
//#include<bits/stdc++.h>

#define NO_CLASSES 41

using namespace std;
using namespace cv;
using namespace caffe;

string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}



int main(int argc, char** argv)
{
    cout << "Tesing code for pspnet" << endl;

    string test_proto = "/evaluation/resnet50_testing.prototxt";
    string pre_trained_model = "<path/to/trained/model>";

    string image_path ="<path/to/image/folder>";


    ////////////setting caffe mode and device id ///////////////////////////////////

    Caffe::set_mode(Caffe::GPU);
    Caffe::SetDevice(0);


    ////defining net parameter and reading proto file ///////////////////////////////

    caffe::NetParameter net_param;
    caffe::ReadNetParamsFromTextFileOrDie(test_proto,&net_param);




    ///// intialize the whole net //////////////////
    caffe::Net<float> test_pspnet(net_param);


    ///// defining input and output blobs of network /////////////////////////////
    vector<Blob<float>* > input_blobs;
    vector<Blob<float>* > output_blobs;


    input_blobs = test_pspnet.input_blobs();
    output_blobs = test_pspnet.output_blobs();


    //////Reading the weights from pretrainde caffe model ///////////////////////////////

    test_pspnet.CopyTrainedLayersFrom(pre_trained_model);



    vector<int> input_blob_dim,output_blob_dim;


    cout << "input blob dimensions : " ;
    for(int i=0;i<input_blobs[0]->shape().size();i++)
    {
        input_blob_dim.push_back(input_blobs[0]->shape(i));
        cout << input_blobs[0]->shape(i) << "\t";
    }

    cout << endl;

    cout << "output blob dimensions : " ;
    for(int i=0;i<output_blobs[0]->shape().size();i++)
    {
        output_blob_dim.push_back(output_blobs[0]->shape(i));
        cout  << output_blobs[0]->shape(i) << "\t" ;
    }

    cout << endl;

    int color[123]={150, 150, 150,
                    102, 51, 0,
                    102, 102, 0,
                    51, 102, 0,
                    0,102,0,
                    0, 102, 51,
                    0, 102, 102,
                    0, 51, 102,
                    0, 0, 102,
                    51, 0, 102,
                    102, 0, 102,
                    102, 0, 51,
                    32, 32, 32,
                    255, 0, 0,
                    255, 128, 0,
                    255, 255, 0,
                    128, 255, 0,
                    0, 255, 0,
                    0, 255, 128,
                    0, 255, 255,
                    0, 128, 255,
                    0, 0, 255,
                    127, 0, 255,
                    255, 0, 255,
                    255, 102, 178,
                    51, 0, 0,
                    51, 25, 0,
                    51, 51, 0,
                    25, 51, 0,
                    0, 51, 0,
                    0, 51, 25,
                    0, 25, 51,
                    0, 0, 51,
                    51, 0, 51,
                    51, 0, 25,
                    255, 153, 153,
                    255, 255, 153,
                    153, 255, 153,
                    153, 255, 255,
                    153, 204, 255,
                    204, 153, 255
                   };
    int it=0;
    int images[29]={ 1, 2, 4, 5, 7, 9, 10, 11 ,12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 26, 27, 29, 30, 31, 32, 33, 34};
    while(it<29)
    {
        //        int it2= it*150;
        cout << "Testing the segmentation" << endl;

        stringstream ss;
        //        ss << image_path << it << ".jpg";
        ss << image_path << images[it] << ".png";

        cv::Mat input_image;



        input_image = imread(ss.str());


        //// resize the input image according the network input
        cv::resize(input_image,input_image,cv::Size(input_blob_dim[2],input_blob_dim[3]));


        float* input_data_net = ((caffe::Blob<float>*)(input_blobs[0]))->mutable_cpu_data();

        cout << "input_image size" << input_image.rows << " " << input_image.cols << endl;

        float* input_data_net_ch1 = input_data_net;
        float* input_data_net_ch2 = input_data_net + input_image.rows*input_image.cols;
        float* input_data_net_ch3 = input_data_net + 2*input_image.rows*input_image.cols;




        /////////////// Feeding image into the network //////////////////////////////

        for(int i=0;i<input_image.rows ; i++)
            for(int j=0;j<input_image.cols ; j++)
            {
                //                float* pixel = input_image.at<cv::Vec3b>(i,j);

                (input_data_net_ch1 + i*input_image.cols)[j] = input_image.at<cv::Vec3b>(i,j)[0];
                (input_data_net_ch2 + i*input_image.cols)[j] = input_image.at<cv::Vec3b>(i,j)[1];
                (input_data_net_ch3 + i*input_image.cols)[j] = input_image.at<cv::Vec3b>(i,j)[2];

            }



        std::cout << "Forward passing\n";
        test_pspnet.ForwardFrom(0);
        std::cout << "Forward passing Done\n";

        float* output_data_net = ((caffe::Blob<float>* )(output_blobs[0]))->mutable_cpu_data();
        cv::Mat mask= cv::Mat::zeros(input_image.rows,input_image.cols,CV_8UC3);


        //////////// Analaysing the output /////////////////////////////////////////////////
        cout<<"\n\nanalysing output"<<endl;

        for(int i=0 ; i< input_image.rows ; i++)
        {
            for(int j=0 ; j< input_image.cols ; j++)
            {
                float* prob_pixel = output_data_net + i* input_image.cols + j;
                int jump = mask.cols * mask.rows;

                float max = -1.0;
                int index =0;

                for(int k=0;k<output_blob_dim[1];k++)
                {

                    int k_jump=k*jump;
                    float val = (prob_pixel + k_jump)[0];
                    ;
                    if(val >  max)
                    {
                        max = val;
                        index = k;
                    }
                }

                if((index)&&(max>0.4))
                {


                    int id =index*3;
                    mask.at<Vec3b>(i,j,0)[2]=0.85*color[id+0]+.15*(input_image.at<cv::Vec3b>(i,j)[2]);
                    mask.at<Vec3b>(i,j,0)[1]=0.85*color[id+1]+.15*(input_image.at<cv::Vec3b>(i,j)[1]);
                    mask.at<Vec3b>(i,j,0)[0]=0.85*color[id+2]+.15*(input_image.at<cv::Vec3b>(i,j)[0]);

                }
                else
                {
                    mask.at<Vec3b>(i,j,0)[2]=input_image.at<cv::Vec3b>(i,j)[2];
                    mask.at<Vec3b>(i,j,0)[1]=input_image.at<cv::Vec3b>(i,j)[1];
                    mask.at<Vec3b>(i,j,0)[0]=input_image.at<cv::Vec3b>(i,j)[0];
                }
            }
            //cout<<endl;
        }

        std::ostringstream filename;
        filename <<"<path/to/output/folder>/image_"<<it<<".png";

        imwrite(filename.str(),mask);


        it=it+1;
    }


    return 0;
}
