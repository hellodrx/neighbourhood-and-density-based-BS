#include "BS_DBBASED.h"

//����ʵ��ı���ģ������һ�������з���Ŷ�Ԫ�أ�ÿһ�Զ��������Сֵ


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
	//����ֻ���̶ܹ����ݽṹ�����ݼ�

	CV_Assert(input.size() != 0);	
    model = input;//����ģ�Ͷ���input

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

	getForeground(src,dst);//��ȡǰ��
}

void BS::getForeground(const Mat& src, Mat& dst)
{
	CV_Assert( src.size() == dst.size());
	CV_Assert( src.channels() == 3);

	int rows = src.rows;
	int cols = src.cols;

	Mat_ <Vec3b> temp = src; 
	Mat_ <Vec3b> _dst(temp.size(),CV_8UC3);

	int index = 0;//��ǰΪ��k������

	for ( int i = 0; i < rows; i++)
	{
		
		uchar* pDst = dst.ptr<uchar>(i);

		for ( int j = 0; j < cols ; j++)
		{
			int m = 0;//�޳�Ʊ������
			bool isB = false;//��Ǹ������Ƿ�Ϊ����ģ��

			Vec3b val= temp(i,j);

			
			computeVotes( rows,cols,index,val,m, isB);

			if ( isB)//���Ϊ��������õ�Ϊ0
			{
				pDst[j] = 0;
				_dst(i,j) = Vec3b(0,0,0);
			}
			else//Ϊǰ��
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

			if ( ii < 0 || ii > rows - 1 || jj < 0 || jj > cols -1)//ȷ����Խ��
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