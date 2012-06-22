//
//  SmartBlurFilter.h
//  SmartBlurFilter
//
//  Created by Yu-Jen Lin on 6/22/12.
//  Copyright (c) 2012 home. All rights reserved.
//

#ifndef SmartBlurFilter_SmartBlurFilter_h
#define SmartBlurFilter_SmartBlurFilter_h

#include <opencv2/opencv.hpp>

static const int PADDING = 5;
static const int WINDOWSIZE = 5;
static const int PIXELAMOUNT = WINDOWSIZE * WINDOWSIZE;
static const int HALF_WINDOWSIZE = 2;
static const int BLUR_KERNEL = 51;

class SmartBlurFilter
{
public:
    SmartBlurFilter();
    cv::Mat compute(cv::Mat inputMat, int whiteThreshold, int brightThreshold, int blurThreshold);
     
    
private:
    cv::Mat hsvMat;
    
    cv::Mat computeWhite(cv::Mat inputMat, int whiteThreshold, int brightThreshold);
    cv::Mat computeBlur(cv::Mat inputMat, int threshold);
    //float lerp(float pointA, float pointB, float amt);
    //cv::Vec3b lerpPixel( cv::Vec3b pointA, cv::Vec3b pointB, float amt);
    //float rmsError(cv::Mat roiMat);
    //float getAmount(float rms, float cutoff);
    
    
};


#endif
