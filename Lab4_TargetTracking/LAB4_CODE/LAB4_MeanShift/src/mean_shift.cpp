/*
 * mean_shift.cpp
 *
 *  Created on: May 4, 2019
 *      Author: daniel
 */

#include "mean_shift.h"


Mat Compute_PDF(Mat frame,Rect myROI)
{
	/// Establish the number of bins
	int histSize = 256;
	Mat croppedImage = frame(myROI);

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	Mat hist_model;
	calcHist( &croppedImage, 1, 0, Mat(), hist_model, 1, &histSize, &histRange, true, false );
	/*Normalize histogram - sum of elements equals 1*/
	normalize(hist_model, hist_model, 1.0, 0.0, NORM_L1);
	return hist_model;

}

Mat Compute_Weights(Mat Frame, Mat target_PDF,Mat Candidate_PDF, Rect ROI)
{
    int col_index = ROI.x;
    int row_index = ROI.y;

    int rows = ROI.height;
    int cols = ROI.width;

	Mat weight(rows,cols,CV_32F);

	for(int i=0;i<rows;i++)
	{
		col_index = ROI.x;
		for(int j=0;j<cols;j++)
		{
			int pixel = (Frame.at<uchar>(row_index,col_index));
			weight.at<float>(i,j) = (float)((sqrt(target_PDF.at<float>(pixel)/Candidate_PDF.at<float>(pixel))));
			col_index++;
		}
		row_index++;
	}


	return weight;
}

Point2f Compute_New_Center(Rect myROI,Mat weight)
{
	Point2f center(myROI.x+floor((myROI.width)/2),myROI.y+floor((myROI.height)/2));

	float delta_x = 0.0;
	float delta_y = 0.0;
	float sum_wij = 0.0;
	int mult = 1;
	float centrex = (float)((weight.cols-1)/2.0);
	float centrey = (float)((weight.rows-1)/2.0);

	int counter = 0;
	for(int i=0;i<weight.rows;i++)
	{
		for(int j=0;j<weight.cols;j++)
		{
			float norm_i = (float)((float)i-centrey)/centrey;
			float norm_j = (float)((float)j-centrex)/centrex;
			mult = 0;
			/*Consider only inner circle*/
			if((pow(norm_i,2)+pow(norm_j,2)) >1)
			{
				mult = 1;
			}
			delta_x += (float)(norm_j*weight.at<float>(i,j)*mult);
			delta_y += (float)(norm_i*weight.at<float>(i,j)*mult);
			sum_wij += (float)(weight.at<float>(i,j)*mult);
			counter++;
		}
	}

	center.x += (int)((delta_x/sum_wij)*centrex);
	center.y += (int)((delta_y/sum_wij)*centrey);


	return center;
}
