#include "videowrite.h"

using namespace std;
using namespace cv;

videowriter::videowriter()
{

}

videowriter::~videowriter()
{

}

videowriter::videowriter(const char* name, const cv::Size& s, int fourcc, float frames, bool isRGB)
{
	wri = VideoWriter(name,fourcc,frames,s,0);
	counter = 0;
}

void videowriter::inputImg(cv::Mat src)
{
	Mat _src = src.clone();
	images.push_back(_src);//±ÿ–Ë…ÓøΩ±¥
	counter++;
}

void videowriter::generateVideo()
{
	for ( int i = 0; i < counter; i++)
	{
		if ( !images[i].data)
		{
			break;
		}

		wri << images[i];
	}
}