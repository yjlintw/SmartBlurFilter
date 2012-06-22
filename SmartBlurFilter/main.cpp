//
//  main.cpp
//  SmartBlurFilter
//
//  Created by Yu-Jen Lin on 6/22/12.
//  Copyright (c) 2012 home. All rights reserved.
//

#include <iostream>
//#include <opencv2/opencv.hpp>
#include "SmartBlurFilter.h"

using namespace std;

int main(int argc, const char * argv[])
{
    string inFileName, outFileName;
    int whiteThreshold, brightThreshold, blurThreshold;
    cout << argc << endl;
    if (argc == 6)
    {
        inFileName = argv[1];
        outFileName = argv[2];
        whiteThreshold = atoi(argv[3]);  //  +- 20
        brightThreshold = atoi(argv[4]); // +- 5
        blurThreshold = atoi(argv[5]);   //  180
        
    }
    else 
    {
        cout << "ERROR: incomplete argument" << endl;
    }
    
    cv::Mat inputMat;
    inputMat = cv::imread(inFileName);
    if ( inputMat.data == 0 )
    {
        cout << "ERROR: no such file" << endl;
        return 0;
    }
    
    SmartBlurFilter *filter = new SmartBlurFilter();
    cv::Mat resultMat = filter->compute(inputMat, whiteThreshold, brightThreshold, blurThreshold);
    
    cv::imshow("Original", inputMat);
    cv::imshow("Result", resultMat);
    cv::imwrite(outFileName, resultMat);

    
    cv::waitKey(0);
    
    return 0;
}

