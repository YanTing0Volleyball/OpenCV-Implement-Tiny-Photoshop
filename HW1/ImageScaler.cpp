#include "ImageScaler.h"

#include <iostream>
using namespace std;


cv::Mat ImageScaler::Resize(const cv::Mat &origImg, const int newWidth, const int newHeight)
{
	cv::Size newSize(newWidth, newHeight);
	cv::Mat editImg = cv::Mat(newSize, origImg.type());
	
	//Calculate the scaling rate in height and width
	float hScaling = (float)newHeight / origImg.rows;  
	float wScaling = (float)newWidth / origImg.cols;

	//ox, oy => original image coordinate (ox: 0 ~ Width-1 oy: 0 ~ Hight - 1)
	//a, b => stand for line segment ratio
	float ox, oy, a, b;
	cv::Vec3b nColor;
	
	//according to bilinear interpolation to calcualte all scalling pixel
	for (int nWidth = 0; nWidth < newWidth; ++nWidth) {
		for (int nHeight = 0; nHeight < newHeight; ++nHeight) {
			ox = nWidth / wScaling;
			oy = nHeight / hScaling;
			a = ox - (float)nWidth / wScaling;
			b = oy - (float)nHeight / hScaling;
			//three situation from new pixel to original image
			if (a == 0 && b == 0) nColor = origImg.at<cv::Vec3b>((int)oy, (int)ox);  //at the point 
			else if (a == 0 || b == 0) {  //on the edge
				if (a == 0) nColor = origImg.at<cv::Vec3b>((int)oy, (int)ox) * (1 - b) + origImg.at<cv::Vec3b>((int)oy + 1, (int)ox) * b;
				if (b == 0) nColor = origImg.at<cv::Vec3b>((int)oy, (int)ox) * (1 - a) + origImg.at<cv::Vec3b>((int)oy, (int)ox + 1) * a;
				
			}
			else {  //in the square
				nColor = origImg.at<cv::Vec3b>((int)oy, (int)ox) * (1 - a) * (1 - b) +
						 origImg.at<cv::Vec3b>((int)oy, (int)ox + 1) * a * (1 - b) +
						 origImg.at<cv::Vec3b>((int)oy + 1, (int)ox + 1) * a * b +
						 origImg.at<cv::Vec3b>((int)oy + 1, (int)ox) * (1 - a) * b;
			}
			editImg.at<cv::Vec3b>(nHeight, nWidth) = nColor;   //add calculated pixel to scalling imgae
		}
	}
	
	return editImg;
}

