/*! \file kdtreenode.h
    \brief declaration of KDTreeNode
    
    \author RaySaint 
	\modified by Jinhua Song
*/

#ifndef KDTREENODE_H_H
#define KDTREENODE_H_H

#include "examplarset.h"
using namespace std;
//namespace KDTree
//{
/**
  *�������ڵ�
  */
	class DllAPI KDTreeNode
	{
	private:
		int _split_dim;		     //�ָ�ά��
		Point _dom_elt;			 //�ָ����ݵ�
		_HyperRectangle _range_hr;	 //���ݷ�Χ
	public:
		KDTreeNode *_left_child, *_right_child;		  //�����ӽڵ�
		KDTreeNode *_parent;						  //�����
	public:
		KDTreeNode();								 //���캯��
//		~KDTreeNode(){}
		KDTreeNode(KDTreeNode *left_child, KDTreeNode *right_child,	KDTreeNode *parent, int split_dim, Point dom_elt, _HyperRectangle range_hr);

		KDTreeNode(const KDTreeNode &rhs);

		KDTreeNode& operator=(const KDTreeNode &rhs);		  //���ء�=�������

		Point& getDomElt();									  //��ȡ�ָ��

		_HyperRectangle& getHyperRectangle();				 //��ȡ���ݷ�Χ

		int& splitDim();									 //��ȡ�ָ�ά��

		void create(KDTreeNode *left_child, KDTreeNode *right_child, 		 //����һ���ڵ�
			KDTreeNode *parent, int split_dim, Point dom_elt,  _HyperRectangle range_hr);
	};
//}

#endif