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
  *定义树节点
  */
	class DllAPI KDTreeNode
	{
	private:
		int _split_dim;		     //分割维度
		Point _dom_elt;			 //分割数据点
		_HyperRectangle _range_hr;	 //数据范围
	public:
		KDTreeNode *_left_child, *_right_child;		  //左右子节点
		KDTreeNode *_parent;						  //父结点
	public:
		KDTreeNode();								 //构造函数
//		~KDTreeNode(){}
		KDTreeNode(KDTreeNode *left_child, KDTreeNode *right_child,	KDTreeNode *parent, int split_dim, Point dom_elt, _HyperRectangle range_hr);

		KDTreeNode(const KDTreeNode &rhs);

		KDTreeNode& operator=(const KDTreeNode &rhs);		  //重载“=”运算符

		Point& getDomElt();									  //获取分割点

		_HyperRectangle& getHyperRectangle();				 //获取数据范围

		int& splitDim();									 //获取分割维度

		void create(KDTreeNode *left_child, KDTreeNode *right_child, 		 //创建一个节点
			KDTreeNode *parent, int split_dim, Point dom_elt,  _HyperRectangle range_hr);
	};
//}

#endif