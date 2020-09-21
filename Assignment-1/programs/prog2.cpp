#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int getAverageIntensityValue(int hist[], int a, int b)													//Function to find average_intensity_value of histogram based on algorithm provided in the README file
{
	int sum_of_pixels=0;
	sum_of_pixels = sum_of_pixels + hist[0];
	int sum_product_of_pixels = (hist[0] * 0);

	for (int i = a; i < b; i++)
	{
		sum_of_pixels = sum_of_pixels + hist[i];
	}

	for (int i = a; i < b; i++)
	{
		sum_product_of_pixels = sum_product_of_pixels + (hist[i] * i) ;
	}

	int average_intensity_value = sum_product_of_pixels / sum_of_pixels;														//average_intensity_value calculation code ends here
	return average_intensity_value;
}

cv::Mat getBinaryImage(Mat img, int threshold)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (int(img.at<uchar>(i, j)) <= threshold)
			{
				img.at<uchar>(i, j) = 0;																						//If pixel intensity is greater than threshold
			}
			else
			{
				img.at<uchar>(i, j) = 255;																						//If pixel intensity is less than threshold
			}
		}
	}

	return img;
}

cv::Mat convertToBinary(Mat img){
	int rows = img.rows;
	int cols = img.cols;
	int original_image_histogram_values[255];
	for (int a = 0; a < 256; a++)																			//Setting histogram Y axis values to zero
	{
		original_image_histogram_values[a] = 0;
	}

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			original_image_histogram_values[int(img.at<uchar>(i, j))]++;										//Generate histogram for original image
		}
	}

	int old_threshold = 128;
	int new_threshold = 128;
	int delta_threshold = 1;
	int m1, m2;
	
	do
	{
		old_threshold = new_threshold;
		m1 = getAverageIntensityValue(original_image_histogram_values, 1, old_threshold);
		m2 = getAverageIntensityValue(original_image_histogram_values, old_threshold + 1, 256);
		new_threshold = (m1 + m2) / 2;
		std::cout<<"Old Threshold "<<old_threshold<<"new threshold"<<new_threshold<<std::endl;
		
	}while ((old_threshold-new_threshold) > delta_threshold);
	std::cout<<"Threshold"<<new_threshold<<std::endl;
	img = getBinaryImage(img, new_threshold);
	return img;
}
int main(){

	Mat shapes=imread("shapes2.1.bmp",IMREAD_GRAYSCALE);
	imshow("shapes before binary conversion",shapes);
	Mat res = convertToBinary(shapes);
	imshow("shapes after Binary Conversion",res);
	Mat dont_panic=imread("guide_8bits.bmp",IMREAD_GRAYSCALE);
	imshow("Don't Panic Image before Binary Conversion",dont_panic);
	res = convertToBinary(dont_panic);
	imshow("Don't Panic Binary Image",res);
	cv::waitKey(0);
	destroyAllWindows();
	return 0;
}