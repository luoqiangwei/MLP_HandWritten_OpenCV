#include "fileUse.h"

fileUse::fileUse()
{
}

fileUse::fileUse(const char * fpi, const char * fpl)
{
	fileUse::readFile(fpi, fpl);
}

int fileUse::getMagicNumber()
{
	return fileUse::magicNumber;
}

int fileUse::getNumImages()
{
	return fileUse::numImages;
}

int fileUse::getNumRows()
{
	return fileUse::numRows;
}

int fileUse::getNumCols()
{
	return fileUse::numCols;
}

vector<int> fileUse::getNum()
{
	int chara[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	vector<int> result;
	//�Բ�ͬ���ֵ���Ŀ����ͳ��
	for (int i = 0; i < numImages; i++)
	{
		//Mat timg = trainingData[0][i];
		Mat tlab = trainingData[1][i];
		switch ((int)tlab.at<float>(0, 0))
		{
		case 0:
			chara[0]++;
			break;
		case 1:
			chara[1]++;
			break;
		case 2:
			chara[2]++;
			break;
		case 3:
			chara[3]++;
			break;
		case 4:
			chara[4]++;
			break;
		case 5:
			chara[5]++;
			break;
		case 6:
			chara[6]++;
			break;
		case 7:
			chara[7]++;
			break;
		case 8:
			chara[8]++;
			break;
		case 9:
			chara[9]++;
			break;
		default:
			break;
		}
	}
	result.push_back(chara[0]);
	result.push_back(chara[1]);
	result.push_back(chara[2]);
	result.push_back(chara[3]);
	result.push_back(chara[4]);
	result.push_back(chara[5]);
	result.push_back(chara[6]);
	result.push_back(chara[7]);
	result.push_back(chara[8]);
	result.push_back(chara[9]);
	return result;
}

vector<vector<Mat>> fileUse::getTrainingData()
{
	return fileUse::trainingData;
}

vector<vector<Mat>> fileUse::readFile(const char* fpiname, const char* fplname)
{
	FILE *fpi = fopen(fpiname, "rb");
	FILE *fpl = fopen(fplname, "rb");

	if (!fpi || !fpl)
	{
		cout << "Files not Found" << endl;
		exit(-1);
	}

	int magicNumber = readFlippedInteger(fpi);
	fileUse::magicNumber = magicNumber;
	int numImages = readFlippedInteger(fpi);
	fileUse::numImages = numImages;
	int numRows = readFlippedInteger(fpi);
	fileUse::numRows = numRows;
	int numCols = readFlippedInteger(fpi);
	fileUse::numCols = numCols;

	fseek(fpl, 0x08, SEEK_SET);

	int size = numRows * numCols;

	vector<Mat> trainingVectors;
	vector<Mat> trainingLabels;
	vector<vector<Mat>> trainingData;

	uchar *temp = new uchar[size];

	uchar tempClass = 0;

	for (int i = 0; i < numImages; i++)
	{
		Mat img(numRows, numCols, CV_32FC1);
		Mat Labels(1, 1, CV_32FC1);
		fread((void*)temp, size, 1, fpi);
		fread((void*)(&tempClass), sizeof(uchar), 1, fpl);
		Labels.at<float>(0, 0) = tempClass;

		for (int k = 0; k < size; k++)
		{
			img.at<float>(k / numCols, k % numCols) = temp[k];
		}
		trainingVectors.push_back(img);
		trainingLabels.push_back(Labels);
		//cout << sizeof(temp) << endl;
		//imshow("data", img);
	}

	trainingData.push_back(trainingVectors);
	trainingData.push_back(trainingLabels);

	//�������ǲ�����Ĵ�����
	/*for (int i = 0; i < numImages; i++)
	{
	Mat timg = trainingData[0][i];
	Mat tlab = trainingData[1][i];
	i = i;
	}*/
	fileUse::trainingData = trainingData;
	return trainingData;
}

int fileUse::readFlippedInteger(FILE *fp) { //ÿ�ζ�ȡ4���ֽ� index3 �ļ�ǰ16���ֽ���˵�����֣���֮������ݸ�ʽ�������á�index1�ļ�ǰ8���ֽ���˵������
	int ret = 0;
	uchar *tmp;
	tmp = (uchar*)(&ret);
	fread(&tmp[3], sizeof(uchar), 1, fp);
	fread(&tmp[2], sizeof(uchar), 1, fp);
	fread(&tmp[1], sizeof(uchar), 1, fp);
	fread(&tmp[0], sizeof(uchar), 1, fp);
	return ret;
}

void AppendText(string filename, string text)
{
	fstream ftxt;
	ftxt.open(filename, ios::out | ios::app);
	if (ftxt.fail())
	{
		cout << "�����ļ�ʧ��!" << endl;
		getchar();
	}
	ftxt << text << endl;//ֱ����дʲô����ӡʲô
	ftxt.close();
}