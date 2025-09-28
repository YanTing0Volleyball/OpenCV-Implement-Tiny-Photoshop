#include "ImageFilter.h"
#define PI 3.14159265358979323846



cv::Mat GaussianFilter::Filter(const cv::Mat& img)
{
	int radius = (int)ceil(2 * spatialSigma);

	cv::Mat fltImg = cv::Mat(img.size(), img.type());

	int kernelSize = 2 * radius + 1;

	//Use OpenCV built-in function boxFilter Running Time: less than 1 second
	std::vector<std::vector<double>>v_filter;  //v_filter => 2D vector to builde up customized filter
	double sumWeight = 0;
	
	//build up filter
	for (int i = 0; i < kernelSize; i++) {
		std::vector <double> temp;
		for (int j = 0; j < kernelSize; j++) {
			//Gaussian filter formula
			double weight = exp(-((radius - i) * (radius - i) + (radius - j) * (radius - j)) / (2 * spatialSigma * spatialSigma)) / (2 * PI * spatialSigma * spatialSigma);
			temp.push_back(weight);
			sumWeight += weight;
		}
		v_filter.push_back(temp);
	}
	cv::Mat s_filter(v_filter.size(), v_filter[0].size(), CV_64FC1);  //translate filter from vector form to Mat form
	for (int i = 0; i < v_filter.size(); ++i) {
		for (int j = 0; j < v_filter[0].size(); ++j) {
			s_filter.at<double>(i, j) = v_filter[i][j] / sumWeight;
		}
	}
	cv::filter2D(img, fltImg, -1, s_filter, cv::Point(-1, -1), false, cv::BORDER_REPLICATE);  //OpenCV built-in function
	//filter2D(input image, output image, output image depth, filter, kernel position, delta(can increase the final calculated value), border dealing method)
	//BORDER_REPLICATE => extend border value


	/*
	//Depart spaitlaSigma into Xsigma and Ysigma, create two one-D filter go through all pixels. Running Time: about 7~8 seconds
	double oneSigma = 0.3 * ((kernelSize - 1) * 0.5 - 1) + 0.8;
	double oneWeights[30];  //oneWeights => represent the 1D Gaussian filter
	double weightSum = 0;
	for (int i = 0; i < kernelSize; ++i) {
		double temp = exp(-(i - (kernelSize - 1) / 2) * (i - (kernelSize - 1) / 2) / (2 * oneSigma * oneSigma));  //1D Gaussian formula
		oneWeights[i] = temp;
		weightSum += temp;
	}
	double alpha = 1 / weightSum;
	for (int i = 0; i < kernelSize; i++) {
		oneWeights[i] *= alpha;
	}

	//1D Gaussian filter go through all pixel
	for (int h = 0; h < img.rows; ++h) {
		for (int w = 0; w < img.cols; ++w) {
			cv::Vec3d colorSum = cv::Vec3d(0, 0, 0);
			for (int dx = -radius; dx <= radius; ++dx) {
				for (int dy = -radius; dy <= radius; ++dy) {
					int x = w + dx;
					int y = h + dy;
					if (x < 0 || x >= img.cols || y < 0 || y >= img.rows) continue;
					colorSum += img.at<cv::Vec3b>(y, x) * oneWeights[dy + radius] * oneWeights[dx + radius];
				}
			}
			fltImg.at<cv::Vec3b>(h, w) = colorSum;
		}
	}
	*/
	

	/*
	//Create filter at first, let filter go throug all pixels. Running Time: about 5~7seconds
	double down = 2 * spatialSigma * spatialSigma * PI;
	double distanceMatrix[100][100];  //distanceMatrix => represent a pre-calculate 2D Gaussian filter
	
	//build up Gaussian filter
	for (int i = 0; i < kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) distanceMatrix[i][j] = exp(-((radius - i) * (radius - i) + (radius - j) * (radius - j)) / (2 * spatialSigma * spatialSigma)) / (2 * PI * spatialSigma * spatialSigma); //2D Gaussian formula
	}

	//Gaussian filter go through all pixel
	for (int h = 0; h < img.rows; ++h) {
		for (int w = 0; w < img.cols; ++w) {
			cv::Vec3d colorSum = cv::Vec3d(0, 0, 0);
			for (int dx = -radius; dx <= radius; ++dx) {
				for (int dy = -radius; dy <= radius; ++dy) {
					int x = w + dx;
					int y = h + dy;
					if (x < 0 || x >= img.cols || y < 0 || y >= img.rows) continue;
					colorSum += img.at<cv::Vec3b>(y, x) * distanceMatrix[dy + radius][dx + radius];
				}
			}
			fltImg.at<cv::Vec3b>(h, w) = colorSum;
		}
	}
	*/
	
	
	
	/*
	//Dirtectly iterate all pixels. Running Time: about 7~10seconds
	//Gaussian formula go through all pixel
	for (int h = 0; h < fltImg.rows; ++h) {
		for (int w = 0; w < fltImg.cols; ++w) {
			cv::Vec3d sumColor = cv::Vec3d(0, 0, 0);
			for (int dx = -radius; dx <= radius; ++dx) {
				for (int dy = -radius; dy <= radius; ++dy) {
					int x = w + dx;
					int y = h + dy;
					if (x < 0 || x >= fltImg.cols || y < 0 || y >= fltImg.rows) continue;
					cv::Vec3b oColor = img.at<cv::Vec3b>(y, x);
					double weight = exp(-(dx * dx + dy * dy) / (2 * spatialSigma * spatialSigma)) / (2 * PI * spatialSigma * spatialSigma); //2D Gaussian formula
					sumColor += oColor * weight;
				}
			}
			fltImg.at<cv::Vec3b>(h, w) = sumColor;
		}
	}
	*/
	
	//cv::GaussianBlur(img, fltImg, cv::Size(kernelSize, kernelSize), spatialSigma);
	

	return fltImg;
}


