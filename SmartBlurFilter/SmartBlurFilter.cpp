//
//  SmartBlurFilter.cpp
//  SmartBlurFilter
//
//  Created by Yu-Jen Lin on 6/22/12.
//  Copyright (c) 2012 home. All rights reserved.
//

#include "SmartBlurFilter.h"
#include <iostream>


SmartBlurFilter::SmartBlurFilter()
{
    hsvMat = cv::Mat();
}


float SmartBlurFilter::lerp(float pointA, float pointB, float amt)
{
    return pointA + amt * (pointB - pointA);
}

cv::Vec3b SmartBlurFilter::lerpPixel( cv::Vec3b pointA, cv::Vec3b pointB, float amt)
{
    cv::Vec3b resultPixel;
    resultPixel[0] = lerp((float)pointA[0], (float)pointB[0], amt);
    resultPixel[1] = lerp((float)pointA[1], (float)pointB[1], amt);
    resultPixel[2] = lerp((float)pointA[2], (float)pointB[2], amt);

    return resultPixel;
}

float SmartBlurFilter::rmsError(cv::Mat roiMat)
{
    int avgR = 0;
    int avgG = 0;
    int avgB = 0;
    
    for (int j = 0; j < roiMat.rows; j++)
    {
        for (int i = 0; i < roiMat.cols; i++)
        {
            avgR += roiMat.at<cv::Vec3b>(j,i)[0];
            avgG += roiMat.at<cv::Vec3b>(j,i)[1];
            avgB += roiMat.at<cv::Vec3b>(j,i)[2];
        }
    }
    
    avgR /= (PIXELAMOUNT + 15);
    avgG /= (PIXELAMOUNT + 15);
    avgB /= (PIXELAMOUNT + 15);
    
    float accumulator = 0;
    
    for (int j = 0; j < roiMat.rows; j++)
    {
        for (int i = 0; i < roiMat.cols; i++)
        {
            int tempR = roiMat.at<cv::Vec3b>(j,i)[0] - avgR;
            int tempG = roiMat.at<cv::Vec3b>(j,i)[1] - avgG;
            int tempB = roiMat.at<cv::Vec3b>(j,i)[2] - avgB;
            
            tempR *= tempR;
            tempG *= tempG;
            tempB *= tempB;
            
            accumulator += (tempR + tempG + tempB);
        }
    }
    accumulator /= PIXELAMOUNT;
    float rms = sqrtf(accumulator);
    return rms;
}

float SmartBlurFilter::getAmount(float rms, float cutoff)
{
    if (rms > cutoff)
        return 1.0f;
    return rms/cutoff;
}


cv::Mat SmartBlurFilter::compute(cv::Mat inputMat, int whiteThreshold, int brightThreshold, int blurThreshold)
{
    cv::Mat sourceMat = inputMat.clone();
    cv::Mat resultMat;
    
    cv::cvtColor(sourceMat, hsvMat, CV_RGB2HSV);
    
    resultMat = computeWhite(sourceMat, whiteThreshold, brightThreshold);
    resultMat = computeBlur(resultMat, blurThreshold);
    
    return resultMat;
}


cv::Mat SmartBlurFilter::computeBlur(cv::Mat inputMat, int threshold)
{
    cv::Mat blurMat;
    //  size base on picture size
    cv::GaussianBlur(inputMat, blurMat, cv::Size(BLUR_KERNEL, BLUR_KERNEL), 0);
    //cv::medianBlur(inputMat, blurMat, 5);
    //cv::blur(inputMat, blurMat, cv::Size(5,5));

    for (int j = PADDING; j <hsvMat.rows - PADDING; j++)
    {
        for (int i = PADDING; i < hsvMat.cols - PADDING; i++)
        {
            int h = hsvMat.at<cv::Vec3b>(j, i)[0];
            int s = hsvMat.at<cv::Vec3b>(j, i)[1];
            
            if (h > 90 && h < 130 &&  s > 30 && s < 180)
            {
                //  FACE
                cv::Rect rect(i-HALF_WINDOWSIZE, j-HALF_WINDOWSIZE, WINDOWSIZE, WINDOWSIZE);
                cv::Mat roiMat = inputMat(rect);
                float rms = rmsError(roiMat);
                float amt = getAmount(rms, threshold);
                inputMat.at<cv::Vec3b>(j,i) = lerpPixel(inputMat.at<cv::Vec3b>(j,i), blurMat.at<cv::Vec3b>(j,i), amt);
                
            }
            else 
            {
                //inputMat.at<cv::Vec3b>(j, i)[0] = 0;
                //inputMat.at<cv::Vec3b>(j, i)[1] = 0;
                //inputMat.at<cv::Vec3b>(j, i)[2] = 0;
            }
        }
    }
    return inputMat;
}

cv::Mat SmartBlurFilter::computeWhite(cv::Mat inputMat, int whiteThreshold, int brightThreshold)
{
    
    
    for (int j = PADDING; j<hsvMat.rows - PADDING; j++)
    {
        for (int i = PADDING; i < hsvMat.cols - PADDING; i++)
        {
            int h = hsvMat.at<cv::Vec3b>(j, i)[0];
            int s = hsvMat.at<cv::Vec3b>(j, i)[1];
            int v = hsvMat.at<cv::Vec3b>(j, i)[2];
            
            if (h > 100 && h < 130 &&  s > 50 && s < 200)
            {
                s += whiteThreshold;
                if ( s < 0 )
                {
                    s = 5;
                }
                
                if ( s > 255)
                {
                    s = 250;
                }
                
                
                hsvMat.at<cv::Vec3b>(j, i)[1] = s;
                
                v += brightThreshold;
                if ( v < 0 )
                {
                    v = 5;
                }
                
                if ( v > 255)
                {
                    v = 250;
                }
                
                hsvMat.at<cv::Vec3b>(j, i)[2] = v;
            }
            else
            {
            }
        }
    }
    
    cv::cvtColor(hsvMat, inputMat, CV_HSV2RGB);
    
    return inputMat;
    
}


