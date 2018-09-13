
//#define DllAPI _declspec(dllexport)

#include "kdtree.h"

KDTree::KDTree():_root(NULL){}

KDTree::~KDTree()
{ 
	destroyKDTree(_root);

	es.destroy(); 
}

int KDTree:: create( ExamplarSet &exm_set)
{
	es=exm_set;
	if((_root = createKDTree(exm_set, 0, exm_set.getSize())) == NULL)
		return 1;
	
	return 0;
}

int KDTree:: create( ExamplarSet &exm_set, const int dim )
{
	es=exm_set;
	if((_root = createKDTree(exm_set, dim, 0, exm_set.getSize())) == NULL)
		return 1;
	
	return 0;
}

 KDTreeNode* KDTree:: createKDTree( ExamplarSet &exm_set, int begin, int end )
{
	if(exm_set.empty())
		return NULL;
	if(end <= begin)
		return NULL;

	int dims = 2;
	int size = end - begin;

	//! 计算每个维的方差，选出方差值最大的维
	float var_max = -0.1f; 
	float avg, var;
	int dim_max_var = 0;
	for(int i=0;i<dims;i++)
	{
		avg = 0;
		var = 0;
		//! 求某一维的总和
		for(int j=begin;j<end;j++)
		{
			avg += exm_set[j][i];
		}
		//! 求平均
		avg /= size;
		//! 求方差
		for(int j=begin;j<end;j++)
		{
			var += ( exm_set[j][i] - avg ) * 
				( exm_set[j][i] - avg );
		}
		var /= size;
		if(var > var_max)
		{
			var_max = var;
			dim_max_var = i;
		}
	}

//	! 确定节点的数据矢量
	_HyperRectangle hr = exm_set.calculateRange(begin, end);
	exm_set.sortByDim(dim_max_var, begin, end);
	int mid = begin + size / 2;
	Point exm_split = exm_set.examplarAt(mid);

	//! 确定左右节点
	KDTreeNode *pNewNode = new KDTreeNode(0, 0, 0, dim_max_var, exm_split, hr);
	pNewNode->_left_child = createKDTree(exm_set,begin,mid);
	if(pNewNode->_left_child != NULL)
		pNewNode->_left_child->_parent = pNewNode;
	pNewNode->_right_child = createKDTree(exm_set,mid+1,end);
	if(pNewNode->_right_child != NULL)
		pNewNode->_right_child->_parent = pNewNode;

	return pNewNode;
}

 KDTreeNode* KDTree:: createKDTree( ExamplarSet &exm_set, const int dim, int begin, int end )
{
	if(exm_set.empty())
		return NULL;

	int dims = 2;
	int size = end - begin;

	int dim_max_var=dim;

	_HyperRectangle hr = exm_set.calculateRange(begin, end);
	exm_set.sortByDim(dim_max_var, begin, end);
	int mid = begin + size / 2;
	Point exm_split = exm_set.examplarAt(mid);

	//! 确定左右节点
	KDTreeNode *pNewNode = new KDTreeNode(0, 0, 0, dim_max_var, exm_split, hr);
	pNewNode->_left_child = createKDTree(exm_set,begin,mid);
	if(pNewNode->_left_child != NULL)
		pNewNode->_left_child->_parent = pNewNode;
	pNewNode->_right_child = createKDTree(exm_set,mid+1,end);
	if(pNewNode->_right_child != NULL)
		pNewNode->_right_child->_parent = pNewNode;

	return pNewNode;
}

void KDTree:: destroyKDTree( KDTreeNode *&root )
{
	if(root != NULL)
	{
		destroyKDTree(root->_left_child);
		destroyKDTree(root->_right_child);
		delete root;
	}
	root = NULL;
}

void KDTree:: destroy()
{
	destroyKDTree(_root);
	es.destroy();
}

ExamplarSet& KDTree::getExamplar(){return es;}

int KDTree::findKNN(int k, Point target, TopkSet& res, const Mahalanobis &dist )
{
	 res.construct(k);

	return innerGetClosestK(_root, target, res, dist);

}

