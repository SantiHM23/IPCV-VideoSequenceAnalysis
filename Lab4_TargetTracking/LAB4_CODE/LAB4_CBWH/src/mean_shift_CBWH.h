/*
 * mean_shift_CBWH.h
 *
 *  Created on: May 4, 2019
 */

#ifndef LAB4_MEANSHIFT_SRC_MEAN_SHIFT_CBWH_H_
#define LAB4_MEANSHIFT_SRC_MEAN_SHIFT_CBWH_H_

#include <opencv2/opencv.hpp> //opencv libraries

#include <opencv2/features2d.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui.hpp"

#define BINS_NUMB  (32)
#define BIN_WIDTH  (256/BINS_NUMB)


using namespace cv;

Mat Compute_PDF(Mat frame,Rect myROI);
Mat Compute_Bkg_PDF(Mat frame,Rect bkg_ROI, Rect myROI);
Mat Compute_Weights(Mat Frame, Mat target_PDF,Mat Candidate_PDF, Rect ROI,Mat V_u);
Mat Get_V_Min_Value(Mat O_u);
Point2f Compute_New_Center(Rect myROI,Mat weight);



#endif /* LAB4_MEANSHIFT_SRC_MEAN_SHIFT_CBWH_H_ */
