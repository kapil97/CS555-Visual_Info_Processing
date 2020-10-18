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

int main(int argc, char** argv){
    vector<int> input_1{10, 11, 12, 11, 12, 13, 12, 11};
    vector<int> input_2{10, -10, 8, -7, 8, -8, 7, -7};
    vector<double> output_1=generateDCT(8,input_1);
    vector<double> output_2=generateDCT(8,input_2);
    for(auto v: output_1)
        cout<<v<<" ";
    cout<<"\n";
    for(auto v: output_2)
        cout<<v<<" ";
return 0;
}