/* Applied Video Analysis of Sequences (AVSA)
 *
 * Headers of blob-based functions
 *
 * Author: Juan C. SanMiguel (juancarlos.sanmiguel@uam.es)
 */

#ifndef BLOBFUNS_H_INCLUDE
#define BLOBFUNS_H_INCLUDE

#include <iostream>
#include "basicblob.hpp"

//blob extraction functions (included in 'blobextract.cpp')
int extractBlobs(Mat fgmask, std::vector<cvBlob> &bloblist);
Mat paintBlobImage(Mat frame, std::vector<cvBlob> &bloblist);

//blob classification functions (included in 'blobclassify.cpp')
int classifyBlobs(Mat frame, Mat fgmask, std::vector<cvBlob> &bloblist);
Mat paintBlobClasses(Mat frame, std::vector<cvBlob> &bloblist);	

//stationary blob extraction functions
int extractStationaryFG (Mat fgmask, Mat &fgmask_counter, Mat &sfgmask);
int detectBlobStationary (Mat frame, Mat sfgmask, std::vector<cvBlob> &sbloblist);

//abandoned/stolen classification
int classifyBlobStationary (Mat frame, Mat bkg, Mat sfgmask, std::vector<cvBlob> &sbloblist, std::vector<cvBlob> &sbloblistObj);
/*Function to compute the gradient of an image*/
Mat Get_Gradient_Image(Mat source_image);
/*Function to paint stationaty blobs*/
cv::Mat paintStationaryClasses(cv::Mat frame, std::vector<cvBlob> &sbloblist);

#endif
