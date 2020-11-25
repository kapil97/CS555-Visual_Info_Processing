#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

struct info
{
	int average;
	int label;
	
};

info train[2400], test[2400];
int training[2400][4][4], testing[2400][4][4], blocks;
Mat img3, img4, img7, img8;

Mat Generate_M1(Mat input)
{
	int p=0,label;
	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);
	Mat m1_img = input.clone();
	for (int m=0; m<height/2; m=m+4)
	{
		for (int n=0; n<width; n=n+4)					
		{
			for (int i=m; i<m+4; i++)
			{
				for (int j=n; j<n+4; j++)
				{
					int avg = 0, temp = 0;
					for (int x=m; x<m+4; x++)
					{
						for (int y=n; y<n+4; y++)
						{
							temp += input.at<uchar>(x, y);
						}
					}
					avg = temp / 16;
					train[p].average = avg;

					if (avg < 125)
					{   label = 0;
						train[p].label = label;
					}
					else if (avg >= 125 && avg < 175)
					{   label = 128;
						train[p].label = label;
					}
					else if (avg >= 175)
					{   label = 255;
						train[p].label = label;
					}

					for (int x = m; x < m + 4; x++)
					{
						for (int y = n; y < n + 4; y++)
						{
							m1_img.at<uchar>(x, y) = label;

						}
					}

				}
			}
			p++;
		}
	}
	return m1_img;
}

void Top(Mat input, int temp1[2400][4][4])
{
	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);
	int label, p=0;
	for (int m=0; m<height/2; m=m+4)
	{
		for (int n=0; n<width; n=n+4)					
		{
			for (int i=m; i<m+4; i++)
			{
				for (int j=n; j<n+4; j++)
				{
					int avg=0, temp=0, x1=0;
					for (int x=m; x<m+4; x++)
					{
						int y1=0;
						for (int y=n; y<n+4; y++)
						{
							temp1[p][x1][y1]=input.at<uchar>(x, y);
							y1++;
							temp += input.at<uchar>(x, y);
						}
						x1++;
					}
					avg = temp / 16;
					train[p].average = avg;

					if (avg < 125)
					{
						label = 0;
						train[p].label = label;
					}
					else if (avg >= 125 && avg < 175)
					{
						label = 128;
						train[p].label = label;
					}
					else if (avg >= 175)
					{
						label = 255;
						train[p].label = label;
					}

				}
			}
			p++;
		}
	}
	
}

void Bottom(Mat input, int temp1[2400][4][4])
{
	
	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);
	int label, p = 0;
	for (int m=input.rows/2; m<height; m= m+4)
	{
		for (int n=0; n<width; n=n+4)					
		{
			for (int i=m; i<m+4; i++)
			{
				for (int j=n; j<n+4; j++)
				{
					int avg=0, temp=0, x1=0;
					for (int x=m; x<m+4; x++)
					{
						int y1 = 0;
						for (int y = n; y < n + 4; y++)
						{
							temp1[p][x1][y1] = input.at<uchar>(x, y);
							y1++;
							temp += input.at<uchar>(x, y);
						}
						x1++;
					}
					avg = temp / 16;
					test[p].average = avg;

					if (avg < 125)
					{
						label = 0;
						test[p].label = label;
					}
					else if (avg >= 125 && avg < 175)
					{
						label = 128;
						test[p].label = label;
					}
					else if (avg >= 175)
					{
						label = 255;
						test[p].label = label;
					}
				}
			}
			p++;
		}
	}
	

}

