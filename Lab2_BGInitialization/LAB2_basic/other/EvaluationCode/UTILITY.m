function UTILITY(GT_path, evaluated_path)
%function to evaulate the your resutls for SBMnet dataset, this code will
%generate a 'cm.txt' file in your result path to save all the metrics.
%input: GT_path: the path of the groundtruth folder.
%       evaluated_path: the path of your evaluated results folder.

%%%%%%%%%%%%%%%%%%%%%%%%%%%input%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%GT_path = 'C:\Project\SBMIweb\dataset\dataset\full_dataset\';
%evaluated_path = 'C:\Project\SBMIweb\result\testing\median_filter\results\';

category_list ={'backgroundMotion', 'basic', 'clutter', 'illuminationChanges', 'intermittentMotion',...
                'jitter', 'veryLong'};%, 'veryShort'};
category_num = length(category_list);

fid = fopen(fullfile(evaluated_path, 'cm.txt'), 'a+');            
fprintf(fid, '\t\tvideo\tAGE\tpEPs\tpCEPs\tMSSSIM\tPSNR\tCQM\r\n');
m_AGE = 0; m_pEPs = 0; m_pCEPs = 0; m_MSSSIM = 0; m_PSNR = 0; m_CQM = 0;
for category_id = 1 : length(category_list)
    category_name = category_list{category_id};
    c_AGE = 0; c_pEPs = 0; c_pCEPs = 0; c_MSSSIM = 0; c_PSNR = 0; c_CQM = 0;
    GT_category_path = fullfile(GT_path, category_name);
    evaluated_category_path = fullfile(evaluated_path, category_name);
    
    GT_videos = dir(GT_category_path);
    evaluated_videos = dir(evaluated_category_path);
    if length(evaluated_videos) == 0
        msg = ['The category "', category_name, '" is missing in your results'];
        msgbox(msg);
        warning(msg);
        continue
    end
    video_num = 0;
   
    fprintf(fid, '%s: \r\n', category_name(1:min(8, length(category_name))));
    
    for video_id = 1 : length(GT_videos)
        if GT_videos(video_id).isdir
            if strcmp(GT_videos(video_id).name, '.') || strcmp(GT_videos(video_id).name, '..')
                continue
            else
                video_name = GT_videos(video_id).name;
                GT_video_path = fullfile(GT_category_path, video_name);
                GTs = dir(fullfile(GT_video_path, '*.jpg'));
                if isempty(GTs)     % no GT for the video
                    continue;
                else
                    MSSSIM_max = 0;
                    video_num = video_num + 1;
                    %if more than one GT exists for the video, we keep the
                    %metrics with the highest MSSSIM value.
                    for GT_id = 1 : length(GTs)
                        GT_img = imread(fullfile(GT_video_path, GTs(GT_id).name));
                        evaluated_video_path = fullfile(evaluated_category_path, video_name);
                        evaluated_video_files = dir(fullfile(evaluated_video_path, '*.jpg'));
                        result_img = imread(fullfile(evaluated_video_path, evaluated_video_files(1).name));
                        [AGE, pEPs, pCEPs, MSSSIM, PSNR, CQM] = Evaluate(GT_img, result_img, 0);
                        if MSSSIM > MSSSIM_max
                            v_AGE = AGE; v_pEPs = pEPs; v_pCEPs = pCEPs; 
                            v_MSSSIM = MSSSIM; v_PSNR = PSNR; v_CQM = CQM;
                            MSSSIM_max = MSSSIM;
                        end
                    end
                    %save the video evaluation results
                    fprintf(fid, '\t\t%s:\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\r\n', video_name(1:min(5, length(video_name))), v_AGE, v_pEPs, v_pCEPs, v_MSSSIM, v_PSNR, v_CQM);
                    c_AGE = c_AGE + v_AGE; c_pEPs = c_pEPs + v_pEPs; c_pCEPs = c_pCEPs + v_pCEPs; 
                    c_MSSSIM = c_MSSSIM + v_MSSSIM; c_PSNR = c_PSNR + v_PSNR; c_CQM = c_CQM + v_CQM;
                end  
            end
        end
    end
    %save the category evaluation results
    c_AGE = c_AGE / video_num; c_pEPs = c_pEPs / video_num; c_pCEPs = c_pCEPs / video_num;
    c_MSSSIM = c_MSSSIM / video_num; c_PSNR = c_PSNR / video_num; c_CQM = c_CQM / video_num;
    fprintf(fid, '\r\n%s_AVG:\t\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\r\n\r\n', category_name(1:min(8, length(category_name))), c_AGE, c_pEPs, c_pCEPs, c_MSSSIM, c_PSNR, c_CQM);
    m_AGE = m_AGE + c_AGE; m_pEPs = m_pEPs + c_pEPs; m_pCEPs = m_pCEPs + c_pCEPs; 
    m_MSSSIM = m_MSSSIM + c_MSSSIM; m_PSNR = m_PSNR + c_PSNR; m_CQM = m_CQM + c_CQM;
end 
%save the method evaluation results
m_AGE = m_AGE / category_num; m_pEPs = m_pEPs / category_num; m_pCEPs = m_pCEPs / category_num;
m_MSSSIM = m_MSSSIM / category_num; m_PSNR = m_PSNR / category_num; m_CQM = m_CQM / category_num;
fprintf(fid, 'Total:\t\t\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\r\n', m_AGE, m_pEPs, m_pCEPs, m_MSSSIM, m_PSNR, m_CQM);
fclose(fid);
end

