// ImageComparison.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

Mat srcImg, dstImg;

Mat fuseImg(Mat &_leftImg, Mat &_rightImg, double _ratio)
{
	if (_leftImg.size() != _rightImg.size())
	{
		return Mat();
	}

	if (_ratio == 1.0)
	{
		return _leftImg.clone();
	}
	else if (_ratio == 0.0)
	{
		return _rightImg.clone();
	}
	else
	{
		size_t imgWidth = _leftImg.size().width;

		size_t cropPoint = imgWidth * _ratio;
		Range leftRange = Range(0, cropPoint);
		Range rightRange = Range(cropPoint, imgWidth - 1);

		Mat retImg(_leftImg.size(), _leftImg.type());
		_leftImg(Range::all(), leftRange).copyTo(retImg(Range::all(), leftRange));
		_rightImg(Range::all(), rightRange).copyTo(retImg(Range::all(), rightRange));

		return retImg;
	}
}

void onTrackbarChange(int _position, void *_imgSet)
{
	double splitRatio = 1.0 * getTrackbarPos("SplitImg", "Comparison") / TRACKBAR_MAX_POSITION;
	imshow("Comparison", fuseImg(srcImg, dstImg, splitRatio));
	waitKey(50);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 3)
	{
		return 1;
	}

	string srcFile(argv[1]);
	string dstFile(argv[2]);

	srcImg = imread(srcFile);
	dstImg = imread(dstFile);
	if (srcImg.size() != dstImg.size())
	{
		resize(srcImg, dstImg, dstImg.size());
	}
	flip(dstImg, dstImg, 1);

	namedWindow("Comparison", CV_WINDOW_AUTOSIZE);
	int trackbarInitPos = TRACKBAR_MAX_POSITION / 2;
	createTrackbar("SplitImg", "Comparison", &trackbarInitPos, TRACKBAR_MAX_POSITION, onTrackbarChange);
	onTrackbarChange(trackbarInitPos, NULL);

	waitKey();
	return 0;
}