#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//����1���ļ��� ����2����ӵ���
void AppendText(string filename, string text);

class fileUse {
	int magicNumber = 0;
	int numImages = 0;
	int numRows = 0;
	int numCols = 0;
	vector<vector<Mat>> trainingData;
public:
	fileUse();
	fileUse(const char* fpi, const char* fpl);
	int getMagicNumber();
	int getNumImages();
	int getNumRows();
	int getNumCols();
	vector<int> getNum();
	vector<vector<Mat>> getTrainingData();
	vector<vector<Mat>> readFile(const char* fpi, const char* fpl);
	int readFlippedInteger(FILE *fp);
};