Mat Generate_N1(Mat input, int training[2400][4][4], int testing[2400][4][4], int blocks)
{
	Mat n1_img = input.clone();
	int mean, avg, temp, sum, classID, p;

	for (int i = 0; i < blocks; i++)
	{
		mean = 128;
		for (int j = 0; j < blocks; j++)
		{
			avg = 0, temp = 0;
			for (int k = 0; k < 4; k++)
			{
				for (int l = 0; l < 4; l++)
				{
					temp += pow(training[j][k][l] - testing[i][k][l], 2);
				}
			}
			sum = sqrt(temp);
			avg = sum / 16;
			if (avg < mean)
			{
				mean = avg;
				classID = j;
			}

		}
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				testing[i][x][y] = training[classID][x][y];		
			}
		}
	}

	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);

	p = 0;
	for (int m=height/2; m<height; m=m+4)
	{
		for (int n=0; n<width; n=n+4)					
		{
			for (int i=m; i<m+4; i++)
			{
				for (int j=n; j<n+4; j++)
				{
					for (int x=m; x<m+4; x++)
					{
						for (int y=n; y<n+4; y++)
						{
							n1_img.at<uchar>(x, y) = testing[p][0][0];
						}
					}
				}
			}
			p++;
		}
	}
	return n1_img;
}


Mat Generate_N2(Mat input, int training[2400][4][4], int testing[2400][4][4], int blocks)
{
	Mat n2_img = input.clone();
	int classID = 0, count = 0;
	unsigned int distance, new_distance = 255;
	for (int i = 0; i < blocks; i++)
	{
		distance = 255;
		for (int j = 0; j < blocks; j++)
		{
			new_distance = 255;
			new_distance = train[j].average - test[i].average;
			if (new_distance < distance)
			{
				distance = new_distance;
				classID = j;
			}

		}

		for (int k=0; k<4; k++)
		{
			for (int l=0; l<4; l++)
			{
				testing[i][k][l] = training[classID][k][l];
				
			}
		}
	}

	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);
	int p = 0;
	for (int m=height/2; m<height; m=m+4)
	{
		for (int n=0; n<width; n=n+4)					
		{
			for (int i=m; i<m+4; i++)
			{
				for (int j=n; j<n+4; j++)
				{
					for (int x=m; x<m+4; x++)
					{
						for (int y=n; y<n+4; y++)
						{
							n2_img.at<uchar>(x, y) = testing[p][0][0];
						}
					}
				}
			}
			p++;
		}
	}

	return n2_img;
}

Mat Generate_N3(Mat input, int testing[2400][4][4], int blocks)
{
	Mat n3_img = input.clone();
	int classID = 0, count = 0;
	unsigned int distance, new_distance = 255;
	for (int i = 0; i <blocks; i++)
	{
		distance = 255;
		for (int j = 0; j <blocks; j++)
		{
			new_distance = 255;
			new_distance = train[j].average - test[i].average;
			if (new_distance < distance)
			{
				distance = new_distance;
				classID = j;
			}

		}

		for (int k =0; k<4; k++)
		{
			for (int l =0; l<4; l++)
			{
				testing[i][k][l] = train[classID].average;
				
			}
		}
	}

	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);

	int p = 0;
	for (int m = height / 2; m < height; m += 4)
	{
		for (int n = 0; n < width; n += 4)					
		{
			for (int i = m; i < m + 4; i++)
			{
				for (int j = n; j < n + 4; j++)
				{
	
					for (int x = m; x < m + 4; x++)
					{
	
						for (int y = n; y < n + 4; y++)
						{
							n3_img.at<uchar>(x, y) = testing[p][0][0];
						}
					}
				}
			}
			p++;
		}
	}

	return n3_img;
}

int average(int m)
{
	int i=0;
	int temp=0, avg;
	while (i<2400)
	{
		if (train[i].label == m)
		{
			temp += train[i].average;
		}
		i++;
	}
	avg = temp / i;
	return avg;
}

