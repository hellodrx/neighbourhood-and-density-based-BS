//#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "DBSCAN.h"
#include "BS_DBBASED.h"
#include "videowrite.h"


using namespace std;
using namespace cv;

const int trainingFrames = 100;
static int counter = 0;
const int bias = 15*2;



#define  Program1 1//样本里装的是每一帧图像，每一个像素在时间delta里的像素值
#define  Program2 0//样本里装的是前后帧的差分值

#if Program1

//sample里的元素要装三维
//dbscan里要能够聚类三维
//类里不要只读单通道图片，另外距离计算方式也应改为三维

void processVid(const Mat&, Mat&, vector< vector<pt>>& );
BS bs;

int main()
{
	VideoCapture cap("D:\\BSdataset\\test1.avi");

	Mat src,dst;//每一轮的原始图片和处理后的图片

	vector <vector <pt>> trainingSamples;

	videowriter wri("3channls100framesprogram2thr=30votes=2neighbour=5.avi",Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)),CV_FOURCC('M', 'J', 'P', 'G'),25.0);

	namedWindow("dst");

	for (;;)
	{
		cap >> src;
		if ( !src.data)
		{
			break;;
		}
		
		/*两种计算距离的方式*/
		//1. 三维
		CV_Assert( src.channels() == 3);
			
		//2. 灰度，一维
		/*if ( src.channels() != 1)
		{
			cvtColor(src,src,CV_BGR2GRAY);
		}*/

		processVid(src,dst, trainingSamples);
		wri.inputImg(dst);

		imshow("dst",dst);

		char c = waitKey(30);

		if ( 27 == c)
		{
			break;
		}


	}

	wri.generateVideo();
	return 0;

}

void processVid(const Mat& src, Mat& dst, vector< vector<pt>>& samples)
{
	counter++;//过一帧+一次

	cout<<" This is the "<<counter<<"th frame."<<endl;

	dst.create(src.size(),src.type());
	dst = Scalar::all(0);

	if ( samples.size() == 0)//如果samples没有初始化
	{
		int ssamples = src.rows * src.cols;
		samples.reserve(ssamples);

		for ( int i = 0; i < ssamples; i++)
		{
			samples.push_back(vector<pt> ());//双层之内部初始化
			samples[i].reserve(trainingFrames);
		}
	}

	int rows = src.rows;
	int cols = src.cols;

	if ( counter <= trainingFrames)//训练期
	{
		int index = 0;
		
		Mat_ <Vec3b> I = src;

		for ( int i = 0; i < rows; i++)//对于每一帧，样本上的每一个采样对应图像上每一个像素点
		{
			for ( int j = 0; j < cols; j++)
			{
				pt p;
				p.a = I(i,j)[0];
				p.b = I(i,j)[1];
				p.c = I(i,j)[2];

				samples[index].push_back(p);
				index++;
			}
		}
	}

	if ( counter == trainingFrames + 1)//训练期后第一帧，开始聚类
	{
		int size = samples.size();
		for ( int i = 0; i < size; i++)
		{
			if ( samples[i].size() == 0)
			{
				break;
			}
			
			DBSCAN DBcluster;

			DBcluster(samples[i]);

			vector <vector<pt>> clusters = DBcluster.getClusters();

			int sClusters = clusters.size();

			if ( sClusters == 0)
			{
				;
			}
			else
			{
				int maxSize = 0;
				int maxSizeIndex;
				for ( int j = 0; j < sClusters; j++)//寻找含有最多数目的集合，记录编号
				{
					if ( clusters[j].size() > maxSize)
					{
						maxSize = clusters[j].size();
						maxSizeIndex = j;
					}
					else
					{
						continue;
					}
				}

				//一维可以找最大值最小值，三维就存储所有的样本值
				samples[i].resize(maxSize);//每个样本存储的是含有最多样本值的集合
				for ( int ii = 0; ii < maxSize; ii++)
				{
					samples[i][ii] = clusters[maxSizeIndex][ii];
				}

			}
			cout<<" Had processed the "<<i<<"th samples, please be patient!"<<endl;
		}

		//把聚类的值放入背景模型
		bs.init(samples,src);

	}

	if ( counter > trainingFrames + 1)
	{
		bs(src,dst);
	}

}

#endif



#if Program2
const int thr = 15;
void processVid(const Mat&, Mat&, vector< vector<int>>& );
BS bs;


Mat subtraction(const Mat& prev, const Mat& curr)//取两像素相减的绝对值
{
	CV_Assert ( prev.channels() == 1 && curr.channels() == 1);

	int cols = prev.cols;
	int rows = prev.rows;

	Mat temp(rows,cols,CV_8UC1);

	for ( int i = 0; i < rows; i++)
	{
		const uchar* p = curr.ptr<uchar>(i);
		const uchar* q = prev.ptr<uchar>(i);
		uchar* m = temp.ptr<uchar>(i);

		for ( int j = 0; j < cols; j++)
		{
			m[j] = abs(p[j] - q[j]);
		}
	}

	return temp;
}

