#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;

vector<double> generateDCT(int n, vector<int> input){
    vector<double> out;
    double pi= 3.1415;
    for(int i=0; i<n; i++){
        double sum=0.0;
        double value=0.0;
        double c=0.0;
        for(int j=0; j<n; j++)
            sum += input[j]*cos( (((2*j)+1)*i*pi) / (2*n));
        if(i==0)
            c=1/sqrt(2);
        else
            c=1;
        value = (sqrt(2.0/n)) * c* sum;
        out.push_back(value);
    }
    return out;
}

cv::Mat DCT_Image(cv::Mat input_image){
    int height = input_image.rows - (input_image.rows % 8);
	int width = input_image.cols - (input_image.cols % 8);
    double sum,pi=3.1415;
    cv::Mat dct_image(input_image.rows,input_image.cols,input_image.type());

    for (int m = 0; m < height; m += 8)
	{
		for (int n = 0; n < width; n += 8)
		{
			for (int i = m; i < m + 8; i++)
			{
				for (int j = n; j < n + 8; j++)
				{
					sum = 0.0;
					for (int x = m; x < m + 8; x++)
					{
						for (int y = n; y < n + 8; y++)
						{
							sum += (input_image.at<cv::Vec3b>(x, y)[2]) *
								(cos( ((((2*x) + 1)) * ((i*pi))) / (2*8) )) *
								(cos( ((((2*y) + 1)) * ((j*pi))) / (2*8) ));
						}
					}
                    double c_i,c_j;
                    if(i==0) c_i= 1/sqrt(2);
                    else c_i=1;
                    if(i==0) c_j= 1/sqrt(2);
                    else c_j=1;
					sum *= ((1/sqrt(2 * 8))) * (c_i*c_j);
					dct_image.at<cv::Vec3b>(i, j)[2] = int(sum);
				}

			}
		}
	}
    return dct_image;
}

cv::Mat DCC_component(cv::Mat input_image){

    int height = input_image.rows - (input_image.rows % 8);
	int width = input_image.cols - (input_image.cols % 8);
    cv::Mat dcc_image=input_image.clone();
    
    for (int m = 0; m < height; m += 8)
	{
		for (int n = 0; n < width; n += 8)					
		{
			for (int i = m; i < m + 8; i++)
			{
				for (int j = n; j < n + 8; j++)
				{
					if (i==m && j==n)
					{
                        continue;
					}
                    else
                    {
                        dcc_image.at<cv::Vec3b>(i, j)[2] = 0;
                    }
                    
				}
			}
		}
	}
	return dcc_image;
}

cv::Mat first9_freq(cv::Mat input_image){
    int height = input_image.rows - (input_image.rows % 8);
	int width = input_image.cols - (input_image.cols % 8);
    cv::Mat dcc_image_9=input_image.clone();
	
    for (int m = 0; m < height; m += 8)
	{
		for (int n = 0; n < width; n += 8)
		{
			for (int i = m; i < m + 8; i++)
			{
				for (int j = n; j < n + 8; j++)
				{
					if ((i == m && j == n)          ||
                        (i == m && j == n+1)        ||
                        (i == m && j == n + 2)      ||
                        (i == m && j == n + 3)      ||  
                        (i == m + 1 && j == n)      ||  
                        (i == m + 1 && j == n+1)    ||  
                        (i == m + 1 && j == n+2)    ||  
                        (i == m + 2 && j == n)      ||  
                        (i == m + 2 && j == n + 1))
					{
                        continue;
					}
					else
					{
						dcc_image_9.at<cv::Vec3b>(i, j)[2] = 0;
					}
				}
			}
		}
	}
    return dcc_image_9;
} 

cv::Mat IDCT(cv::Mat input_image){
    int height = input_image.rows - (input_image.rows % 8);
	int width = input_image.cols - (input_image.cols % 8);
	double sum,pi=3.1415;
    cv::Mat idct_image=input_image.clone();
    for (int m = 0; m < height; m += 8)
	{
		for (int n = 0; n < width; n += 8)					
		{
			for (int i = m; i < m + 8; i++)
			{
				for (int j = n; j < n + 8; j++)
				{
					sum = 0.0;
					for (int x = m; x < m + 8; x++)
					{
						for (int y = n; y < n + 8; y++)				
						{
                            double c_i,c_j;
                            if(i==0) c_i= 1/sqrt(2);
                            else c_i=1;
                            if(i==0) c_j= 1/sqrt(2);
                            else c_j=1;   
							sum += (input_image.at<cv::Vec3b>(x, y)[2]) *
								(cos((((2 * x) + 1)) * ((i * pi)) / (2 * 8))) *
								(cos((((2 * y) + 1)) * ((j * pi)) / (2 * 8))) *
								(c_i*c_j);
						}
					}
					idct_image.at<cv::Vec3b>(i, j)[2] = int(sum);				

				}
			}
		}
	}
    return idct_image;
}

