#include "DBSCAN.h"





DBSCAN::DBSCAN()
{
	dis = 15;
	minPts = 3;
}

DBSCAN::DBSCAN(vector<pt> _data, float distance, int _minPts)
{
	init(_data);

	minPts = _minPts;
	dis = distance;

}

void DBSCAN::expandCluster( pt p, vector<int> neighbour, int classNUM)
{
	clusters[classNUM].push_back(p);

	int size = neighbour.size();

	for ( int i = 0; i < size; i++)
	{
		if ( dataset[neighbour[i]].isvisited)//����õ��Ѿ������ʹ�
		{
			continue;
		}
		else
		{
			dataset[neighbour[i]].isvisited = true;

			vector<int> _neighbour= regionQuery( dataset[neighbour[i]]);


			if ( _neighbour.size() > minPts)
			{
				//�ϲ�_neighbour��neighbour
				neighbour.insert(neighbour.end(), _neighbour.begin(),_neighbour.end());
			}

			clusters[classNUM].push_back(dataset[neighbour[i]].a);//��Ϊ��ǰ��֮ǰû�з��ʣ�����Ҳû�з��ֱ࣬�ӱ��������ǰ��

		}
	}
}

vector<int> DBSCAN::regionQuery( data p)
{
	int size = dataset.size();
	vector<int> nums;//it stores the order numbers in dataset of p's neighgbours

	for( int i = 0; i < size; i++)
	{
		if ( distPT(dataset[i].a, p.a) < dis)
		{
			nums.push_back(i);
		}
	}

	return nums;
}

void DBSCAN::operator()(vector<pt> _data)
{
	assert( _data.size() != 0);
	if ( dataset.size() == 0)
	{
		init( _data);
	}

	int C = -1;//�����
	int size = _data.size();

	for ( int i = 0; i < size; i++)
	{
		if ( dataset[i].isvisited == true)//����õ��ѱ����ʹ�
		{
			continue;
		}
		else//���û������
		{
			dataset[i].isvisited = true;
			vector<int> neighbour = regionQuery( dataset[i]);
			if ( neighbour.size() < minPts)//�������Ⱥ��
			{
				outliers.push_back(dataset[i].a);
			}
			else//����Ǻ��ĵ�
			{
				C++;
				clusters.push_back( vector<pt>());//��ʼ��ÿһ�����
				expandCluster(dataset[i].a, neighbour, C);//neighbour��ֻ�洢���ݵı��
			}
		}
	}
}

void DBSCAN::init(vector<pt> _data)
{
	assert( _data.size() != 0);//��֤���ǿռ�

	int size = _data.size();

	for ( int i = 0; i < size; i++)
	{
		data temp;
		temp.a = _data[i];
		temp.isvisited = false;

		dataset.push_back(temp);
	}
}

void DBSCAN::displayClusters()//display clusters' members and outliers'
{
	int m_clusters = clusters.size();

	cout<<"CLUSTERS"<<endl;
	for ( int i = 0; i < m_clusters; i++)
	{
		int temp = clusters[i].size();
		cout<<" /******************This is the "<<i<<"th cluster.***************************/"<<endl;

		for ( int j = 0; j < temp; j++)
		{
			cout<<clusters[i][j].a<<" "<<clusters[i][j].b<<" "<<clusters[i][j].c<<endl;
		}

		cout<<endl;
	}

	int m_outliers =outliers.size();

	cout<<endl<<endl<<" OUTLIERS"<<endl;
	for ( int ii = 0; ii < m_outliers; ii++)
	{
		cout<<outliers[ii].a<<" "<<outliers[ii].b<<" "<<outliers[ii].c<<endl;
	}
}

vector< vector<pt>> DBSCAN::getClusters()
{
	return clusters;
}