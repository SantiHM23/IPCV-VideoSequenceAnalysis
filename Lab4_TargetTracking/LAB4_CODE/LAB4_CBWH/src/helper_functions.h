/*
 * helper_functions.h
 *
 *  Created on: May 1, 2019
 *      Author: daniel
 */

#ifndef LAB4_MEANSHIFT_SRC_HELPER_FUNCTIONS_H_
#define LAB4_MEANSHIFT_SRC_HELPER_FUNCTIONS_H_

#include <opencv2/opencv.hpp> //opencv libraries
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>



#define drawCross( frame, center, color, d ) \
line( frame, Point( center.x - d, center.y - d ),Point( center.x + d, center.y + d ), color, 1, LINE_AA, 0); \
line( frame, Point( center.x + d, center.y - d ),Point( center.x - d, center.y + d ), color, 1, LINE_AA, 0 )


typedef struct Object_Location_Tag
{
	cv::Point2f center;
	/*Bounding Box*/
	int x_tl; /* top left x */
	int y_tl; /* top left y */
	/*Center*/
	int x_c;  /* center x*/
	int y_c;  /* center y*/
	int w; /* width*/
	int h; /*height*/
}Object_Location_T;

Object_Location_T Read_First_Line_Ground_Truth(std::string ground_truth_file);


#endif /* LAB4_MEANSHIFT_SRC_HELPER_FUNCTIONS_H_ */
