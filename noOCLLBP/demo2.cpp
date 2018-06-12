// TransLbp.cpp : 定义控制台应用程序的入口点。
//



#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui/highgui_c.h"



#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;

static void help()
{
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip);

string nestedCascadeName = "../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";


string cascadeName_haar = "D:/opencv300/opencv/mybuild/bin/Release/dt0905_haar/cascade.xml";//在刘永才电脑上训练的。

string cascadeName_lbp = "cascade.xml"; // 调整尾部


char videoName[1200];
int sampleIdx = 0;

char *algName[2] = { "LBP", "Haar" };
int flag = -1;

int main(int argc, const char** argv)
{
	CvCapture* capture = 0;
	Mat frame, frameCopy, image, imageRoi;
	const string scaleOpt = "--scale=";
	size_t scaleOptLen = scaleOpt.length();
	const string cascadeOpt = "--cascade=";
	size_t cascadeOptLen = cascadeOpt.length();
	const string nestedCascadeOpt = "--nested-cascade";
	size_t nestedCascadeOptLen = nestedCascadeOpt.length();
	const string tryFlipOpt = "--try-flip";
	size_t tryFlipOptLen = tryFlipOpt.length();
	string inputName;
	bool tryflip = false;

	help();

	CascadeClassifier cascade_lbp, cascade_haar, nestedCascade;
	double scale = 3;


	if (!cascade_lbp.load(cascadeName_lbp))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		help();
		return -1;
	}

	//if( !cascade_haar.load( cascadeName_haar ) )
	//{
	//	cerr << "ERROR: Could not load classifier cascade" << endl;
	//	help();
	//	return -1;
	//}


	string video_name = "E:/行车记录仪调研/Video_飞度_手机录制视频/VID_20170623_085140.mp4";

	const char *pstr = video_name.c_str();

	memcpy(videoName, pstr, 100);


	cvNamedWindow("result", 1);
	int circle = 1;

	cout << "In image read" << endl;
	char imageName[1200];


	int idxStart = 10015212;//100628;
	int idxEnd = idxStart + 20000;

	int stepFrm = 0;

	for (int imgIdx = idxStart; imgIdx < idxEnd; imgIdx++)
	{

		//sprintf(imageName,"E:/Project/LDW1080P/GrayImages/1020FitYizhuang/%d.jpg", imgIdx);			//  单个图像测试
		//sprintf(imageName,"G:/Project/LDW1080P手机/GrayImages/1016/%d.jpg", imgIdx);			//  单个图像测试
		//sprintf(imageName,"E:/Project/LDW1080P/GrayImages/1017FitNight/%d.jpg", imgIdx);			//  单个图像测试

		//sprintf(imageName,"E:/Project/LDW1080P/GrayImages/1017FitYouanToYizhuang/%d.jpg", imgIdx);			//  单个图像测试
		//sprintf(imageName,"E:/Project/LDW1080P/GrayImages/1020FitYizhuang/%d.jpg", imgIdx);			//  单个图像测试
		//sprintf(imageName,"E:/Project/LDW1080P/GrayImages/1021Fit/%d.jpg", imgIdx);			//  单个图像测试
		//sprintf(imageName,"E:/Project/LDW1080P/GrayImages/1025YiZhuangXiaWu/%d.jpg", imgIdx);			//  单个图像测试

		sprintf(imageName, "M:\\新摄像头相机质量调试_视频采集\\20180409DAY2/C%d.bmp", imgIdx);  // 100000.bmp



																				  //cout<<imageName<<endl;


																				  //  
		image = imread(imageName, 0);

		if (image.empty())
		{
			continue;
		}
		scale = 1.0;
		double angle = -5;
		Point2f center(image.cols / 2, image.rows / 2);
		Mat rot = getRotationMatrix2D(center, angle, 1);
		Rect bbox = RotatedRect(center, image.size(), angle).boundingRect();

		rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
		rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

		Mat dst;
		warpAffine(image, dst, rot, bbox.size());


		detectAndDraw(image, cascade_lbp, nestedCascade, scale, tryflip);



		char c = waitKey(stepFrm);

		if (c == 27)
			break;

		if (c == 'n')
		{
			stepFrm = 0;
			continue;
		}

		if (c == 'k')
		{
			stepFrm = 8;	//20;
		}

		if (c == 'm')
		{
			stepFrm = 200;
		}

		if (c == 'p')		//		回去很多
		{
			imgIdx -= 20;
			stepFrm = 0;
		}

		if (c == 'f')
		{
			imgIdx -= 5;
			stepFrm = 0;
		}

		if (c == 'l')
		{
			imgIdx += 200;
			stepFrm = 0;

		}
	}





	cvDestroyWindow("result");

	return 0;
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip)
{
	int i = 0;
	double t = 0;
	vector<Rect> cars, faces2;
	const static Scalar colors[] = { CV_RGB(0,0,255),
		CV_RGB(0,128,255),
		CV_RGB(0,255,255),
		CV_RGB(0,255,0),
		CV_RGB(255,128,0),
		CV_RGB(255,255,0),
		CV_RGB(255,0,0),
		CV_RGB(255,0,255) };
	Mat gray, smallImg2(cvRound(img.rows / scale), cvRound(img.cols / scale), CV_8UC1);
	//Mat imgCopy = img.clone();
	resize(img, smallImg2, smallImg2.size(), 0, 0, INTER_LINEAR);
	int height = smallImg2.rows;
	int width = smallImg2.cols;


	//cvtColor( img, gray, COLOR_BGR2GRAY );
	//Rect rect(0, height/3, width, height*2/3);  // 宽高
	//Mat smallImg = smallImg2(rect);

	Mat smallImg = smallImg2;



	//smallImg = dst;
	//cv::imshow("dst", dst);
	//cv::waitKey(0);
	//Mat image_roi = image(rect);
	//gray = img;

	//equalizeHist( smallImg, smallImg );

	t = (double)cvGetTickCount();
	cascade.detectMultiScale(smallImg, cars,
		1.1, 2, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE
		| CASCADE_DO_CANNY_PRUNING
		,
		Size(32, 28)
		//Size(30,24)
	);

	t = (double)cvGetTickCount() - t;
	printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	for (vector<Rect>::const_iterator r = cars.begin(); r != cars.end(); r++, i++)
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r->width / r->height;
		if (0.5 < aspect_ratio && aspect_ratio < 1.5)
		{
			// 			center.x = cvRound((r->x + r->width*0.5)*scale);
			// 			center.y = cvRound((r->y + r->height*0.5)*scale);
			// 			radius = cvRound((r->width + r->height)*0.25*scale);
			// 			// circle( img, center, radius, color, 3, 8, 0 );
			// 
			// 
			// 
			// 			//  Save Samples Images;  程序进行了修改：
			// 			int crx = cvRound(r->x*scale);
			// 			int cry =cvRound(r->y*scale);
			// 			int crdx = cvRound((r->x + r->width-1)*scale);
			// 			int crdy = cvRound((r->y + r->height-1)*scale);
			// 			int crw = crdx - crx;
			// 			int crh = crdy - cry;
			// 			int exW = cvRound(crw*0.12);
			// 			int exH = cvRound(crh*0.12);
			// 
			// 			crx = crx - exW; crx = max(0,crx);
			// 			cry = cry - exH; cry = max(0,cry);
			// 			crdx = crdx +exW; crdx = min(1280, crdx);
			// 			crdy = crdy + exH; crdy = min(720, crdy);
			// 
			// 			crw = crdx - crx;
			// 			crh = crdy - cry;
			// 
			// 			Rect rect(crx, cry, crw, crh);  // 宽 高

			rectangle(smallImg, cvPoint(cvRound(r->x), cvRound(r->y)), cvPoint(cvRound((r->x + r->width - 1)), cvRound((r->y + r->height - 1))), color, 1, 8, 0);

		}
		else
		{
			rectangle(smallImg, cvPoint(cvRound(r->x), cvRound(r->y)),
				cvPoint(cvRound((r->x + r->width - 1)), cvRound((r->y + r->height - 1))),
				color, 1, 8, 0);
		}

	}


	cv::imshow("result", smallImg);
}