cv::Mat ROI(cv::Mat input_image){

    cv::Mat hough_img=input_image.clone();
    for(int i=0; i<input_image.rows; i++){
        for(int j=0; j<input_image.cols; j++){
            hough_img.at<cv::Vec3b>(i, j)[0]= 0.0;
            hough_img.at<cv::Vec3b>(i, j)[1]= 0.0;
            hough_img.at<cv::Vec3b>(i, j)[2]= 0.0;
    }
  }
    for(int i=1;i<input_image.rows-1;i++){
        for(int j=1;j<input_image.cols-1;j++){
            int gx=
            (input_image.at<cv::Vec3b>(i-1,j-1)[2]*(-1)) +
            (input_image.at<cv::Vec3b>(i-1,j)[2]*(0)) +
            (input_image.at<cv::Vec3b>(i-1,j+1)[2]*(1)) +

            (input_image.at<cv::Vec3b>(i,j-1)[2]*(-2)) +
            (input_image.at<cv::Vec3b>(i,j)[2]*(0)) +
            (input_image.at<cv::Vec3b>(i,j+1)[2]*(2)) +

            (input_image.at<cv::Vec3b>(i+1,j-1)[2]*(-1)) +
            (input_image.at<cv::Vec3b>(i+1,j)[2]*(0)) +
            (input_image.at<cv::Vec3b>(i+1,j+1)[2]*(1));
            
            int gy=
            (input_image.at<cv::Vec3b>(i-1,j-1)[2]*(-1)) +
            (input_image.at<cv::Vec3b>(i-1,j)[2]*(-2)) +
            (input_image.at<cv::Vec3b>(i-1,j+1)[2]*(-1)) +

            (input_image.at<cv::Vec3b>(i,j-1)[2]*(0)) +
            (input_image.at<cv::Vec3b>(i,j)[2]*(0)) +
            (input_image.at<cv::Vec3b>(i,j+1)[2]*(0)) +

            (input_image.at<cv::Vec3b>(i+1,j-1)[2]*(1)) +
            (input_image.at<cv::Vec3b>(i+1,j)[2]*(2)) +
            (input_image.at<cv::Vec3b>(i+1,j+1)[2]*(1));

            double g=sqrt(pow(gx,2)+pow(gy,2));
            if(g>255)
                g=255;
            else if(g<0)
                g=0;
            hough_img.at<cv::Vec3b>(i,j)[2]=g;
        }
    }
    return hough_img;
}
cv::Mat RGBtoHSI(cv::Mat input_image){
    double theta=0;
    cv::Mat hsi_image(input_image.rows, input_image.cols, input_image.type());
    double r,g,b,h,s,i;
    for(int p=0;p<input_image.rows;p++)
    {
        for(int q=0;q<input_image.cols;q++){
            r = input_image.at<cv::Vec3b>(p,q)[2];
            g = input_image.at<cv::Vec3b>(p,q)[1];
            b = input_image.at<cv::Vec3b>(p,q)[0];
            i = (r+g+b)/3.0;
            s = 1 - ((3*(min(r,min(g,b))))/(r+g+b));
            theta=acos( ((1/2)*(r-g+r-b))  / (sqrt(pow((r-g),2) + ((r-b)*(g-b)))) );
            if(b<=g)
                h=theta;
            else
                h=360-theta;

            // hsi_image.at<cv::Vec3b>(p,q)[0]= (h * 180) / 3.14159265;
            // hsi_image.at<cv::Vec3b>(p,q)[1]= s*100;
            hsi_image.at<cv::Vec3b>(p,q)[2]= i;
        }
    }

    return hsi_image;
} 

int main(int argc, char** argv){
    vector<int> input_1{10, 11, 12, 11, 12, 13, 12, 11};
    vector<int> input_2{10, -10, 8, -7, 8, -8, 7, -7};
    vector<double> output_1=generateDCT(8,input_1);
    vector<double> output_2=generateDCT(8,input_2);
    input_1.insert(input_1.end(), input_2.begin(), input_2.end());
    vector<double> output_3=generateDCT(16,input_1);
    cout<<"OUTPUT: 8 Point DCT for Input 1"<<endl;
    for(auto v: output_1)
        cout<<v<<" ";
    cout<<"\n\n";
    cout<<"OUTPUT: 8 Point DCT for Input 2"<<endl;
    for(auto v: output_2)
        cout<<v<<" ";
    cout<<"\n\n";
    cout<<"OUTPUT: 16 Point DCT for Input 1"<<endl;
    for(auto v: output_3)
        cout<<v<<" ";

    cv::Mat input_image_1= cv::imread("images/basel3.bmp",cv::IMREAD_COLOR);
    cv::imshow("f1",input_image_1);
    cv::Mat hsi_img=RGBtoHSI(input_image_1);
    cv::imshow("Intensity Image I",hsi_img);
    cv::Mat F_image = DCT_Image(hsi_img);
    cv::imshow("Frequency Image F",F_image);
    cv::Mat D1_image = DCC_component(F_image);
    cv::imshow("Low Frequency Image D1",D1_image);
    cv::Mat D2_image = first9_freq(F_image);
    cv::imshow("First 9 Frequesncy D2 Image",D2_image);
    cv::Mat R1_image = IDCT(D1_image);
    cv::Mat R2_image = IDCT(D2_image);
    cv::imshow("IDCT Image R1", R1_image);
    cv::imshow("IDCT Image R2",R2_image);
    
    cv::Mat input_image_2 = cv::imread("images/Building1.bmp",cv::IMREAD_COLOR);
    cv::Mat input_image_3 = cv::imread("images/Disk.bmp",cv::IMREAD_COLOR);
    cv::Mat hsi_img2 = RGBtoHSI(input_image_2);
    cv::Mat hsi_img3 = RGBtoHSI(input_image_3);
    
    cv::Mat Hough_image2 = ROI(hsi_img2);
    cv::Mat Hough_image3 = ROI(hsi_img3);

    cv::imshow("ROI for Building1",Hough_image2);
    cv::imshow("ROI for Plate Image",Hough_image3);

    cv::waitKey(0);
return 0;
}