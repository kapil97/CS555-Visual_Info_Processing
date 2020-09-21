#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int original_img_hist[255];
int negative_img_hist[255];
void generateHistogram(cv::Mat img, int img_hist[])
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			img_hist[int(img.at<uchar>(i, j))]++;
		}
	}
}		
void dispHistogram(int img_hist[], std::string win_name){
	int histogram_height = 400;
	int histogram_width = 512;
	int max_intensity_value = 0;
	int binary_width = cvRound((double)histogram_width / 256);
	cv::Mat histogram_image(histogram_height, histogram_width, CV_8UC1, cv::Scalar(255, 255, 255));

	max_intensity_value = img_hist[0];

	for (int i =1; i<=255; i++)
	{
		if (img_hist[i] > max_intensity_value)
		{
			max_intensity_value = img_hist[i];
		}
	}

	for (int i =0; i<=255; i++)
	{
		img_hist[i] = ((double)img_hist[i] / max_intensity_value) * histogram_image.rows;
	}

	for (int i =0; i <=255; i++)
	{
		line(histogram_image, Point(binary_width*(i), histogram_height), Point(binary_width*(i), histogram_height - img_hist[i]), Scalar(0,0,0),1,8,0);
	
	}
	imshow(win_name, histogram_image);

}
void generateNegativeImage(cv::Mat img,cv::Mat mod)
{
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			mod.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);		
		}
	}
	imshow("Negative", mod);
	
}

cv::Mat equalizeHistogram(cv::Mat &img, std::string win_name){
    // Total number of occurance of the number of each pixels at different levels from 0 - 256
    // Flattening our 2d matrix
    int flat_img[256] = {0};
    for(int i=0; i<img.rows; i++){
        for(int j=0; j<img.cols; j++){
            int index;
            index = static_cast<int>(img.at<uchar>(i,j)); // safe convertion to int
            flat_img[index]++;
        }
    }

    // calculate CDF corresponding to flat_img
    // CDF --> cumsum
    int cumsum[256]={0};
    int memory=0;
    for(int i=0; i<256; i++){
        memory += flat_img[i];
        cumsum[i] = memory;
    }

    // using general histogram equalization formula
    int normalize_img[256]={0};
    for(int i=0; i<256; i++){
    	// norm(v) = round(((cdf(v) - mincdf) / (M * N) - mincdf) * (L - 1));
        normalize_img[i] = ((cumsum[i]-cumsum[0])*255)/(img.rows*img.cols-cumsum[0]);
        normalize_img[i] = static_cast<int>(normalize_img[i]);
    }
	int scaled[256]={0};
	std::copy(std::begin(normalize_img), std::end(normalize_img), std::begin(scaled));
	dispHistogram(scaled,win_name);

    // convert 1d back into a 2d matrix
    cv::Mat result(img.rows, img.cols, CV_8U);
    
    Mat_<uchar>::iterator itr_result = result.begin<uchar>(); // our result
    Mat_<uchar>::iterator it_begin = img.begin<uchar>(); // beginning of the image
    Mat_<uchar>::iterator itr_end = img.end<uchar>(); // end of the image
    
    for(; it_begin!=itr_end; it_begin++){
        int intensity_value = static_cast<int>(*it_begin); // get the value and cast it into an int
        *itr_result = normalize_img[intensity_value];
        itr_result++;
    }


    return result;
}


int main(int argc, char **argv) {
	const char *filename = "House_width_times4.bmp";

	Mat original_image = imread(filename, IMREAD_GRAYSCALE);
	Mat modified_image = imread(filename, IMREAD_GRAYSCALE);
	
	for(int i=0;i<=255;i++){
		original_img_hist[i]=0;
		negative_img_hist[i]=0;
	}
	imshow("Original Image ",original_image);
	generateHistogram(original_image ,original_img_hist);
	dispHistogram(original_img_hist,"Original Image Histogram");

	// cv::Mat *negativeImg = &original_image;
	generateNegativeImage(original_image,modified_image);
	generateHistogram(modified_image,negative_img_hist);
	dispHistogram(negative_img_hist,"Negative Image Histogram");
	Mat enhanced = equalizeHistogram(original_image,"After Equalization");
    imshow("Enhanced Image",enhanced);
	Mat nyc_image=imread("NYC_width_4times.bmp",IMREAD_GRAYSCALE);
	imshow("Before Equalization NYC",nyc_image);
	enhanced=equalizeHistogram(nyc_image, "After Equalization of New York");
	imshow("After Histogram Equalization",enhanced);
	cv::waitKey(0);
	destroyAllWindows();

	return 0;
}