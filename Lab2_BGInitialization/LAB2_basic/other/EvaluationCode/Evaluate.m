%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [AGE,pEPs,pCEPs,MSSSIM,PSNR,CQM]=Evaluate(GT, BC, show)

% GT = imread('C:\Project\SBMIweb\dataset\SBMnet_dataset\artificial\211\GT_background1.jpg');
% BC = imread('C:\Project\SBMIweb\results-median_filter\Artificial\211\bg_1.jpg');
% GT = imread('lena2.jpg');
% BC = imread('lena2.jpg');
    if (~exist('show'))
        show=0;
    end
    
    [M,N,Bands]=size(GT);
    dimension=M*N;
%
%In case of color images, use luminance in YCbCr
%
    if (Bands==3)
%         GTconv = rgb2ycbcr(GT); YGT=GTconv(:,:,1);
%         BCconv = rgb2ycbcr(BC); YBC=BCconv(:,:,1);
        YGT = .299*double(GT(:,:,1)) + .587*double(GT(:,:,2)) + .114*double(GT(:,:,3));
        YBC = .299*double(BC(:,:,1)) + .587*double(BC(:,:,2)) + .114*double(BC(:,:,3));
    else
        YGT=GT;
        YBC=BC;
    end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%% AGE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    Diff=uint8(abs(double(YGT)-double(YBC)));
 %       Diff=abs(double(YGT)-double(YBC));
    AGE=mean(mean(Diff));

%%%%%%%%%%%%%%%%%%%%%%%%%%% EPs and pEPs %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    threshold=20;
    Errors=Diff>threshold;
    EPs=sum(sum(Errors));
    pEPs=EPs/dimension;

%%%%%%%%%%%%%%%%%%%%%%%%%% CEPs and pCEPs %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    se = strel('diamond',1);        
    erodedErrors = imerode(Errors,se);
    CEPs=sum(sum(erodedErrors));
    pCEPs=CEPs/dimension;
    if (show)
        figure; 
        subplot(2,3,1); imshow(GT);title('GT');
        subplot(2,3,2); imshow(BC);title('BC');
        subplot(2,3,3); imshow(Diff,[]);title('|GT-BC|');
        subplot(2,3,4); imshow(Errors);title('EPs');
        subplot(2,3,5); imshow(erodedErrors);title('CEPs');
    end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%% MSSSIM %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Use default values for msssim(img1, img2, K, win, level, weight, method):
% K = [0.01 0.03], 
% level = 5, 
% weight = [0.0448 0.2856 0.3001 0.2363 0.1333],
% method = 'product';
% except for 'win', whose size depends on the input images size
    if (min(size(YGT))>=176)
        dim=11;
    else
        dim=floor(min(size(YGT))/16);
    end
    win = fspecial('gaussian', dim, 1.5);
    MSSSIM = msssim(YGT,YBC,[0.01 0.03],win);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PSNR %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    PSNR=PeakSignaltoNoiseRatio(YGT,YBC,255);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%% CQM %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if (Bands==3)
        CQM=cqm(GT, BC);
    end
end
