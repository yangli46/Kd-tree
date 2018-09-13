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
  *k-d tree�ṹ����
  */
	class DllAPI KDTree	
	{
	private:
		KDTreeNode *_root;		//k-d tree�ĸ��ڵ�
		ExamplarSet es; 		//Kd-tree�����ݼ�
	public:														  
		KDTree();

		/*����k-d tree��ʵ���ϵ���createKDTree��exm_set, begin, end ��*/
		int create(ExamplarSet &exm_set);			

		/*����k-d tree��ʵ���ϵ���createKDTree��exm_set, dim, begin, end��,dim�ǵ�һ�ηָ����ݼ�ʱ�õ��ķָ�ά��*/
		int create(ExamplarSet &exm_set, const int dim);

	   /*����k-d tree��ʵ���ϵ���destroyKDTree*/
		void destroy();			

		~KDTree();

		ExamplarSet& getExamplar();					  //��ȡ���ݼ�

		/*����k������ڵ㣬���سɹ����ı�ʶ��k���ڴ����res�У�ʵ���ǵ���innerGetClosestK*/
		int findKNN(int k, Point target, TopkSet& res, const Mahalanobis &dist );	

		/*����k����Ϊ��ѯ����ĺ���*/
		void interpret (const TopkSet &ts, float *&sys_load, float *&in_value, float *&distance, float *&value, string *&strtime ) const;

	private:
		/**�������ݼ�exm_set����kd-tree��
		  *begin��end���õ������ݼ������ݵķ�Χ����begin��ʼ��end���������������������ڵݹ��
		  *����ά���Ǹ������ݼ��ķ���ѡ��ģ�ÿ�ζ���ѡ�񷽲���һά��Ϊ����ά��
		  */
		KDTreeNode* createKDTree(ExamplarSet &exm_set, int begin, int end );

		/**�������ݼ�exm_set����kd-tree��
		  *begin��end���õ������ݼ������ݵķ�Χ����begin��ʼ��end���������������������ڵݹ��
		  *��һ�η��ѵķ���ά������dimָ����֮��ķ���ά���ֻ�
		  */
		KDTreeNode* createKDTree(ExamplarSet &exm_set, const int dim, int begin, int end );

		/*����kd-tree���ͷ��ڴ�*/
		void destroyKDTree(KDTreeNode *&root);

		/*����k����*/
		int innerGetClosestK(KDTreeNode* pNode, Point point, TopkSet &ts, const Mahalanobis &dist );

	};

//}

#endif