cv::Mat BilateralFilter::Filter(const cv::Mat& img)
{
	int radius = (int)ceil(2 * spatialSigma);
		
	cv::Mat fltImg = cv::Mat(img.size(), img.type());
	
	//Directly iterate all pixels. Running Time: about 9~10seconds
	for (int h = 0; h < fltImg.rows; ++h) {
		for (int w = 0; w < fltImg.cols; ++w) {
			cv::Vec3d sumColor = cv::Vec3d(0, 0, 0);  //record the sum of the value of the Color
			double sumWeight = 0;  // record the sum of weight
			for (int dy = -radius; dy <= radius; ++dy) {
				for (int dx = -radius; dx <= radius; ++dx) {
					int x = w + dx;
					int y = h + dy;
					if (x < 0 || x >= fltImg.cols || y < 0 || y >= fltImg.rows) continue;
					cv::Vec3b centerColor = img.at<cv::Vec3b>(h, w);
					cv::Vec3b sampleColor = img.at<cv::Vec3b>(y, x);
					//calculate the value of weight in sparse
					double sparseWeight = exp(-(dx * dx + dy * dy) / (2 * spatialSigma * spatialSigma));
					//calculate the value of weight in range
					double rangeWeight = exp(-(((centerColor[0] - sampleColor[0]) * (centerColor[0] - sampleColor[0])) + ((centerColor[1] - sampleColor[1]) * (centerColor[1] - sampleColor[1])) + ((centerColor[2] - sampleColor[2]) * (centerColor[2] - sampleColor[2]))) / (2 * rangeSigma * rangeSigma));
					sumColor += sparseWeight * rangeWeight * sampleColor;
					sumWeight += sparseWeight * rangeWeight;
				}
			}
			fltImg.at<cv::Vec3b>(h, w) = sumColor / sumWeight;
		}
	}
	
	//fltImg = img.clone();
	
	return fltImg;
}