Mat Generate_N4(Mat input, int testing[2400][4][4], int blocks)
{
	Mat n4_img = input.clone();
	int classID = 0, count = 0, matching_label, cumulative_avg;
	unsigned int distance, new_distance = 255;
	for (int i = 0; i < blocks; i++)
	{
		distance = 255;
		for (int j = 0; j < blocks; j++)
		{
			new_distance = 255;
			new_distance = train[j].average - test[i].average;
			if (new_distance < distance)
			{
				distance = new_distance;
				classID = j;
			}

		}

		matching_label = train[classID].label;
		cumulative_avg = average(matching_label);

		for (int k = 0; k<4; k++)
		{
			for (int l = 0; l<4; l++)
			{
				testing[i][k][l] = cumulative_avg;
			}
		}
	}

	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);
	int p = 0;
	for (int m = height/2; m<height; m = m+4)
	{
		for (int n = 0; n<width; n = n+4)				
		{
			for (int i = m; i <m+4; i++)
			{
				for (int j = n; j<n+4; j++)
				{
				
					for (int x = m; x <m+4; x++)
					{
						
						for (int y = n; y < n + 4; y++)
						{
							n4_img.at<uchar>(x, y) = testing[p][0][0];
						}
					}
				}
			}
			p++;
		}
	}

	return n4_img;
}

Mat Generate_T1(Mat input)
{
	int height = input.rows - (input.rows % 4);
	int width = input.cols - (input.cols % 4);
    
	int p = 0,label;
	Mat t1_img = input.clone();
	for (int m=input.rows/2; m<height; m = m+4)
	{
		for (int n=0; n<width; n = n+4)					
		{
			for (int i=m; i<m+4; i++)
			{
				for (int j=n; j <n+4; j++)
				{
					int avg = 0, temp=0;
					for (int x=m; x<m+4; x++)
					{
						for (int y=n; y<n+4; y++)
						{
							temp += input.at<uchar>(x, y);
						}
					}
					avg = temp / 16;
					test[p].average = avg;

					if (avg < 125)
					{
						label = 0;
						test[p].label = label;
					}
					else if (avg >= 125 && avg < 175)
					{
						label = 128;
						test[p].label = label;
					}
					else if (avg >= 175)
					{
						label = 255;
						test[p].label = label;
					}

					for (int x=m; x<m+4; x++)
					{
						for (int y=n; y<n+4; y++)
						{
							t1_img.at<uchar>(x, y)=label;

						}
					}

				}
			}
			p++;
		}
	}
	return t1_img;
}
void k_Means(Mat input)
{
	Mat k1 = input.clone();
	int height = input.rows;
	int width = input.cols;

	for (int i=0; i<height/2; i++)
	{
        for (int j=0; j<width; j++)
        {
            if (input.at<uchar>(i, j) > 0 && input.at<uchar>(i, j) <= 85 )
            k1.at<uchar>(i, j) = 0;
            else if ( input.at<uchar>(i, j) > 85 && input.at<uchar>(i, j) <= 170 )
            k1.at<uchar>(i, j) = 128;
            else if( input.at<uchar>(i, j) > 170 && input.at<uchar>(i, j) <= 255 )
            k1.at<uchar>(i, j) = 255;
        }
	}
	imshow("K-means", k1);
}


void error(int blocks){
    float error = 0;
	float no_of_different_averages = 0;
	float no_of_same_averages = 0;
	float no_of_different_labels = 0;
	float no_of_same_labels = 0;
	for (int i = 0; i < blocks; i++)
	{

		if (train[i].average != test[i].average)
			no_of_different_averages++;
		else if (train[i].average == test[i].average)
			no_of_same_averages++;

		if (train[i].label != test[i].label)
			no_of_different_labels++;
		else if (train[i].label == test[i].label)
			no_of_same_labels++;

	}

	error = (no_of_different_labels / blocks) * 100;
	cout <<"Error Rate:"<< error << endl;
}

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,double, const Scalar& color, int temp)
{
    for(int i=0; i<cflowmap.rows; i= i+step){
        for(int j=0; j<cflowmap.cols; j= j+step)
        {
            const Point2f& fxy = flow.at<Point2f>(i, j);
            if (temp == 0){
                line(cflowmap, Point(j,i), Point(cvRound(j+fxy.x), cvRound(i+fxy.y)),color);
            }
            if (temp == 1){
                line(cflowmap, Point(j,i), Point(cvRound(j+fxy.x), cvRound(i+fxy.y)),CV_RGB(255, 0, 0));
            }
            circle(cflowmap, Point(j,i), 2, color, -1);
        }
    }
}

