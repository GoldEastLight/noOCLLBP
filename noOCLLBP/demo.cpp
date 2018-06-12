//#include<opencv2/opencv.hpp>
//#include<iostream>
//using namespace std;
//using namespace cv;
//
//String fileName = "F:\\Program Files (x86)\\Opencv310_2\\opencv\\build\\etc\\lbpcascades\\lbpcascade_frontalface.xml";
//int main(int argc, char**argv)
//{
//	CascadeClassifier faceCascade;
//	if (!faceCascade.load(fileName))
//	{
//		return -1;
//	}
//	Mat src = imread("G:\\pic\\1.jpg");
//
//	if (src.empty())
//	{
//		return -2;
//	}
//	imshow("src", src);
//	waitKey(0);
//	vector<Rect> faces;
//	faceCascade.detectMultiScale(src, faces, 1.1, 0, 0, Size(30, 24));
//
//	for (vector<Rect>::iterator it = faces.begin(); it != faces.end(); it++)
//	{
//		rectangle(src, *it, Scalar(0, 0, 255), 1, 8, 0);
//	}
//	imshow("dst", src);
//	waitKey(0);
//	return 0;
//
//
//
//}