#include <iostream>
#include "opencv2/opencv.hpp"
#include "fileUse.h"
#include "MLPuse.h"

using namespace std;
using namespace cv;

//annѵ������Ԥ��
void annPreper(string outputxml, vector<vector<Mat>> trainingData, int numImages)
{
	cout << "��ʼ����mlp����Ԥ��." << endl;
	Mat trainingDataf5; //��ͬ������feature
	Mat trainingDataf10;
	Mat trainingDataf15;
	Mat trainingDataf20;
	Mat classes;        //labels
	vector<int> trainingLabels;
	//ѵ��������Ԥ��
	for (int i = 0; i < numImages; i++)
	{
		//ǰ50����������ѵ��
		//cout <<strCharacters[i]<<"д��ѵ������"<< endl;
		
		Mat img = trainingData[0][i];
		//resize(img, img, Size(20, 20));
		Mat f5 = features(img, 5);
		Mat f10 = features(img, 10);
		Mat f15 = features(img, 15);
		Mat f20 = features(img, 20);
		trainingDataf5.push_back(f5);
		trainingDataf10.push_back(f10);
		trainingDataf15.push_back(f15);
		trainingDataf20.push_back(f20);
		trainingLabels.push_back(trainingData[1][i].at<float>(0, 0));			//ÿһ���ַ�ͼƬ����Ӧ���ַ���������±�

	}
	//��ʽת��
	/*trainingDataf5.convertTo(trainingDataf5, CV_32FC1);
	trainingDataf10.convertTo(trainingDataf10, CV_32FC1);
	trainingDataf15.convertTo(trainingDataf15, CV_32FC1);
	trainingDataf20.convertTo(trainingDataf20, CV_32FC1);*/
	Mat(trainingLabels).copyTo(classes);
	//outputxml = ann_data.xml
	//д��,��������xml����
	FileStorage fs(outputxml, FileStorage::WRITE);
	fs << "TrainingDataF5" << trainingDataf5;
	fs << "TrainingDataF10" << trainingDataf10;
	fs << "TrainingDataF15" << trainingDataf15;
	fs << "TrainingDataF20" << trainingDataf20;
	fs << "classes" << classes;
	fs.release();
	cout << "����Ԥ������" << endl;
}

//annѵ��
void annTrain(int predictsize, int neurons, string inputxml, string outputxml, float back, float pro)
{
	Ptr<ml::ANN_MLP> ann = ml::ANN_MLP::create();
	//��ȡ����
	//inputxml = "ann_data.xml";
	FileStorage fs(inputxml, FileStorage::READ);
	int _predictsize = predictsize;
	int _neurons = neurons;
	Mat TrainingData;
	Mat Classes;
	string training;
	if (1)
	{
		stringstream ss(stringstream::in | stringstream::out);
		ss << "TrainingDataF" << _predictsize;//ע���������������д���ȥ��
		training = ss.str();
	}
	fs[training] >> TrainingData;
	fs["classes"] >> Classes;
	//����ѵ��ann
	cout << "����������: " << _predictsize
		<< "  ���������ز�ڵ���: " << _neurons << " ����ѵ��" << endl;
	////////////////////////////////////////////////
	Mat layers(1, 3, CV_32SC1);
	layers.at<int>(0) = TrainingData.cols;
	layers.at<int>(1) = _neurons; //3�����ز�
	layers.at<int>(2) = 10; //10������ �����Ϊ10
	//���ø�����Ԫ����
	ann->setLayerSizes(layers);
	//���ü����
	ann->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM);
	//����MLP��ѵ������  ���򴫲�
	ann->setTrainMethod(ml::ANN_MLP::BACKPROP, back, pro);

	//ann.create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);//CvANN_MLP::IDENTITY, CvANN_MLP::SIGMOID_SYM, and CvANN_MLP::GAUSSIAN.
													 //Prepare trainClases
													 //Create a mat with n trained data by m classes
	Mat trainClasses;
	trainClasses.create(TrainingData.rows, 10, CV_32FC1);
	for (int i = 0; i < trainClasses.rows; i++)
	{
		for (int k = 0; k < trainClasses.cols; k++)
		{
			if (k == Classes.at<int>(i))
				trainClasses.at<float>(i, k) = 1;
			else
				trainClasses.at<float>(i, k) = 0;
		}
	}
	//Mat weights(1, TrainingData.rows, CV_32FC1, Scalar::all(1));
	
	//Learn classifier
	ann->train(TrainingData, ml::ROW_SAMPLE, trainClasses);
	//ann->train(TrainingData, trainClasses, weights);
	/////////////////////////���//////////////////////
	cout << "End the saveModelChar" << endl;
	//string model_name = "ann.xml";
	/*FileStorage fsTo(outputxml, FileStorage::WRITE);
	ann->write(*fsTo, "ann");
	fsTo.release();*/
	ann->save(outputxml);
	fs.release();
	ann.release();

	// test
	/*Mat test = Mat(1, 81, CV_32FC1);
	for (int i = 0; i < 81; i++)
		test.at<float>(0, i) = TrainingData.at<float>(0, i);
	Mat output(1, 10, CV_32FC1);
	ann->predict(test, output);
	output = output;*/
}

