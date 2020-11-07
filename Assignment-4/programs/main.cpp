#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

Mat binaryImage;

Mat convert_binary(Mat input_img, int threshold)
{
	for (int i=0; i<input_img.rows; i++)
	{
		for (int j=0; j<input_img.cols; j++)
		{
			if (int(input_img.at<uchar>(i, j)) <= threshold)
				input_img.at<uchar>(i, j) = 0;
			else
				input_img.at<uchar>(i, j) = 255;
		}
	}
	return input_img;
}

Mat erosion_binary(Mat input_image){
    Mat eroded_image = input_image.clone();
	for (int i=0; i<input_image.rows; i++){
        for (int j=0; j<input_image.cols; j++){
            eroded_image.at<uchar>(i,j)=0;
        }
    }
	for (int i=0; i<input_image.rows; i++){
	    for (int j=0; j<input_image.cols; j++) {
	      if (input_image.at<uchar>(i,j)==255) { 
		   int temp=0;
            for (int m=0; m<3; m++){
                for (int n=0; n<3; n++) {
                    if ((i-2+m) >= 0 && (j-2+n) >=0 && (i-2+m) < input_image.rows && (j-2+n) < input_image.cols && input_image.at<uchar>(i-2+m,j-2+n)==255){
                        temp++;
                    }
                }
            }
            if (temp == 9){
            eroded_image.at<uchar>(i,j) = 255;
            }
		
	      }
	    }
	  }
    return eroded_image;
}
Mat dilation_binary(Mat input_image){
    Mat dilated_image = input_image.clone();
	for (int i=0; i<input_image.rows; i++)
        for (int j=0; j<input_image.cols; j++)
            dilated_image.at<uchar>(i,j)=0;

	for (int i=0; i<input_image.rows; i++){
	    for (int j=0; j<input_image.cols; j++) {
            if (input_image.at<uchar>(i,j)==255) {       
                for (int m=0; m<3; m++){
                    for (int n=0; n<3; n++) {
                        if ((i-2+m) >= 0 && (j-2+n) >=0 && (i-2+m) < input_image.rows && (j-2+n) < input_image.cols && input_image.at<uchar>(i-2+m, j-2+n)!=255)
                            input_image.at<uchar>(i-2+m, j-2+n) = 255;
                    
                    }
                }
	        }
	    }
	}
    for(int a=0; a < input_image.rows;a++){
        for( int b=0 ;b < input_image.cols; b++){
            if((input_image.at<uchar>(a,b))==255){
            dilated_image.at<uchar>(a,b)=binaryImage.at<uchar>(a,b);
            }
        }
    }

    return dilated_image;
}

Mat erosion_gray(Mat input_image) {
	int mask[3][3];
	for (int x=0; x<3; x++) {
		for (int y=0; y<3; y++) {
			mask[x][y] = 1;
		}
	}
	int list[10];
	Mat eroded_image = input_image.clone();
	for (int i=0; i<input_image.rows; i++)
        for (int j=0; j<input_image.cols; j++)
            eroded_image.at<uchar>(i,j)=0;

	for (int i=1; i<input_image.rows-1; i++) {
		for (int j=1; j<input_image.cols-1; j++) {
			int  n=0;
			for (int x=-1; x<= 1; x++) {
				for (int y=-1; y<= 1; y++) {
					list[n++] = input_image.at<uchar>(i + x, j + y)*mask[x + 1][y + 1];
				}
			}
           int min = list[0]; 
            for (i=1; i<n; i++) 
                if (list[i] < min) 
                    min = list[i];
			eroded_image.at<uchar>(i, j) = min;
		}
	}
	return eroded_image;
}

Mat dilation_gray(Mat input_image) {

	Mat dilated_image = input_image.clone();
	int mask[3][3];
	for (int x=0; x<3; x++) {
		for (int y=0; y<3; y++) {
			mask[x][y] = 1;
		}
	}
	int list[10];
	for (int i=1; i<input_image.rows-2; i++) {
		for (int j=1; j<input_image.cols-2; j++) {
			int  n=0;
			for (int x=-1; x<=1; x++) {
				for (int y=-1; y<=1; y++) {
					list[n] = input_image.at<uchar>(i + x, j + y)*mask[x + 1][y + 1];
					n++;
				}
			}
			int max = list[0]; 
            for (i=1; i<n; i++) 
                if (list[i] > max) 
                    max = list[i];
			dilated_image.at<uchar>(i, j) = max;
		}
	}
	return dilated_image;
}

Mat performErosionDilation(Mat input){

    Mat erosion = erosion_binary(input);
	Mat dilation = dilation_binary(erosion);
	erosion = erosion_binary(dilation);
	erosion = erosion_binary(erosion);
	dilation = dilation_binary(erosion);
	dilation = dilation_binary(dilation);
    dilation = dilation_binary(dilation);
    erosion = erosion_binary(dilation);
	dilation = dilation_binary(erosion);
    return dilation;
}

int main(){
    Mat input_image_even = imread("programs/images/TestImage-even-width.bmp",IMREAD_GRAYSCALE);
    Mat input_image_odd = imread("programs/images/TestImage-odd-width.bmp",IMREAD_GRAYSCALE);
    int intensity=0;
    imshow("Even Width",input_image_even);
    imshow("Odd Width",input_image_odd);
    for(int i=0;i<input_image_even.rows;i++){
        for(int j=0;j<input_image_even.cols;j++){
            intensity+=input_image_even.at<uchar>(i,j);
        }
    }
    int threshold = intensity/(input_image_even.rows*input_image_even.cols);
    cout<<threshold<<endl;
    Mat binary_image= convert_binary(input_image_even,threshold);
    imshow("Binary Image",binary_image);
    binaryImage=binary_image;
    Mat eroded_binary_image=erosion_binary(binary_image);
    imshow("Eroded Binary Image",eroded_binary_image);
    Mat dilated_image=dilation_binary(eroded_binary_image);
    imshow("Dilated Binary Image", dilated_image);
    Mat seperated_img=performErosionDilation(binary_image);
    imshow("Seperated Image", seperated_img);
    
    imshow("Eroded Grayscale Image",erosion_gray(input_image_odd));
    imshow("Dilation Grayscale Image",dilation_gray(input_image_odd));
    waitKey(0);
    return 0;
}