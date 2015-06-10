/************************************************************************/
/*                      DBSCAN          �Ǽල�����㷨                  */
/************************************************************************/



#include <iostream>
#include <vector>
#include <assert.h>
#include "PT.h"
#include <opencv2/core/core.hpp>

#ifndef _DBSCAN_H
#define _DBSCAN_H

using namespace std;


class  DBSCAN
{
private:
	int                   minPts;//�����������С����
	float                 dis;
	vector<data>          dataset;//�������ݼ�
	vector<pt>           outliers;//��Ⱥ��
	vector< vector<pt> > clusters;//�ѷֺõ���

public:
	DBSCAN();
	DBSCAN(vector<pt> , float distance, int minPts);
	void expandCluster( pt p, vector<int> neighbour, int classNUM );//
	vector<int> regionQuery( data p);//���õ������ڵ�������ı�ŷ���
	void operator()( vector<pt> _data);
	void init( vector<pt> _data);//������ת��ɴ���bool��ǵ����ݼ�
	void displayClusters();
	vector< vector<pt>> getClusters();
};


#endif
#pragma   once 
