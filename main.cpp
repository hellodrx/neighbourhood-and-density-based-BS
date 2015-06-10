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



#define  Program1 1//������װ����ÿһ֡ͼ��ÿһ��������ʱ��delta�������ֵ
#define  Program2 0//������װ����ǰ��֡�Ĳ��ֵ

#if Program1

//sample���Ԫ��Ҫװ��ά
//dbscan��Ҫ�ܹ�������ά
//���ﲻҪֻ����ͨ��ͼƬ�����������㷽ʽҲӦ��Ϊ��ά

void processVid(const Mat&, Mat&, vector< vector<pt>>& );
BS bs;

int main()
{
	VideoCapture cap("D:\\BSdataset\\test1.avi");

	Mat src,dst;//ÿһ�ֵ�ԭʼͼƬ�ʹ�����ͼƬ

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
		
		/*���ּ������ķ�ʽ*/
		//1. ��ά
		CV_Assert( src.channels() == 3);
			
		//2. �Ҷȣ�һά
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
	counter++;//��һ֡+һ��

	cout<<" This is the "<<counter<<"th frame."<<endl;

	dst.create(src.size(),src.type());
	dst = Scalar::all(0);

	if ( samples.size() == 0)//���samplesû�г�ʼ��
	{
		int ssamples = src.rows * src.cols;
		samples.reserve(ssamples);

		for ( int i = 0; i < ssamples; i++)
		{
			samples.push_back(vector<pt> ());//˫��֮�ڲ���ʼ��
			samples[i].reserve(trainingFrames);
		}
	}

	int rows = src.rows;
	int cols = src.cols;

	if ( counter <= trainingFrames)//ѵ����
	{
		int index = 0;
		
		Mat_ <Vec3b> I = src;

		for ( int i = 0; i < rows; i++)//����ÿһ֡�������ϵ�ÿһ��������Ӧͼ����ÿһ�����ص�
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

	if ( counter == trainingFrames + 1)//ѵ���ں��һ֡����ʼ����
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
				for ( int j = 0; j < sClusters; j++)//Ѱ�Һ��������Ŀ�ļ��ϣ���¼���
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

				//һά���������ֵ��Сֵ����ά�ʹ洢���е�����ֵ
				samples[i].resize(maxSize);//ÿ�������洢���Ǻ����������ֵ�ļ���
				for ( int ii = 0; ii < maxSize; ii++)
				{
					samples[i][ii] = clusters[maxSizeIndex][ii];
				}

			}
			cout<<" Had processed the "<<i<<"th samples, please be patient!"<<endl;
		}

		//�Ѿ����ֵ���뱳��ģ��
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


Mat subtraction(const Mat& prev, const Mat& curr)//ȡ����������ľ���ֵ
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

	Mat src,dst;//ÿһ�ֵ�ԭʼͼƬ�ʹ�����ͼƬ

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
	counter++;//��һ֡+һ��

	cout<<" This is the "<<counter<<"th frame."<<endl;

	dst.create(src.size(),src.type());
	dst = Scalar::all(0);

	if ( samples.size() == 0)//���samplesû�г�ʼ��
	{
		int ssamples = src.rows * src.cols;
		samples.reserve(ssamples);

		for ( int i = 0; i < ssamples; i++)
		{
			samples.push_back(vector<int> ());//˫��֮�ڲ���ʼ��
			samples[i].reserve(trainingFrames);
		}
	}

	int rows = src.rows;
	int cols = src.cols;

	Mat sub = subtraction(curr,pr);

	if ( counter <= trainingFrames)//ѵ����
	{
		int index = 0;

		for ( int i = 0; i < rows; i++)
		{
			uchar* pSub = sub.ptr<uchar>(i);//��ֵ
			const uchar* pSrc = src.ptr<uchar>(i);

			for ( int j = 0; j < cols; j++)
			{
				//ֻ����������ֺ󣬺�ɫ���֣���ɫ���ֵ�ֵ������sample
				
				if ( pSub[j] <= thr)
				{
					samples[index].push_back(pSrc[j]);//������trainingFrames��ֵ����ÿ��sample
				}
				index++;

			}
		}
	}

	if ( counter == trainingFrames + 1)//ѵ���ں��һ֡����ʼ����
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

				while (true)//������������������һ���ȵڶ���С
				{
					RNG& rng = theRNG();//���50֡������Ⱥ�㣬�����ȡ50֡����������ֵ
					a1 = rng.uniform(0,trainingFrames);
					a2 = rng.uniform(0,trainingFrames);

					if ( a1 < a2)
					{
						break;
					}
				}


				samples[i].resize(2);//��ÿ�����ϵ�Ԫ����Ŀ��С��������ֻ�洢���ֵ����Сֵ
				samples[i][0] = a1;
				samples[i][1] = a2;
			}
			else
			{
				int maxSize = 0;
				int maxSizeIndex;
				for ( int j = 0; j < sClusters; j++)//Ѱ�Һ��������Ŀ�ļ��ϣ���¼���
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

				int max = 0;//Ѱ�Ҹü��ϵ������Сֵ
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

				samples[i].resize(2);//��ÿ�����ϵ�Ԫ����Ŀ��С��������ֻ�洢���ֵ����Сֵ
				samples[i][0] = min;
				samples[i][1] = max;
			}
			cout<<" Had processed the "<<i<<"th samples, please be patient!"<<endl;
		}

		//�Ѿ����ֵ���뱳��ģ��
		bs.init(samples,src);

	}

	if ( counter > trainingFrames + 1)
	{
		bs(src,dst);
	}

}

#endif