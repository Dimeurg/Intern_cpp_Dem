#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>


int tresh_callback(cv::Mat & img_gray, int thresh)
{
	cv::Mat canny_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	
	cv::Canny(img_gray, canny_output, thresh, thresh*2, 3);
	cv::findContours(canny_output,contours,hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,cv::Point(0,0));
	
	cv::Mat draw = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	
	int result_angle = 0;
	int area_max = 0;
	for(int i = 0; i < contours.size(); i++){
	cv::RotatedRect rRect = cv::minAreaRect(contours[i]);
	cv::Rect rect = rRect.boundingRect();

	cv::Point2d	center = rRect.center;
	int area = int(rect.width * rect.height);
	int angle = rRect.angle;
	
		if(area > 1000){
			if(area > area_max)	{
				area_max = area;
				result_angle = angle;
			}
			
			cv::Scalar color = cv::Scalar(200,200,200);
			cv::drawContours(draw, contours, i, color, 2,8,hierarchy,0, cv::Point());

			char buf[5];
			sprintf (buf, "%d", angle);
			std::string s = buf;
			
			cv::putText(draw, s, center, cv::FONT_HERSHEY_SIMPLEX, 1, color, 0, cv::LINE_AA);
		}
	}
	
	if(!draw.empty()){
		cv::imshow("contours", draw);	
		cv::imwrite("contours.jpg", draw);
	}
	
	return result_angle;
}

int main(int argc,char** argv)
{
	cv::Mat img,img_gray, rotatedImg;
	
	img = cv::imread("book.jpg");
	
	if(img.empty())	{
		std::cout << "Couldn`t open img \n";
		return -1;
	}
	
	cv::cvtColor(img,img_gray,CV_BGR2GRAY);
	cv::blur(img_gray, img_gray, cv::Size(3,3));	
	
	int angle = tresh_callback(img_gray,200);
	angle += 90;
	
	cv::Point2f center(img.cols/2., img.rows/2.);
    cv::Mat r = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(img, rotatedImg, r, rotatedImg.size());
	
	cv::imwrite("rotatedImg.jpg", rotatedImg);
	imshow("win", rotatedImg);
	cv::waitKey(0);
	return 0;
}

