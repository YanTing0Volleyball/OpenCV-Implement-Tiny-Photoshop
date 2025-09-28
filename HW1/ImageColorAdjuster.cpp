#include "ImageColorAdjuster.h"
#include<iostream>
using namespace std;


cv::Mat ImageColorAdjuster::RemapColorCurve1(const cv::Mat& image)
{
	cv::Mat newImage = cv::Mat(image.size(), image.type());

	int width  = image.cols;  //the width of image
	int height = image.rows;  //the height of image
	
	//go through all pixel to update information in pixel
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			//Get pixel color
			cv::Vec3d oColor = image.at<cv::Vec3b>(h, w);
			double B = oColor[0], G = oColor[1], R = oColor[2];  //B,G,R represent the original color value, Blue, Green, Red
			//according to the slide translate RGB to YCbCr
			double Y = 0.299 * R + 0.587 * G + 0.114 * B;
			double Cb = 0.568 * (B - Y) + 128;
			double Cr = 0.713 * (R - Y) + 128;
			// Scale the luminance channel.
			// Y' = Y^{0.85}.
			Y /= 255;   //normalization  (make the value of Y between 0 and 1)
			Y = pow(Y, 0.85);
			Y *= 255;  //denormalization
			
			// YCbCr to RGB.
			R = Y + 1.371 * (Cr - 128);
			G = Y - 0.698 * (Cr - 128) - 0.336 * (Cb - 128);
			B = Y + 1.732 * (Cb - 128);
			// Write the new color to newImage.
			newImage.at<cv::Vec3b>(h, w) = cv::Vec3d(B, G, R);
		}
	}
	return newImage;
}


cv::Mat ImageColorAdjuster::RemapColorCurve2(const cv::Mat& image)
{
	cv::Mat newImage = cv::Mat(image.size(), image.type());
	
	int width  = image.cols;  //the width of image
	int height = image.rows;  //the height of image


	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			// Get pixel color.
			cv::Vec3d oColor = image.at<cv::Vec3b>(h, w);
			double B = oColor[0], G = oColor[1], R = oColor[2];  //B,G,R represent the original color value, Blue, Green, Red
			// RGB to YCbCr (please use the formula on slides).
			double Y = 0.299 * R + 0.587 * G + 0.114 * B;
			double Cb = 0.568 * (B - Y) + 128;
			double Cr = 0.713 * (R - Y) + 128;
			// Scale the luminance channel.
			// Y' = Y^{1.50}.
			Y /= 255;  //normalization (make the value of Y between 0 and 1)
			Y = pow(Y, 1.5);
			Y *= 255;  //denormalization
			
			
			// YCbCr to RGB.
			R = Y + 1.371 * (Cr - 128);
			G = Y - 0.698 * (Cr - 128) - 0.336 * (Cb - 128);
			B = Y + 1.732 * (Cb - 128);
			// Write the new color to newImage.
			newImage.at<cv::Vec3b>(h, w) = cv::Vec3d(B, G, R);
		}
	}
	return newImage;
}