//ann�������
float annKfoldTest(string mlpxml, int predictsize, vector<vector<Mat>> trainingData, int numImages)
{
	/*Ptr<ml::ANN_MLP>  ann = ml::ANN_MLP::create();
	ann->load(mlpxml);*/
	Ptr<ml::ANN_MLP> ann = ml::ANN_MLP::load(mlpxml);
	//Mat o = ann->getLayerSizes();
	int result = -1;
	Mat output(1, 10, CV_32FC1);
	int icorrect = 0;

	float fMinAccuracy = 1;
	float fAvgAccuracy = 0;
	float fSumAccuracy = 0;
		/*cout << "���" << strCharacters[i] << endl;*/
		/*stringstream ss(stringstream::in | stringstream::out);
		ss << charpath << "/" << strCharacters[i];
		vector<string> files;
		getFiles(ss.str(), files);*/
		
	for (int k = 0; k < numImages; k++)
	{
		Mat img = trainingData[0][k];
		Mat f = features(img, predictsize);
			
		ann->predict(f, output);
		result = -1;
		float maxVal = -2;
		for (int j = 0; j < 10; j++)
		{
			float val = output.at<float>(j);
			if (val > maxVal)
			{
				maxVal = val;
				result = j;
			}
		}
		//cout << trainingData[1][k].at<float>(0, 0) << endl;
		//�ж�
		if (result == trainingData[1][k].at<float>(0, 0))
		{
			icorrect = icorrect + 1;
		}

	}
	//printf("������ĸ %c ����ȷ��Ϊ %f\n", strCharacters[result], (float)icorrect / (float)files.size());
	/*fSumAccuracy += (float)icorrect / (float)numImages;
	if (fMinAccuracy > (float)icorrect / (float)numImages)
	{
		fMinAccuracy = (float)icorrect / (float)numImages;
	}
	icorrect = 0;
	fAvgAccuracy = fSumAccuracy / 10;*/
	//cout << icorrect << endl;
	fSumAccuracy = (float)icorrect / (float)numImages;
	char *txt = new char[50];
	std::sprintf(txt, "��ȷ��: %f     \n", fSumAccuracy);
	AppendText("output.txt", txt);
	return fSumAccuracy;
}

int main()
{
	//����
	fileUse tran = fileUse("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
	fileUse test = fileUse("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
	int magicNumber = tran.getMagicNumber();
	int numImages = tran.getNumImages();
	int numRows = tran.getNumRows();
	int numCols = tran.getNumCols();
	
	vector<vector<Mat>> trainingData = tran.getTrainingData();
	vector<vector<Mat>> testData = test.getTrainingData();

	//vector<int> chara = test.getNum();
	/*for (int i = 0; i < 10; i++)
		cout << chara[i] << " ";*/

	//annPreper("test.xml", trainingData, numImages);
	

	//�ҵ�BUG�����Խ���
	//Mat img = trainingData[0][0];
	//resize(img, img, Size(20, 20));
	//Mat f5 = features(img, 5);

	float back;
	float pro;
	float accuracy = 0;
	// Ѱ�����Ų���
	//annPreper("ann_data.xml", trainingData, numImages);
	for (int i = 1; i < 100; i++)
	{
		for (int j = 1; j < 100; j++)
		{
			char *txt = new char[50];
			annTrain(5, 20, "ann_data.xml", "ann.xml", (float)i / 100.0, float(j) / 100.0);
			float taccuracy = annKfoldTest("ann.xml", 5, testData, numImages);
			if (taccuracy > accuracy) 
			{
				back = (float)i / 100.0;
				pro = float(j) / 100.0;
				accuracy = taccuracy;
			}
		}
	}

	//����ֵ
	int p[4] = { 5,10,15,20 };
	//��Ԫ��
	int n[8] = { 20,40,60,80,100,120,140,160 };
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<8; j++)
		{
			char *txt = new char[50];
			std::sprintf(txt, "����ѵ��������ά��%d    ���������ؽڵ���%d", p[i], n[j]);
			AppendText("output.txt", txt);
			//step 1
			//annPreper("ann_data.xml", trainingData, numImages);
			//step 2
			annTrain(p[i], n[j], "ann_data.xml", "ann.xml", 0.05, 0.45);
			//step 3
			annKfoldTest("ann.xml", p[i], testData, numImages);
		}
	}
	cout << "����������" << endl;
	//getchar();

	return 0;
}

