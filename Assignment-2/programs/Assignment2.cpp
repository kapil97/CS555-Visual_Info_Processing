#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

cv::Mat LoGEdgeGeneration(cv::Mat input_image, int mask_size,double sigma){
    cv::Mat enhanced_image;
    int min = -(mask_size/2);
    int max = (mask_size/2);
    int mask[mask_size][mask_size];
    double e = 2.71,pi=3.1415;
    int scaling = 0;
    int l = 0;
    if(sigma==1.4 && mask_size==7)
        scaling=483;
    else
        scaling=40000;

    for(int i=min;i<=max;i++){
        int m=0;
        for(int j=min;j<=max;j++)
        {   
            double val= (-1.0)* (1.0/(pow(sigma,4)*pi)) * (1-((pow(i,2)+pow(j,2))/(2*pow(sigma,2)))) * pow(e,(-1* ((pow(i,2)+pow(j,2))/(2*pow(sigma,2)))));  
            mask[l][m++]=int(val*scaling);
        }
        l++;
    }
    for(int i=0; i<mask_size;i++){
        for(int j=0;j<mask_size;j++){
            std::cout<<mask[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    enhanced_image=input_image.clone();

    for (int i=0; i <mask_size; i++)
    {
        for (int j=0; j<mask_size; j++)
        {
            enhanced_image.at<uchar>(i, j) = 0;
        }
    }

    for (int x = max; x < input_image.rows - max; x++)
    {
        for (int y = max; y < input_image.cols - max; y++)
        {
            int p = 0, q = 0;
            int value = 0;
            for (int s = min; s <= max; s++)
            {
                for (int t = min; t <= max; t++)
                {
                    
                    value = value + mask[p][q] * input_image.at<uchar>(x + s, y + t);
                    q++;
                }
                p++;
            }
            if (value < 0)
            {
                value = 0;
            }
            else
            {
                enhanced_image.at<uchar>(x, y) = int(value) / 10000000;
            }
        }
    }
    return enhanced_image;
}
cv::Mat unsharpMaskingGaussian(cv::Mat input_image){
    cv::Mat blurred_image = input_image.clone();
    double gaussianMask[3][3]={ {1/22.0,3/22.0,1/22.0}, {3/22.0,6/22.0,3/22.0},{1/22.0,3/22.0,1/22.0} };

    for (int i=0; i<input_image.rows; i++)
    {
        for (int j=0; j<input_image.cols; j++)
        {
            blurred_image.at<uchar>(i, j) = 0.0;
        }
    }
    for(int i=1;i<input_image.rows-1;i++){
        for(int j=1;j<input_image.cols-1;j++){
            blurred_image.at<uchar>(i,j)=
            gaussianMask[0][0] * input_image.at<uchar>(i-1,j-1) +
            gaussianMask[0][1] * input_image.at<uchar>(i-1,j) +
            gaussianMask[0][2] * input_image.at<uchar>(i-1,j+1) +

            gaussianMask[1][0] * input_image.at<uchar>(i,j-1) +
            gaussianMask[1][1] * input_image.at<uchar>(i,j) +
            gaussianMask[1][2] * input_image.at<uchar>(i-1,j+1) +

            gaussianMask[2][0] * input_image.at<uchar>(i+1,j-1) +
            gaussianMask[2][1] * input_image.at<uchar>(i+1,j) +
            gaussianMask[2][2] * input_image.at<uchar>(i+1,j+1) ;
        }
    }
    for (int i=0; i<input_image.rows; i++)
    {
        for (int j=0; j<input_image.cols; j++)
        {
            blurred_image.at<uchar>(i, j) = input_image.at<uchar>(i, j) - blurred_image.at<uchar>(i, j);
        }
    }

    for(int i=0; i<input_image.rows; i++)
    {
        for (int j=0; j<input_image.cols; j++)
        {
            blurred_image.at<uchar>(i, j) = input_image.at<uchar>(i, j) + blurred_image.at<uchar>(i, j);
        }
    }

    return blurred_image;
}
cv::Mat sobelOperation(cv::Mat input_image){
    cv::Mat sobel_image=input_image.clone();
    for (int i=0; i<input_image.rows; i++)
    {
        for (int j=0; j<input_image.cols; j++)
        {
            sobel_image.at<uchar>(i, j) = 0.0;
        }
    }
    for(int i=1;i<input_image.rows-1;i++){
        for(int j=1;j<input_image.cols-1;j++){
            int gx=
            (input_image.at<uchar>(i-1,j-1)*(-1)) +
            (input_image.at<uchar>(i-1,j)*(0)) +
            (input_image.at<uchar>(i-1,j+1)*(1)) +

            (input_image.at<uchar>(i,j-1)*(-2)) +
            (input_image.at<uchar>(i,j)*(0)) +
            (input_image.at<uchar>(i,j+1)*(2)) +

            (input_image.at<uchar>(i+1,j-1)*(-1)) +
            (input_image.at<uchar>(i+1,j)*(0)) +
            (input_image.at<uchar>(i+1,j+1)*(1));
            
            int gy=
            (input_image.at<uchar>(i-1,j-1)*(-1)) +
            (input_image.at<uchar>(i-1,j)*(-2)) +
            (input_image.at<uchar>(i-1,j+1)*(-1)) +

            (input_image.at<uchar>(i,j-1)*(0)) +
            (input_image.at<uchar>(i,j)*(0)) +
            (input_image.at<uchar>(i,j+1)*(0)) +

            (input_image.at<uchar>(i+1,j-1)*(1)) +
            (input_image.at<uchar>(i+1,j)*(2)) +
            (input_image.at<uchar>(i+1,j+1)*(1));

            double g=sqrt(pow(gx,2)+pow(gy,2));
            if(g>255)
                g=255;
            else if(g<0)
                g=0;
            sobel_image.at<uchar>(i,j)=g;
        }
    }

    return sobel_image;
}
int main(int argc, char** argv){
    // std::string filename="";
    // while(argc<3){
    //     filename=argv[1];
    // }
    cv::Mat f1 = cv::imread("TestImage/ant_gray.bmp",cv::IMREAD_GRAYSCALE);
    cv::imshow("Input Image F1", f1);
    cv::Mat f2 = cv::imread("TestImage/basel_gray.bmp",cv::IMREAD_GRAYSCALE);
    cv::imshow("Input Image F2", f2);
    cv::Mat E1 = unsharpMaskingGaussian(f1);
    cv::imshow("Enhanced image E1", E1);
    cv::Mat E2 = unsharpMaskingGaussian(f2);
    cv::imshow("Enhanced image E2", E2);
    cv::Mat Es1 = sobelOperation(f1);
    cv::imshow("Edge Image ES1",Es1);
    cv::Mat Es2 = sobelOperation(f2);
    cv::imshow("Edge Image ES2",Es2);
    std::cout<<"MASK 7x7 for E1_1"<<std::endl;
    cv::Mat E1_1=LoGEdgeGeneration(f1,7,1.4);
    std::cout<<"MASK 7x7 for E2_1"<<std::endl;
    cv::Mat E2_1=LoGEdgeGeneration(f2,7,1.4);
    std::cout<<"MASK 11x11 for E1_2"<<std::endl;
    cv::Mat E1_2=LoGEdgeGeneration(f1,11,5);
    std::cout<<"MASK 11x11 for E1_2"<<std::endl;
    cv::Mat E2_2=LoGEdgeGeneration(f2,11,5);
    cv::imshow("LoG Edge Detection 7X7 for E1", E1_1);
    cv::imshow("LoG Edge Detection 7x7 for E2",E2_1);
    cv::imshow("LoG Edge Detection 11X11 for E1", E1_2);
    cv::imshow("LoG Edge Detection 11x11 for E2",E2_2);
    cv::waitKey(0);
    return 0;
}
