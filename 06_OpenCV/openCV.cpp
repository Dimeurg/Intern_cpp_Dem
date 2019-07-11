#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

int main(int argc,char** argv)
{
	cv::Point p;
	cv::Mat frame, changeFrame;
	cv::VideoCapture cam;
	cv::Scalar color(255, 255, 255);

	cam.open(cv::CAP_ANY);
	
	if(!cam.isOpened())
	{
		std::cout << "Couldn`t open cam \n";
		return -1;
	}

	while(cam.isOpened()){
		cam.read(frame);
				
		cv::cvtColor(frame, changeFrame, cv::COLOR_BGR2GRAY);
		std::string s = "some text";
		
		int baseline = 0;
		cv::Size textSize = cv::getTextSize(s, cv::FONT_HERSHEY_SIMPLEX, 1, 0, &baseline);
		baseline++;
		
		p.x = changeFrame.cols - textSize.width;
		p.y = textSize.height;
		cv::putText(changeFrame, s, p, cv::FONT_HERSHEY_SIMPLEX, 1, color, 0, cv::LINE_AA);
		cv::imshow("window", changeFrame);
		
		std::vector<int> v;
		switch(cv::waitKey(10))	{
			case 32:
			//space
			cv::imwrite("camshot.png", changeFrame, v);
			break;
			case 27:
			//esc
			
			cam.release();
			break;
		}
	}
	
	return 0;
}