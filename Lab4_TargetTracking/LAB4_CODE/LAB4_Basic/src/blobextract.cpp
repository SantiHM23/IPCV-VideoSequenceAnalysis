/* Applied Video Analysis of Sequences (AVSA)
 *
 * Blob-based functions for extraction and painting
 *
 * Author: Juan C. SanMiguel (juancarlos.sanmiguel@uam.es)
 */
#include "blobfuns.hpp"

#define MIN_WIDTH 5
#define MIN_HEIGHT 5

/**
 *	Blob extraction from 1-channel image (binary). The extraction is performed based
 *	on the analysis of the connected components. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param fgmask Foreground/Background segmentation mask (1-channel binary image) 
 * \param bloblist List with found blobs
 *
 * \return Operation code (negative if not succesfull operation) 
 */
typedef struct PIXEL_ANALYSIS_TAG
{
	int cx;
	int cy;
}PIXEL_ANALYSIS_T;

/* GLobal variables*/
cv::Mat global_fgmask;
//cv::Mat label_mask;
PIXEL_ANALYSIS_T fg_pixel;
std::vector<PIXEL_ANALYSIS_T> pix_list;
int blob_counter = 0;

/*Uses global variables*/
cvBlob check_neighbours(void);


int extractBlobs(cv::Mat fgmask, std::vector<cvBlob> &bloblist)		
{
	fgmask.copyTo(global_fgmask);
	//fgmask.copyTo(label_mask);
	//label_mask = 0;
	std::vector<PIXEL_ANALYSIS_T>().swap(pix_list);
	blob_counter = 0;
		
	//scan the FG mask to find blob pixels (use one of the two following choices)
	// choice 1 "linear positioning"
	//			for (int i=0;i<fgmask.size;i++)"
	
	// choice 2 "2d positioning"
	//			for (int x=0;x<fgmask.width;x++)
	//				for (int y=0;y<fgmask.height;y++)
	for (int x=0;x<global_fgmask.rows;x++)
	{
		for (int y=0;y<global_fgmask.cols;y++)
		{
			//extract connected component (blob)
			//...

			if(255 == global_fgmask.at<uchar>(x,y))
			{
				/***   New blob (white pixel) found  ***/
				/* Save position and call neighbor analysis function */

				fg_pixel.cx = x;
				fg_pixel.cy = y;
				pix_list.push_back(fg_pixel);
				cvBlob new_blob = check_neighbours();
				/* Add blob only if fulfills minimum dimensions */
				if(MIN_WIDTH  < new_blob.w && MIN_HEIGHT<new_blob.h)
				{
					blob_counter ++;
					new_blob.ID = blob_counter;
					bloblist.push_back(new_blob);
				}

			}
		}

	}

	//return OK code
	return 1;
}

cvBlob check_neighbours(void)
{
	PIXEL_ANALYSIS_T max_coord = pix_list.back();
	PIXEL_ANALYSIS_T min_coord = pix_list.back();
	cvBlob blob_found={};

	while (!pix_list.empty())
	{
		fg_pixel= pix_list.back();
		//std::cout << " x value " << fg_pixel.cx << ", y value " << fg_pixel.cy << std::endl;
		pix_list.pop_back();
		global_fgmask.at<uchar>(fg_pixel.cx,fg_pixel.cy) = 0;
		//std::cout << "value at x y = " << (int)global_fgmask.at<uchar>(fg_pixel.cx,fg_pixel.cy) << std::endl;
		//label_mask.at<uchar>(fg_pixel.cx,fg_pixel.cy) = blob_counter;

		/*Update min and max blob pixels coordinates*/
		if(fg_pixel.cx < min_coord.cx)
		{
			min_coord.cx = fg_pixel.cx;
		}
		if(fg_pixel.cy < min_coord.cy)
		{
			min_coord.cy = fg_pixel.cy;
		}
		if(fg_pixel.cx > max_coord.cx)
		{
			max_coord.cx = fg_pixel.cx;
		}
		if(fg_pixel.cy > max_coord.cy)
		{
			max_coord.cy = fg_pixel.cy;
		}


		/*check neighbors four directions*/
		int original_x = fg_pixel.cx;
		int original_y = fg_pixel.cy;
		if((original_x-1) > 0 )
		{
			if(global_fgmask.at<uchar>(original_x-1,original_y) == 255)
			{
				//std::cout <<  "1 " << std::endl;
				fg_pixel.cx=original_x-1;
				fg_pixel.cy=original_y;
				pix_list.push_back(fg_pixel);
			}
		}
		if((original_y-1) > 0 )
		{
			if(global_fgmask.at<uchar>(original_x,original_y-1) == 255)
			{
				//std::cout <<  "2 " << std::endl;
				fg_pixel.cx=original_x;
				fg_pixel.cy=original_y-1;
				pix_list.push_back(fg_pixel);
			}
		}
		if((original_x+1) < global_fgmask.rows )
		{
			if(global_fgmask.at<uchar>(original_x+1,original_y) == 255)
			{
				//std::cout <<  "3 " << std::endl;
				fg_pixel.cx=original_x+1;
				fg_pixel.cy=original_y;
				pix_list.push_back(fg_pixel);
			}
		}
		if((original_y+1) < global_fgmask.cols )
		{
			if(global_fgmask.at<uchar>(original_x,original_y+1) == 255)
			{
				//std::cout <<  "4 " << std::endl;
				fg_pixel.cx=original_x;
				fg_pixel.cy=original_y+1;
				pix_list.push_back(fg_pixel);
			}
		}

	}


	//changing because it was wrong
	blob_found.y = (min_coord.cx);
	blob_found.x = (min_coord.cy);
	blob_found.h = (max_coord.cx-min_coord.cx);
	blob_found.w = (max_coord.cy-min_coord.cy);
	blob_found.x_c = blob_found.x + floor(blob_found.h/2);
	blob_found.y_c = blob_found.y + floor(blob_found.w/2);

	return blob_found;
}

/**
 *	Draws blobs with different rectangles on the image 'frame'. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param frame Input image 
 * \param pBlobList List to store the blobs found 
 *
 * \return Image containing the draw blobs. If no blobs have to be painted 
 *  or arguments are wrong, the function returns a copy of the original "frame".  
 *
 */
 Mat paintBlobImage(cv::Mat frame, std::vector<cvBlob> &bloblist)	
{
	cv::Mat blobImage;
	frame.copyTo(blobImage);
	if(!bloblist.empty())
	{

		for(uint i = 0; i < bloblist.size(); i++)
		{
			cvBlob blob = bloblist[i]; //get ith blob
			cv::rectangle( blobImage, cv::Point2f(blob.y,blob.x ), cv::Point2f( (blob.y+blob.h),(blob.x+blob.w)), cv::Scalar( 0,0,255 ),3 );
		}
	}

	
	//return the image to show
	return blobImage;
}
