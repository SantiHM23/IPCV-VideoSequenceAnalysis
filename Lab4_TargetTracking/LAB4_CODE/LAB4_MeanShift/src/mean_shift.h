/*
 * mean_shift.h
 *
 *  Created on: May 4, 2019
 *      Author: daniel
 */

#ifndef LAB4_MEANSHIFT_SRC_MEAN_SHIFT_H_
#define LAB4_MEANSHIFT_SRC_MEAN_SHIFT_H_

#include <opencv2/opencv.hpp> //opencv libraries

#include <opencv2/features2d.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui.hpp"


using namespace cv;

Mat Compute_PDF(Mat frame,Rect myROI);
Mat Compute_Weights(Mat Frame, Mat target_PDF,Mat Candidate_PDF, Rect ROI);
Point2f Compute_New_Center(Rect myROI,Mat weight);



#endif /* LAB4_MEANSHIFT_SRC_MEAN_SHIFT_H_ */
