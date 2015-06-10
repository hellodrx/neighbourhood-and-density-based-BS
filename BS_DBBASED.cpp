#include "BS_DBBASED.h"

//本次实验的背景模型是在一个集合中放入九对元素，每一对都是最大，最小值


static int k = 5;
static int neighbour = k*k;
static int dis = 4*4*15*15;
static int v = 2;




BS::BS()
{

}

BS::BS(vector<vector<pt>> input, const Mat& firstFrame)
{
	init(input,firstFrame);

	s = firstFrame.size();
}

void BS::init(vector<vector<pt>> input, const Mat& firstFrame)
{
	//现在只接受固定数据结构的数据集

	CV_Assert(input.size() != 0);	
    model = input;//整个模型都是input

}

void BS::update( const Mat&, Mat&)
{

}

void BS::operator()( const Mat& src, Mat& dst)
{
	CV_Assert( src.data);

	Size ssrc = src.size();
	dst.create(ssrc,CV_8U);
	dst = Scalar::all(0);

	CV_Assert(model.size() != NULL);

	getForeground(src,dst);//提取前景
}

void BS::getForeground(const Mat& src, Mat& dst)
{
	CV_Assert( src.size() == dst.size());
	CV_Assert( src.channels() == 3);

	int rows = src.rows;
	int cols = src.cols;

	Mat_ <Vec3b> temp = src; 
	Mat_ <Vec3b> _dst(temp.size(),CV_8UC3);

	int index = 0;//当前为第k个像素

	for ( int i = 0; i < rows; i++)
	{
		
		uchar* pDst = dst.ptr<uchar>(i);

		for ( int j = 0; j < cols ; j++)
		{
			int m = 0;//赞成票计数器
			bool isB = false;//标记该像素是否为背景模型

			Vec3b val= temp(i,j);

			
			computeVotes( rows,cols,index,val,m, isB);

			if ( isB)//如果为背景，则该点为0
			{
				pDst[j] = 0;
				_dst(i,j) = Vec3b(0,0,0);
			}
			else//为前景
			{
				pDst[j] = 255; 
				_dst(i,j)[0] = val[0];
				_dst(i,j)[1] = val[1];
				_dst(i,j)[2] = val[2];
			}

			index++;
		}

	}

	imshow( "3 channels",_dst);
}

void BS::computeVotes(int rows, int cols, int index,Vec3b val,int& votes, bool& isB)
{
	CV_Assert( votes == 0);

	int extend = k/2;

	int i = index/cols;
	int j = index%cols;

	for ( int ii = i - extend; ii <= i + extend; ii++)
	{
		if ( isB)
		{
			break;
		}

		for ( int jj = j -extend; jj <= j + extend; jj++)
		{
			if ( isB)
			{
				break;
			}

			if ( ii < 0 || ii > rows - 1 || jj < 0 || jj > cols -1)//确保不越界
			{
				continue;
			}
			else
			{
				int s = model[index].size();

				for ( int iii = 0; iii < s; iii++)
				{
					if ( dist2( model[index][iii], val) < dis)
					{
						votes++;
						
						if ( votes >= v)
						{
							isB = true;
							break;
						}
					}
				}
							
			}
		}
	}


}