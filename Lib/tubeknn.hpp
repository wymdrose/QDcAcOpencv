#pragma once

#include <opencv2/opencv.hpp> 
#include <opencv2/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

#define TRAINPATH     "TrainData"
#define TRAINDATANUM  11
#define NORMWIDTH     40
#define NORMHEIGHT    60

class TubeKnn
{
public:
	static QString getTubeValue(cv::String pathTube)
	{
		Mat image_org = imread(pathTube, IMREAD_COLOR);
		imshow("image_org", image_org);
		Mat image_gry = imread(pathTube, IMREAD_GRAYSCALE);
		if (image_gry.empty())
			return "empty";

		namedWindow("image_gry", WINDOW_AUTOSIZE);
		imshow("image_gry", image_gry);

		Mat test;
		image_gry.copyTo(test);

		// convert to binary image
		Mat image_bin;
		threshold(image_gry, image_bin, 100, 255, THRESH_BINARY);
		imshow("image_gry", image_gry);
		imshow("image_bin", image_bin);

		Mat image_dil;
		Mat element = getStructuringElement(MORPH_RECT, Size(1, 1));
		dilate(image_bin, image_dil, element);
		imshow("image_dil1", image_dil);

		vector<vector<Point> > contours_out;
		vector<Vec4i> hierarchy;
		findContours(image_dil, contours_out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);  // program crash when using debug
		drawContours(image_bin, contours_out, -1, Scalar(255, 0, 0), 1);//  
		imshow("loaction", image_bin);
		imshow("image_dil2", image_dil);

		// re-arrange loaction according to the real position in the original image 
		const size_t size = contours_out.size();
		vector<Rect> num_location;
		for (int i = 0; i < contours_out.size(); i++)
		{
			num_location.push_back(boundingRect(Mat(contours_out[i])));
		}
		sort(num_location.begin(), num_location.end(), cmp);

		// number extraction
		char filenamew[255];
		int tube_num = 0;
		vector<Mat> tube;
		for (int i = 0; i < contours_out.size(); i++)
		{
			if (!IsAllWhite(image_dil(num_location.at(i))))
			{
				tube.push_back(image_bin(num_location.at(i)));
				sprintf(filenamew, "%s%d.jpg", "loaction", tube_num);
				imshow(filenamew, tube.at(tube_num));
				imwrite(filenamew, tube.at(tube_num));
				tube_num++;
			}
		}
		cout << tube_num << endl;

		char trainfile[100];
		//vector<Mat> traindata;
		//vector<int> trainlabel;
		Mat traindata, trainlabel, tmp;
		for (int i = 0; i < 10; i++)
		{
			sprintf(trainfile, "%s\\%d.jpg", TRAINPATH, i);

			//traindata.push_back(imread(trainfile, IMREAD_GRAYSCALE));
			//trainlabel.push_back(i);
			//threshold(traindata.at(i), traindata.at(i), 50, 255, THRESH_BINARY);
			//resize(traindata.at(i), traindata.at(i), Size(NORMWIDTH, NORMHEIGHT));
			tmp = imread(trainfile, IMREAD_GRAYSCALE);
			threshold(tmp, tmp, 100, 255, THRESH_BINARY);
			cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
			traindata.push_back(tmp.reshape(0, 1));
			trainlabel.push_back(i);
		}

		sprintf(trainfile, "%s\\E.jpg", TRAINPATH);
		tmp = imread(trainfile, IMREAD_GRAYSCALE);
		threshold(tmp, tmp, 100, 255, THRESH_BINARY);
		cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
		traindata.push_back(tmp.reshape(0, 1));
		trainlabel.push_back(int('E'));	
		//
		sprintf(trainfile, "%s\\dot.jpg", TRAINPATH);
		tmp = imread(trainfile, IMREAD_GRAYSCALE);
		threshold(tmp, tmp, 100, 255, THRESH_BINARY);
		cv::resize(tmp, tmp, Size(NORMWIDTH, NORMHEIGHT));
		traindata.push_back(tmp.reshape(0, 1));
		trainlabel.push_back(int('D'));


		//
		traindata.convertTo(traindata, CV_32F);


		int K = 1;
		Ptr<TrainData> tData = TrainData::create(traindata, ROW_SAMPLE, trainlabel);
		Ptr<KNearest> knn = KNearest::create();
		knn->setDefaultK(K);
		knn->setIsClassifier(true);
		knn->train(tData);

		QString result;

		for (int i = 0; i < tube_num; i++)
		{
			cv::resize(tube.at(i), tube.at(i), Size(NORMWIDTH, NORMHEIGHT));
			tube.at(i) = tube.at(i).reshape(0, 1);
			tube.at(i).convertTo(tube.at(i), CV_32F);

			if (knn->predict(tube.at(i)) == 'E')
			{
				result += "E";
			}
			else if (knn->predict(tube.at(i)) == 'D')
			{
				result += ".";
			}
			else
			{
				auto a = QString::number(knn->predict(tube.at(i)));
				result +=QString::number(knn->predict(tube.at(i)));
			}
			
		}

		//	waitKey(0);
		//	destroyAllWindows();
		return result;
	}

	static bool IsAllWhite(Mat inputmat)
	{
		if (inputmat.rows * inputmat.cols == cvCountNonZero(&(IplImage)inputmat))
			return true;
		else
			return false;
	}

	static bool cmp(const Rect& a, const Rect& b)
	{
		if (a.x < b.x)
			return true;
		else
			return false;
	}
};




