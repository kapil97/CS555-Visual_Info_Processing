#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
class Shape{
public:
	int x,y,p;
};

Mat binaryImage;
int pixels;
vector<Shape> shape, temp;
int visited[1000][1000]={}; 

bool check(Mat img, int i,int j){
  return (i>=0 && i<img.rows && j>=0 && j<img.cols);
}
void DFS(Mat img, int i, int j)
{ 	
	
	pixels++;                 
	visited[i][j]=1; 
	// cout<<"DFS"<<" i: "<<i<<" j "<<j<<visited[i][j]<<" "<<endl;               
	if(check(img, i+1,j) && !visited[i+1][j] && img.at<uchar>(i+1, j) == 255)
	      DFS(img,i+1,j);

	if(check(img, i,j+1) && !visited[i][j+1] && img.at<uchar>(i, j+1) == 255)
	      DFS(img,i,j+1);
	
	if(check(img, i-1,j) && !visited[i-1][j] && img.at<uchar>(i-1, j) == 255)
	      DFS(img,i-1,j);
	
	if(check(img, i,j-1) && !visited[i][j-1] && img.at<uchar>(i, j-1) == 255)
	      DFS(img,i,j-1);	
}

int count_regions(Mat input){
	// cout<<"Here"<<endl;
    int num_comp=0;
    int k = 0;
    for (int i = 0; i < input.rows; i++){
        for (int j = 0; j < input.cols; j++){
			// cout<<visited[i][j]<<endl; 
			// cout<<visited[i][j]<<" "; 
            if (visited[i][j]==0 && input.at<uchar>(i, j) == 255)
            {    
				pixels=0; 
				cout<<"Here DFS"<<endl;                 
				DFS(input, i, j);
				shape[k++].x = i;
				shape[k++].y = j;
				shape[k++].p = pixels;
				if (pixels > 41)
					num_comp++;      
            }
		}
	}
    return num_comp;
}

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
	int list[10];
	Mat eroded_image = input_image.clone();
	for (int i=0; i<input_image.rows; i++)
        for (int j=0; j<input_image.cols; j++)
            eroded_image.at<uchar>(i,j)=0;

	for (int i=0; i<input_image.rows; i++){
    	for (int j=0; j<input_image.cols; j++){
      		int l = 0;
      		for (int m=0; m<3; m++){
        		for (int n=0; n<3; n++) {
          			if ((i-1+m) >= 0 && (j-1+n) >=0 && (i-1+m) < input_image.rows && (j-1+n) < input_image.cols){
            			list[l++] = input_image.at<uchar>(i-1+m, j-1+n);
          			}
        		}
      		}
			eroded_image.at<uchar>(i, j) = *min_element(list,list+10);;
		}
	}
	return eroded_image;
}

Mat dilation_gray(Mat input_image) {
	Mat dilated_image=input_image.clone();
	for (int i=0; i<input_image.rows; i++)
        for (int j=0; j<input_image.cols; j++)
            dilated_image.at<uchar>(i,j)=0;
	int list[10];
	for (int i=0; i<input_image.rows; i++){
		for (int j=0; j<input_image.cols; j++) {
		int l= 0;
		for (int m=0; m<3; m++){
			for (int n=0; n<3; n++) {
				if ((i-1+m) >= 0 && (j-1+n) >=0 && (i-1+m) < input_image.rows && (j-1+n) < input_image.cols){
					list[l++] =input_image.at<uchar>(i-1+m, j-1+n);
				}
			}
		}
		dilated_image.at<uchar>(i,j) = *min_element(list,list+10);
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
	int components = count_regions(dilated_image);
	cout<<"Count: "<< components;
    imshow("Seperated Image", seperated_img);
	imshow("Eroded Grayscale Image",erosion_gray(input_image_odd));
    imshow("Dilation Grayscale Image",dilation_gray(input_image_odd));
    waitKey(0);
    return 0;
}