/************************************************************************/
/*       videowrite类    将数据存入，然后写成视频                       */
/************************************************************************/


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#ifndef  _VIDEO_WRITE
#define  _VIDEO_WRITE

class videowriter
{
private:
	std::vector <cv::Mat> images;
	cv::VideoWriter       wri;
	int                   counter;//计数器

public:
	videowriter();
	videowriter(const char*, const cv::Size&, int, float, bool isRGB = 0);
	~videowriter();

	void inputImg( cv::Mat);
	void generateVideo();
};




#endif