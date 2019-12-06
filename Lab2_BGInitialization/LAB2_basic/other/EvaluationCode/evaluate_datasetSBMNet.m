% Script to evaluate foreground segmentation masks using the following
% metrics:(1)Recall, (2)Specificity, (3)FPR, (4)FNR, (5)PBC, (6)Precision, (7)FMeasure.
% 
% Script based on the MATLAB evaluation tool available at
% http://jacarini.dinf.usherbrooke.ca/static/code/MatlabCodeStats2012.zip

% A description of the metrics can be found here
% http://jacarini.dinf.usherbrooke.ca/resultEvaluation/
%
% Steps to use this script:
% 1) Please download one of the two options changedetection dataset 2012
%       a) Full version http://jacarini.dinf.usherbrooke.ca/static/dataset/dataset2012.zip
%       b) Lite version (see moodle course)
%
% 2) For each sequence, process the *all frames* in the directory input.
%   <yourpath>/dataset2012/category/video/input/in000001.jpg to inXXXXXX.jpg
%
%   The result images must be placed in your results directory 
%   (can be anywhere in your system)
%   <yourpath>/results/category/video/outXXXXXX.png
%
%   The result images must follow the same numbering as the input and file
%   format is recommended to be PNG (but not mandatory)
%
%   in000001.jpg (dataset) --> out000001.png (your result)
%   ...
%   inXXXXXX.jpg (dataset) --> outnXXXXXX.png (your result)
%
%   Sample results are provided in the moodle course. In addition, students
%   may download and test this script with any results from 
%   http://jacarini.dinf.usherbrooke.ca/results2012
%
% 3) Call the evaluation script as follows with the following commands
%   
%   $ datasetPath='<yourpath>/dataset2012/dataset';
%   $ resultsPath='<yourpath>/results';
%   $ processFolder(datasetPath, resultsPath)
%
%  In addition to command line output, several TXT files are  generated:
%  - One TXT file for each category 'eval_category.txt' located in the
%    directory <yourpath>/results/category/
%  - One TXT file for overall results 'eval_overall.txt' located in the
%    directory <yourpath>/results/
%
% Author: Juan C. SanMiguel
% Date: February 2019

clear all;
close all;
clc;
%addpath('utils'); %functions used for evaluation

DISPLAY_STATS = 1;

%category_list = {'backgroundMotion', 'basic', 'clutter', 'illuminationChanges', 'intermittentMotion', 'jitter', 'veryLong', 'veryShort'};

    datasetPath = './SBMnet_dataset/data';  %only baseline category
    resultsPath = './SBMnet_dataset/results';       %only baseline category
    UTILITY(datasetPath, resultsPath);


%datasetPath= '../dataset2012/dataset';%full dataset
%resultsPath = '../PAWCS_147/results';%full dataset

%call the evaluation script