Mat pr,curr;

int main()
{
	VideoCapture cap("D:\\BSdataset\\test1.avi");

	Mat src,dst;//每一轮的原始图片和处理后的图片

	vector <vector <int>> trainingSamples;

	videowriter wri("100framestrainingtest1Program2.avi",Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)),CV_FOURCC('M', 'J', 'P', 'G'),25.0);

	namedWindow("dst");

	for (;;)
	{
		cap >> src;

		if ( !src.data)
		{
			break;;
		}

		if ( src.channels() != 1)
		{
			cvtColor(src,src,CV_BGR2GRAY);
		}

		curr = src.clone();
		if (! pr.data)
		{
			pr = curr.clone();
			continue;
		}

		processVid(src,dst, trainingSamples);
		wri.inputImg(dst);

		imshow("dst",dst);

		char c = waitKey(30);

		if ( 27 == c)
		{
			break;
		}

		cv::swap(curr,pr);
	}

	wri.generateVideo();
	return 0;

}

void processVid(const Mat& src, Mat& dst, vector< vector<int>>& samples)
{
	counter++;//过一帧+一次

	cout<<" This is the "<<counter<<"th frame."<<endl;

	dst.create(src.size(),src.type());
	dst = Scalar::all(0);

	if ( samples.size() == 0)//如果samples没有初始化
	{
		int ssamples = src.rows * src.cols;
		samples.reserve(ssamples);

		for ( int i = 0; i < ssamples; i++)
		{
			samples.push_back(vector<int> ());//双层之内部初始化
			samples[i].reserve(trainingFrames);
		}
	}

	int rows = src.rows;
	int cols = src.cols;

	Mat sub = subtraction(curr,pr);

	if ( counter <= trainingFrames)//训练期
	{
		int index = 0;

		for ( int i = 0; i < rows; i++)
		{
			uchar* pSub = sub.ptr<uchar>(i);//读值
			const uchar* pSrc = src.ptr<uchar>(i);

			for ( int j = 0; j < cols; j++)
			{
				//只保留背景差分后，黑色部分，白色部分的值不存入sample
				
				if ( pSub[j] <= thr)
				{
					samples[index].push_back(pSrc[j]);//把总数trainingFrames的值放入每个sample
				}
				index++;

			}
		}
	}

	if ( counter == trainingFrames + 1)//训练期后第一帧，开始聚类
	{
		int size = samples.size();
		for ( int i = 0; i < size; i++)
		{
			if ( samples[i].size() == 0)
			{
				break;
			}

			DBSCAN DBcluster;

			DBcluster(samples[i]);

			vector <vector<int>> clusters = DBcluster.getClusters();

			int sClusters = clusters.size();

			if ( sClusters == 0)
			{
				int a1,a2;

				while (true)//两个随机数必须满足第一个比第二个小
				{
					RNG& rng = theRNG();//如果50帧都是离群点，随机抽取50帧内任意两个值
					a1 = rng.uniform(0,trainingFrames);
					a2 = rng.uniform(0,trainingFrames);

					if ( a1 < a2)
					{
						break;
					}
				}


				samples[i].resize(2);//把每个集合的元素数目缩小到两个，只存储最大值和最小值
				samples[i][0] = a1;
				samples[i][1] = a2;
			}
			else
			{
				int maxSize = 0;
				int maxSizeIndex;
				for ( int j = 0; j < sClusters; j++)//寻找含有最多数目的集合，记录编号
				{
					if ( clusters[j].size() > maxSize)
					{
						maxSize = clusters[j].size();
						maxSizeIndex = j;
					}
					else
					{
						continue;
					}
				}

				int max = 0;//寻找该集合的最大，最小值
				int min = clusters[maxSizeIndex][0];
				for ( int m = 0; m < maxSize; m++)
				{
					if ( clusters[maxSizeIndex][m] > max)
					{
						max = clusters[maxSizeIndex][m];
					}
					if ( clusters[maxSizeIndex][m] < min)
					{
						min = clusters[maxSizeIndex][m];
					}
				}

				samples[i].resize(2);//把每个集合的元素数目缩小到两个，只存储最大值和最小值
				samples[i][0] = min;
				samples[i][1] = max;
			}
			cout<<" Had processed the "<<i<<"th samples, please be patient!"<<endl;
		}

		//把聚类的值放入背景模型
		bs.init(samples,src);

	}

	if ( counter > trainingFrames + 1)
	{
		bs(src,dst);
	}

}

#endif