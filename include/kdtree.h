/*! \file kdtree.h
    \brief declaration of KDTree
    
    \author RaySaint 
	\modified by Jinhua Song
*/

#ifndef KDTREE_H_H
#define KDTREE_H_H

#include "kdtreenode.h"
#include "TopkSet.h"

//namespace KDTree
//{
/**
  *k-d tree结构定义
  */
	class DllAPI KDTree	
	{
	private:
		KDTreeNode *_root;		//k-d tree的根节点
		ExamplarSet es; 		//Kd-tree的数据集
	public:														  
		KDTree();

		/*创建k-d tree，实际上调用createKDTree（exm_set, begin, end ）*/
		int create(ExamplarSet &exm_set);			

		/*创建k-d tree，实际上调用createKDTree（exm_set, dim, begin, end）,dim是第一次分割数据集时用到的分割维度*/
		int create(ExamplarSet &exm_set, const int dim);

	   /*销毁k-d tree，实际上调用destroyKDTree*/
		void destroy();			

		~KDTree();

		ExamplarSet& getExamplar();					  //获取数据集

		/*查找k个最近邻点，返回成功与否的标识，k近邻存放在res中，实际是调用innerGetClosestK*/
		int findKNN(int k, Point target, TopkSet& res, const Mahalanobis &dist );	

		/*解析k近邻为查询结果的函数*/
		void interpret (const TopkSet &ts, float *&sys_load, float *&in_value, float *&distance, float *&value, string *&strtime ) const;

	private:
		/**根据数据集exm_set创建kd-tree；
		  *begin和end是用到的数据集中数据的范围，从begin开始到end结束，这两个参数是用于递归的
		  *分裂维度是根据数据集的方差选择的，每次都是选择方差大的一维作为分裂维度
		  */
		KDTreeNode* createKDTree(ExamplarSet &exm_set, int begin, int end );

		/**根据数据集exm_set创建kd-tree；
		  *begin和end是用到的数据集中数据的范围，从begin开始到end结束，这两个参数是用于递归的
		  *第一次分裂的分裂维度是由dim指定，之后的分裂维度轮换
		  */
		KDTreeNode* createKDTree(ExamplarSet &exm_set, const int dim, int begin, int end );

		/*销毁kd-tree，释放内存*/
		void destroyKDTree(KDTreeNode *&root);

		/*查找k近邻*/
		int innerGetClosestK(KDTreeNode* pNode, Point point, TopkSet &ts, const Mahalanobis &dist );

	};

//}

#endif