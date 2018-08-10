#pragma once

#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define HORIZONTAL    1
#define VERTICAL    0

// ！获取垂直和水平方向直方图
Mat ProjectedHistogram(Mat img, int t);

//获得特征
Mat features(Mat in, int sizeData);
