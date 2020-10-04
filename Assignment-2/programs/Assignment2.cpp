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
    return blurred_image;
}

int main(int argc, char** argv){
    // std::string filename="";
    // while(argc<3){
    //     filename=argv[1];
    // }
    cv::Mat input_image=cv::imread("TestImage/basel_gray.bmp",cv::IMREAD_GRAYSCALE);
    cv::imshow("Input Image", input_image);
    cv::Mat blur_image = unsharpMaskingGaussian(input_image);
    cv::imshow("Blurred image", blur_image);
    cv::waitKey(0);
    return 0;
}
