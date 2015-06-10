/************************************************************************/
/*                      DBSCAN          非监督聚类算法                  */
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
	int                   minPts;//邻域包含的最小个数
	float                 dis;
	vector<data>          dataset;//整个数据集
	vector<pt>           outliers;//离群点
	vector< vector<pt> > clusters;//已分好的类

public:
	DBSCAN();
	DBSCAN(vector<pt> , float distance, int minPts);
	void expandCluster( pt p, vector<int> neighbour, int classNUM );//
	vector<int> regionQuery( data p);//将该点邻域内的其它点的编号返回
	void operator()( vector<pt> _data);
	void init( vector<pt> _data);//将数据转变成带有bool标记的数据集
	void displayClusters();
	vector< vector<pt>> getClusters();
};


#endif
#pragma   once 
