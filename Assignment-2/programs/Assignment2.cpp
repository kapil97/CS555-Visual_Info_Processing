#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
cv::Mat unsharpMaskingGaussian(cv::Mat input_image){
    cv::Mat blurred_image=input_image.clone();
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
    cv::waitKey(0);
    return 0;
}