void MotionVectors(string filename1,string filename2){
    Mat img, gray, prev_gray, flow, cflow;
	if((!(img = imread( filename2, IMREAD_COLOR)).empty()))
	    {
		  cvtColor(img, gray, COLOR_BGR2GRAY);
		  prev_gray = imread(filename1,0);

		  if(!prev_gray.empty())
		  {
		    calcOpticalFlowFarneback(prev_gray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
		    cvtColor(prev_gray, cflow, COLOR_GRAY2BGR);
		    drawOptFlowMap(flow, cflow, 8,1.5, CV_RGB(0,255,0), 0);

		    imshow("Motion Vectors", cflow);
		  }

		  std::swap(prev_gray, gray);
	  }	
}

void difference_two_frames(string filename1 , string filename2, string filename3, string filename4){
    Mat m, n;
    Mat img1 = imread(filename1, 0);
    Mat img2 = imread(filename2, 0);
    Mat img5 = imread(filename3, 0);
    Mat img6 = imread(filename4, 0);

    if (img1.empty() || img2.empty() || img5.empty() || img6.empty())
    {
        cout << "Read error" << endl;
        return;
    }

    absdiff(img1,img2,img3);
    imshow("Absolute Diff PlayFrames",img3);
    threshold(img3, img4, 128, 255, THRESH_BINARY);
    imshow("Threshold Diff PlayFrames",img4);

    absdiff(img5,img6,img7);
    imshow("Absolute Diff Tennis Frame",img7);
    threshold(img7, img8, 128, 255, THRESH_BINARY);
    imshow("Threshold Diff Tennis Frame",img8);
}
int main(){

    Mat input_image = imread("images/TestImage-even-width.bmp",IMREAD_GRAYSCALE);
    imshow("Original Image",input_image);
    Mat m1_img = Generate_M1(input_image);
    imshow("M1 Image",m1_img);

    blocks = ((input_image.rows/2)/4) * (input_image.cols/4);
	Top(input_image, training);
	Bottom(input_image, testing);
	Mat N1 = input_image.clone();
	N1 = Generate_N1(input_image, training, testing, blocks);
	imshow("N1", N1);

    error(blocks);

    blocks = ((input_image.rows/2)/4) * (input_image.cols/4);
	Top(input_image, training);
	Bottom(input_image, testing);
	Mat N2 = input_image.clone();
	N2 = Generate_N2(input_image, training, testing, blocks);
	imshow("N2", N2);

    blocks = ((input_image.rows/2)/4) * (input_image.cols/4);
	Top(input_image, training);
	Bottom(input_image, testing);
	Mat N3 = input_image.clone();
	N3 = Generate_N3(input_image, testing, blocks);
	imshow("N3", N3);

    blocks = ((input_image.rows/2)/4) * (input_image.cols/4);
	Top(input_image, training);
	Bottom(input_image, testing);
	Mat N4 = input_image.clone();
	N4 = Generate_N4(input_image, testing, blocks);
	imshow("N4", N4);

    Mat t1_img = Generate_T1(input_image);
    imshow("T1",t1_img);
    k_Means(input_image);
    MotionVectors("images/PlayFrame1_320x200.bmp","programs/images/PlayFrame2_320x200.bmp");
    difference_two_frames("images/PlayFrame1_320x200.bmp","images/PlayFrame2_320x200.bmp",
                    "images/TennisFrame1_320x200.bmp","images/TennisFrame2_320x200.bmp");
    waitKey(0);
    return 0;
}