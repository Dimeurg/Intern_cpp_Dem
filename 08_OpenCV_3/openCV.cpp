#include <ctime>
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#ifdef HAVE_OPENCV_XFEATURES2D
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>


using namespace cv;
using namespace cv::xfeatures2d;

enum {
	orb, akaze, kaze 
};

template<typename T>
void find_obj(float _thresh, int action, Ptr<T> detector)
{
	time_t begin = time(NULL);   
	Mat img_object, img_scene;
	
	img_object = imread("object.jpg", IMREAD_GRAYSCALE);
	img_scene = imread("scene.jpg" ,IMREAD_GRAYSCALE);
	
	if(img_object.empty() || img_scene.empty()){
		std::cout << "img isn`t found";
	}
	
	//find keyPoints

	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	Mat descriptors_object, descriptors_scene;
	detector->detectAndCompute(img_object, noArray(),keypoints_object, descriptors_object);
	detector->detectAndCompute(img_scene, noArray(),keypoints_scene, descriptors_scene);
	
	//find matches
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
	std::vector<std::vector<DMatch>> bad_matches;
	matcher->knnMatch(descriptors_object, descriptors_scene, bad_matches,2);
	
	//filter matches
	std::vector<DMatch> good_matches;
	for(size_t i = 0; i < bad_matches.size();i++){
		if(bad_matches[i][0].distance < _thresh * bad_matches[i][1].distance)
			good_matches.push_back(bad_matches[i][0]);	
	}
		
	//draw good matches 
	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches,
	Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	
	//square
	
	std::vector<Point2f> obj, scene;
	
	for(size_t i = 0; i < good_matches.size(); i++){
		obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
	}
	
	int size = scene.size();
	
	Mat H = findHomography(obj,scene,RANSAC  );
	
	std::vector<Point2f> obj_corners(4);
    obj_corners[0] = Point2f(0, 0);
    obj_corners[1] = Point2f( (float)img_object.cols, 0 );
    obj_corners[2] = Point2f( (float)img_object.cols, (float)img_object.rows );
    obj_corners[3] = Point2f( 0, (float)img_object.rows );
	std::vector<Point2f> scene_corners(4);
	
	perspectiveTransform( obj_corners, scene_corners, H);
	
	line( img_matches, scene_corners[0] + Point2f((float)img_object.cols, 0),
          scene_corners[1] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + Point2f((float)img_object.cols, 0),
          scene_corners[2] + Point2f((float)img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + Point2f((float)img_object.cols, 0),
          scene_corners[3] + Point2f((float)img_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + Point2f((float)img_object.cols, 0),
          scene_corners[0] + Point2f((float)img_object.cols, 0), Scalar( 0, 255, 0), 4 );
	int good_keypoints = 0; 
	for(size_t i = 0; i < scene.size();i++){
		if (scene[i].x > scene_corners[0].x && scene[i].x < scene_corners[1].x
		&& scene[i].y > scene_corners[0].y && scene[i].y < scene_corners[2].y)
			good_keypoints++;
	}
	
	std::string win, file_save;
	switch(action){
		case orb:
		win = "orb";
		file_save = "orb.jpg";
		std::cout << "orb: \n";
		break;
		
		case akaze:
		win = "akaze";
		file_save = "akaze.jpg";
		std::cout << "akaze: \n";
		break;
		
		case kaze:
		win = "kaze";
		file_save = "kaze.jpg";
		std::cout << "kaze: \n";
		break;
	}
	
	imshow(win, img_matches);
	imwrite(file_save, img_matches);	
	
	std::cout << "time: " <<time(NULL) - begin << std::endl;
	std::cout << "good_keypoints: " << good_keypoints << "percent: " << good_keypoints *100 / size  << " %\n";
}


int main()
{
	
		
	Ptr<FeatureDetector> detector1 = ORB::create();
	find_obj(0.9, orb, detector1);
	
	Ptr<FeatureDetector> detector2 = AKAZE::create();
	find_obj(5, akaze, detector2);
	
	Ptr<FeatureDetector> detector3 = KAZE::create();
	find_obj(5, kaze, detector3);
	cv::waitKey(0);
}
#else
int main(){
	std::cout << "need xfeatures2d\n";
	std::cin.get();
	return 0; 	
}
#endif