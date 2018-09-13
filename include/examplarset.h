/*! \file examplarset.h
    \brief declaration of ExamplarSet
    
    \author RaySaint 
	\modified by Jinhua Song

*/

#ifndef EXAMPLARSET_H_H
#define EXAMPLARSET_H_H

  
#define DllAPI _declspec(dllexport)

#include <memory.h>
#include <stdio.h>
#include "assert.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;
//namespace KDTree
//{

/**
 * Data points in the R-tree.
 * A data point is a triplet (id, x, y) where id is an internal ID of the point, x the system load, and y
 * the input concentration value.
 */
struct DllAPI Point {
	Point();
	Point(int a, float b, float c);

	int id;
	float x;
	float y;

	Point& operator=(const Point& rhs);	//重载"="运算符
		
	float& dataAt(int dim);	//定义访问控制函数
	
	float& operator[](int dim);	//重载"[]"运算符，实现下标访问

	const float& dataAt(int dim) const;	//定义只读访问函数

	const float& operator[](int dim) const;	//重载"[]"运算符，实现下标只读访问

	friend ostream& operator<< (ostream &out, const Point &p);

	static const int size = sizeof(int) + 2 * sizeof(float);
};

/**
 * Defines the Mahalanobis distance with a matrix of A = [a11, a12; a21, a22], where a21 = a12.
 */
struct DllAPI Mahalanobis {
	Mahalanobis(float x, float y, float z);

	float a11;
	float a12;
	float a22;

	// Computes the Mahalanobis distance between points p1 and p2.
	float operator() (const Point &p1, const Point &p2) const;
};

/**
 * Stores additional data (i.e., output concentration values, timestamps, etc.) of a point in the
 * R-tree.
 */
struct DllAPI SourceData {
	SourceData();
	SourceData(float o, short int mo, short int d, short int y, short int h, short int mi);

	float output;

	short int month;
	short int day;
	short int year;
	short int hour;
	short int minute;

	string getString() const;

	short int& dataAt(int dim);	//定义访问控制函数
	
	short int& operator[](int dim);	//重载"[]"运算符，实现下标访问
};

	float Distance_exm(const Point &x, const Point &y, const Mahalanobis &dist );	//定义的距离函数,这里用到的是马氏距离

	class ExamplarCompare	//定义数据向量比较类，产生的对象用于sort的comp
	{
	public:
		ExamplarCompare(const int dim) : _dim(dim){}	//这里的dim是指待比较的方向
		bool
			operator()(const Point &x, const Point &y) const
		{
			return x[_dim] < y[_dim];
		}
	private:
		int _dim;	// don't make this const so that an assignment operator can be auto-generated
	};


	struct DllAPI _HyperRectangle	//定义表示数据范围的超矩形结构
	{
		Point min;		//统计数据集中所有数据向量每个维度上最小值组成的一个数据向量
		Point max;		//统计数据集中所有数据向量每个维度上最大值组成的一个数据向量
		_HyperRectangle();
		_HyperRectangle(Point mx, Point mn);
		_HyperRectangle(const _HyperRectangle& rhs);
		_HyperRectangle& operator= (const _HyperRectangle& rhs);
		int create(Point mx, Point mn);
	};

	class DllAPI ExamplarSet	//整个数据集类
	{
	private:
		Point *_ex_set;
//		std::vector<Point> _ex_set;		//定义含有若干个Point类数据向量的数据集
		SourceData *data;				 //其他数据
		int _size;							//数据集大小
//		int _dims;							//数据集中每个数据向量的维度
	public:
		ExamplarSet();

		ExamplarSet& operator=(const ExamplarSet& rhs);		  //重载“=”操作符

		~ExamplarSet();

		/*销毁数据集，实际上是调用releaseExamplarSetMem*/
		void destroy();

		Point& examplarAt(int idx);				  //定义访问控制函数
		Point& operator[](int idx);				  //重载"[]"运算符，实现下标访问

		const Point& examplarAt(int idx) const;		  //定义只读访问函数
		const Point& operator[](int idx) const;		  //重载"[]"运算符，实现下标只读访问

		int create(int size);						  //给数据集分配内存

		int getSize() const;						  //获取数据集大小
		SourceData* getData() const;				  //获取其他数据

		_HyperRectangle calculateRange(int begin, int end);		  //获取begin-end的数据的范围

		bool empty() const;										  //数据集是否为空

		int sortByDim(int dim, int begin, int end);	//按某个方向维的排序函数

		int readData(char *strFilePath);	//从文件读取数据集

	private:
		void releaseExamplarSetMem();		//清除现有数据集，释放内存

	};
//}

#endif