int KDTree:: innerGetClosestK(KDTreeNode* pNode, Point point, TopkSet &ts, const Mahalanobis &dist )  
{  
 	if (NULL == pNode)  
        return 1;  
    float nCurDis = Distance_exm(point, pNode->getDomElt(), dist);  
	if (!ts.is_full() || ts.maxval() > nCurDis)  
    {  
		ts.insert(pNode->getDomElt().id, nCurDis); 
    }  

    if ((pNode->splitDim()==0 && point[0] <= pNode->getDomElt()[0]) || (pNode->splitDim()==1 && point[1] <= pNode->getDomElt()[1]))  
		innerGetClosestK(pNode->_left_child, point, ts, dist);  
    else  
		innerGetClosestK(pNode->_right_child, point, ts,dist);  

    float rang = pNode->splitDim()==0 ? (point[0] - pNode->getDomElt()[0])*(point[0] - pNode->getDomElt()[0])*dist.a11 
		: (point[1] - pNode->getDomElt()[1])*(point[1] - pNode->getDomElt()[1])*dist.a22;  
	if (ts.maxval() < rang)  
        return 2;  

    KDTreeNode* pGoInto = pNode->_left_child;  
    if ((pNode->splitDim()==0 && point[0] <= pNode->getDomElt()[0]) || (pNode->splitDim()==1 && point[1] <= pNode->getDomElt()[1]))  
        pGoInto = pNode->_right_child;  
    innerGetClosestK(pGoInto, point, ts, dist); 

	return 0;
}

void KDTree::interpret (const TopkSet &ts, float *&sys_load, float *&in_value, float *&distance, float *&value,	string *&strtime ) const
{
	const SourceData *src = NULL;

	const size_t N = ts.size();
	const Pair *pr = ts.getData();
	int id = 0;
	for (size_t i = 0; i != N; ++i) {

		id=pr[i].first;
		src = &(es.getData()[id]);

		sys_load[i] = es[id][0];
		in_value[i] = es[id][1];
		distance[i] = pr[i].second;
		value[i] = src->output;
		strtime[i] = src->getString();
	}
}

//void KDTree:: innerGetClosest(KDTreeNode* pNode, Point point, Point& res, float& nMinDis, const Mahalanobis &dist )  
//{  
// 	if (NULL == pNode)  
//        return;  
//    float nCurDis = Distance_exm(point, pNode->getDomElt(), dist);  
//    if (nMinDis < 0 || nCurDis < nMinDis)  
//    {  
//        nMinDis = nCurDis;  
//        res = pNode->getDomElt();  
//    }  
//
//    if ((pNode->splitDim()==0 && point[0] <= pNode->getDomElt()[0]) || (pNode->splitDim()==1 && point[1] <= pNode->getDomElt()[1]))  
//		innerGetClosest(pNode->_left_child, point, res, nMinDis, dist);  
//    else  
//		innerGetClosest(pNode->_right_child, point, res, nMinDis,dist);  
//
//    float rang = pNode->splitDim()==0 ? (point[0] - pNode->getDomElt()[0])*(point[0] - pNode->getDomElt()[0])*dist.a11 
//		: (point[1] - pNode->getDomElt()[1])*(point[1] - pNode->getDomElt()[1])*dist.a22;  
//    if (rang > nMinDis)  
//        return;  
//
//    KDTreeNode* pGoInto = pNode->_left_child;  
//    if ((pNode->splitDim()==0 && point[0] <= pNode->getDomElt()[0]) || (pNode->splitDim()==1 && point[1] <= pNode->getDomElt()[1]))  
//        pGoInto = pNode->_right_child;  
//    innerGetClosest(pGoInto, point, res, nMinDis, dist); 
//}  
//
// pair< Point, float> KDTree:: findNearest_i( KDTreeNode *root, Point target, const Mahalanobis &dist )
//{
//	//! 向下到达叶子节点
//
//	KDTreeNode *pSearch = root;
//
//	//! 堆栈用于保存搜索路径
//	 vector<KDTreeNode*> search_path;
//
//	Point nearest;
//
//	float max_dist;
//
//	while(pSearch != NULL)
//	{
//		search_path.push_back(pSearch);
//		int s = pSearch->splitDim();
//		if(target[s] <= pSearch->getDomElt()[s])
//		{
//			pSearch = pSearch->_left_child;
//		}
//		else
//		{
//			pSearch = pSearch->_right_child;
//		}
//	}
//
//	nearest = search_path.back()->getDomElt();
//	max_dist = Distance_exm(nearest, target, dist);
//
//	search_path.pop_back();
//
//	KDTreeNode *pBack;
//	//! 回溯搜索路径
//	while(!search_path.empty())
//	{
//		pBack = search_path.back();
//		search_path.pop_back();
//
//		if( pBack->_left_child == NULL && pBack->_right_child == NULL)
//		{
//			if( max_dist > Distance_exm(pBack->getDomElt(), target,dist) )
//			{
//				nearest = pBack->getDomElt();
//				max_dist = Distance_exm(pBack->getDomElt(), target, dist);
//			}
//		}
//		else
//		{
//			int s = pBack->splitDim();
//			//if( abs(pBack->getDomElt()[s] - target[s]) < max_dist)
//			//{
//				if( max_dist > Distance_exm(pBack->getDomElt(), target, dist) )
//				{
//					nearest = pBack->getDomElt();
//					max_dist = Distance_exm(pBack->getDomElt(), target, dist);
//				}
//				if(target[s] <= pBack->getDomElt()[s])
//					pSearch = pBack->_right_child;
//				else
//					pSearch = pBack->_left_child;
//				if(pSearch != NULL)
//					search_path.push_back(pSearch);
////			}
//		}
//	}
//
//	 pair<Point, float> res(nearest, max_dist);
//
//	return res;
//}
//

