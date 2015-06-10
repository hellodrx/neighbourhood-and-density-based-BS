

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include "PT.h"


#ifndef _BS
#define _BS


using namespace std;
using namespace cv;


class BS
{
private:
	vector < vector<pt>> model;//当前场景的背景模型
	Size s;//图片大小

public:
	BS();
	BS( vector <vector<pt>>,const Mat&);

	void init( vector<vector<pt>>, const Mat&);//目前只能用特定的数据结构初始化
	void update( const Mat&, Mat&);
	void operator()( const Mat&, Mat&);
	void getForeground( const Mat&, Mat&);
	void computeVotes( int, int, int,Vec3b,int&,bool&);
};

#endif
#pragma   once 