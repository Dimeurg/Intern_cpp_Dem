#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/gui_widgets.h>

#include <cmath>                                   
#define PI 3.14159265  

using namespace dlib;

int main(){
	try{
		
		cv::VideoCapture cap(0);
		if(!cap.isOpened()){
			std::cerr << "Unable to connect to camera" << std::endl;
			return 1;
		}
		
		image_window win;
		frontal_face_detector detector = get_frontal_face_detector();
		
		shape_predictor pose_model;
		deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;
		
		// 3D model points.
		std::vector<cv::Point3d> model_points;
		model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
		model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
		model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
		model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
		model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
		model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner
		
		while(!win.is_closed()){
			cv::Mat temp_img;
			
			if(!cap.read(temp_img)){
				break;
			}
						
			cv_image<bgr_pixel> img(temp_img);
			
			std::vector<rectangle> faces = detector(img);
			std::vector<full_object_detection> shapes;
			for(unsigned long i = 0; i < faces.size(); i++){
				shapes.push_back(pose_model(img, faces[i]));
			}

			if(!shapes.empty()){
							
				// Camera internals
				double focal_length = temp_img.cols; // Approximate focal length.
				cv::Point2d center = cv::Point2d(temp_img.cols/2,temp_img.rows/2);
				cv::Mat camera_matrix = (cv::Mat_<double>(3,3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
				cv::Mat dist_coeffs = cv::Mat::zeros(4,1,cv::DataType<double>::type); // Assuming no lens distortion
	
				std::vector<cv::Point2d> image_points;
				image_points.push_back( cv::Point2d(shapes[0].part(30).x(), shapes[0].part(30).y()) );   // Nose tip
				image_points.push_back( cv::Point2d(shapes[0].part(8).x(), shapes[0].part(8).y()) );   // Chin
				image_points.push_back( cv::Point2d(shapes[0].part(45).x(), shapes[0].part(45).y()) );   // Left eye left corner
				image_points.push_back( cv::Point2d(shapes[0].part(36).x(), shapes[0].part(36).y()) );   // Right eye right corner
				image_points.push_back( cv::Point2d(shapes[0].part(54).x(), shapes[0].part(54).y()) );   // Left Mouth corner
				image_points.push_back( cv::Point2d(shapes[0].part(48).x(), shapes[0].part(48).y()) );   // Right mouth corner
				
				
				// Output rotation and translation
				cv::Mat rotation_vector; // Rotation in axis-angle form
				cv::Mat translation_vector;
				
				// Solve for pose
				cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
				
				// Project a 3D point (0, 0, 1000.0) onto the image plane.
				// We use this to draw a line sticking out of the nose
				
				std::vector<cv::Point3d> nose_end_point3D;
				std::vector<cv::Point2d> nose_end_point2D;
				nose_end_point3D.push_back(cv::Point3d(0,0,1000.0));
				
				projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);			
				
				cv::line(temp_img,image_points[0], nose_end_point2D[0], cv::Scalar(255,0,0), 2);
			
				char side;
				int deltaLeft = image_points[0].x - image_points[2].x;
				int deltaRight = image_points[3].x - image_points[0].x;
				int face_center = (deltaLeft + deltaRight)/2;
				int angle;
				
				if(deltaLeft > deltaRight){
					side = 'R';
					angle =	(90* deltaRight / face_center) -90;
				}
				else {
					side = 'L';
					angle = (90* deltaLeft / face_center) -90;
				}
				
				char buf[20];
				sprintf(buf, "%c: %d", side,angle );				
				cv::putText(temp_img, buf, image_points[0], 0, 2, cv::Scalar(255, 0, 255), 2);
				
				if(side == 'L' && angle > 45){
					system("notepad.exe");
					cv::waitKey(10);
				}
			}
			
			cv_image<bgr_pixel> img_res(temp_img);
			win.clear_overlay();
			win.set_image(img_res);
			win.add_overlay(render_face_detections(shapes));
		}
		
	}
	
	catch(serialization_error & e){
		std::cout << "you need defauld landmarking model" << std::endl;
		std::cout << e.what() << std::endl;
	}
	
	catch(std::exception & e){
		std::cout << e.what() << std::endl;
	}
	
	std::cin.get();
}