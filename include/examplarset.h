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

	Point& operator=(const Point& rhs);	//����"="�����
		
	float& dataAt(int dim);	//������ʿ��ƺ���
	
	float& operator[](int dim);	//����"[]"�������ʵ���±����

	const float& dataAt(int dim) const;	//����ֻ�����ʺ���

	const float& operator[](int dim) const;	//����"[]"�������ʵ���±�ֻ������

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

	short int& dataAt(int dim);	//������ʿ��ƺ���
	
	short int& operator[](int dim);	//����"[]"�������ʵ���±����
};

	float Distance_exm(const Point &x, const Point &y, const Mahalanobis &dist );	//����ľ��뺯��,�����õ��������Ͼ���

	class ExamplarCompare	//�������������Ƚ��࣬�����Ķ�������sort��comp
	{
	public:
		ExamplarCompare(const int dim) : _dim(dim){}	//�����dim��ָ���Ƚϵķ���
		bool
			operator()(const Point &x, const Point &y) const
		{
			return x[_dim] < y[_dim];
		}
	private:
		int _dim;	// don't make this const so that an assignment operator can be auto-generated
	};


	struct DllAPI _HyperRectangle	//�����ʾ���ݷ�Χ�ĳ����νṹ
	{
		Point min;		//ͳ�����ݼ���������������ÿ��ά������Сֵ��ɵ�һ����������
		Point max;		//ͳ�����ݼ���������������ÿ��ά�������ֵ��ɵ�һ����������
		_HyperRectangle();
		_HyperRectangle(Point mx, Point mn);
		_HyperRectangle(const _HyperRectangle& rhs);
		_HyperRectangle& operator= (const _HyperRectangle& rhs);
		int create(Point mx, Point mn);
	};

	class DllAPI ExamplarSet	//�������ݼ���
	{
	private:
		Point *_ex_set;
//		std::vector<Point> _ex_set;		//���庬�����ɸ�Point���������������ݼ�
		SourceData *data;				 //��������
		int _size;							//���ݼ���С
//		int _dims;							//���ݼ���ÿ������������ά��
	public:
		ExamplarSet();

		ExamplarSet& operator=(const ExamplarSet& rhs);		  //���ء�=��������

		~ExamplarSet();

		/*�������ݼ���ʵ�����ǵ���releaseExamplarSetMem*/
		void destroy();

		Point& examplarAt(int idx);				  //������ʿ��ƺ���
		Point& operator[](int idx);				  //����"[]"�������ʵ���±����

		const Point& examplarAt(int idx) const;		  //����ֻ�����ʺ���
		const Point& operator[](int idx) const;		  //����"[]"�������ʵ���±�ֻ������

		int create(int size);						  //�����ݼ������ڴ�

		int getSize() const;						  //��ȡ���ݼ���С
		SourceData* getData() const;				  //��ȡ��������

		_HyperRectangle calculateRange(int begin, int end);		  //��ȡbegin-end�����ݵķ�Χ

		bool empty() const;										  //���ݼ��Ƿ�Ϊ��

		int sortByDim(int dim, int begin, int end);	//��ĳ������ά��������

		int readData(char *strFilePath);	//���ļ���ȡ���ݼ�

	private:
		void releaseExamplarSetMem();		//����������ݼ����ͷ��ڴ�

	};
//}

#endif