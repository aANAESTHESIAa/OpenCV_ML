//
// Created by DUTCH on 2/14/2022.
//

#ifndef CV_LR2_CONSOLEIMAGEEDITOR_FUNCTIONS_H
#define CV_LR2_CONSOLEIMAGEEDITOR_FUNCTIONS_H

#include <opencv2/opencv.hpp>

using namespace cv;

extern const int kMenuTabs;
extern const char* winNames[];

int applyBlur(const Mat &src, Mat &dst);
int applyMedianBlur(const Mat &src, Mat &dst);
int applyGaussianBlur(const Mat &src, Mat &dst);
int applyOperation(const Mat &src, const int operationIdx);
int applyLinearFilter(const Mat &src, Mat &dst);
Mat applyErode(const Mat &src, Mat &dst);
Mat applyDilate(const Mat &src, Mat &dst);
Mat applyEqualizeHist(const Mat &src, Mat &dst);
std::pair<Mat, Mat> applySobel(const Mat &src, Mat &dst);
int applyLaplacian(const Mat &src, Mat &dst);
int applyCanny(const Mat &src, Mat &dst);
int applyCalcHist(const Mat &src, Mat &dst);

#endif //CV_LR2_CONSOLEIMAGEEDITOR_FUNCTIONS_H