#pragma once

#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define HORIZONTAL    1
#define VERTICAL    0

// ����ȡ��ֱ��ˮƽ����ֱ��ͼ
Mat ProjectedHistogram(Mat img, int t);

//�������
Mat features(Mat in, int sizeData);