// pair< Point, float> KDTree::findNearest( Point target, const Mahalanobis &dist )
//{
//	 pair<Point, float> res;
//	if(_root == NULL)
//	{
//		res.second =  numeric_limits<float>::infinity();
//		return res;
//	}
//	Point ex;
//	float min = -1.0f;
//	innerGetClosest(_root,target,ex,min,dist);
//	res.first=ex;
//	res.second=min;
//	return res;
////	return findNearest_i(_root, target, dist);
//}
//
//int KDTree::findNearest( Point target, float range,  vector< pair<Point, float>> &res_nearest, const Mahalanobis &dist )
//{
//	return findNearest_range(_root, target, range, res_nearest, dist);
//}

//int KDTree::findNearest_range( KDTreeNode *root, Point target, float range, 
//	 vector< pair<Point, float>> &res_nearest, const Mahalanobis &dist )
//{
//	if(root == NULL)
//		return 0;
//	float dist_sq, dx;
//	int ret, added_res = 0;
//	dist_sq = 0;
//	dist_sq = Distance_exm(root->getDomElt(), target, dist);
//
//	if(dist_sq <= range) {
//		 pair<Point,float> temp(root->getDomElt(), dist_sq);
//		res_nearest.push_back(temp);
//
//		//! 结果个数+1
//
//		added_res = 1;
//	}
//
//	dx = target[root->splitDim()] - root->getDomElt()[root->splitDim()];
//	//! 左子树或右子树递归的查找
//	ret = findNearest_range(dx <= 0.0 ? root->_left_child : root->_right_child, target, range, res_nearest, dist);
//
//	float rang = root->splitDim()==0 ? (target[0] - root->getDomElt()[0])*(target[0] - root->getDomElt()[0])*dist.a11 
//		: (target[1] - root->getDomElt()[1])*(target[1] - root->getDomElt()[1])*dist.a22;
//	if(ret >= 0 && rang < range) {
//		added_res += ret;
//		ret = findNearest_range(dx <= 0.0 ? root->_right_child : root->_left_child, target, range, res_nearest, dist);
//	}
//
//	added_res += ret;
//	return added_res;
//}
//
