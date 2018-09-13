/*
 * Utilities.h
 *
 *  Created on: 2014-1-23
 *      Author: H. Wang & J.H. Song
 */

#ifndef HEADER_H_
#define HEADER_H_

#ifdef DllAPI  
	#define DllAPI _declspec(dllexport)
#else  
	#define DllAPI _declspec(dllimport)  
#endif

#include <iostream>
#include <time.h>
#include <string>
using namespace std;

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

/**
 * A integer-float pair, used during the kNN query processing.
 * The integer "first" is the ID of a data point, and the float "second" is the Mahalanobis distance
 * between the point specified by "first" and the query point.
 */
struct DllAPI Pair {
	Pair();
	Pair(int x, float y);

	int first;
	float second;

	Pair& operator= (const Pair &p);

	friend ostream& operator<< (ostream &out, const Pair &p);
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
  *�����ʾ���ݷ�Χ�ĳ����νṹ
  */
	struct DllAPI _HyperRectangle	
	{
		Point min;		//ͳ�����ݼ���������������ÿ��ά������Сֵ��ɵ�һ����������
		Point max;		//ͳ�����ݼ���������������ÿ��ά�������ֵ��ɵ�һ����������
		_HyperRectangle();
		_HyperRectangle(Point mx, Point mn);
		_HyperRectangle(const _HyperRectangle& rhs);
		_HyperRectangle& operator= (const _HyperRectangle& rhs);
		int create(Point mx, Point mn);
	};

/**
  *�������ݼ���
  */
	class DllAPI ExamplarSet	
	{
	private:
		Point *_ex_set;		//���庬�����ɸ�Point���������������ݼ�
		SourceData *data;				 //��������
		int _size;							//���ݼ���С

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

/**
 * Stores the k best-seen-so-far candidates during kNN query processing.
 * TopkSet is essentially an array of "Pair"s (see above), with extra two integers indicating (i) the
 * current size of the array and (ii) the capacity (i.e., maximum possible size) of the array.
 */
class DllAPI TopkSet {
public:
	TopkSet();
	~TopkSet();
	TopkSet& operator=(const TopkSet& rhs);

	/**
	 * Allocate a length-k array for a TopkSet object.
	 * @return 0: allocation successful;
	 *         1: allocation failure due to memory problems
	 */
	int construct(unsigned int k);

	/**
	 * Set the size of the array to 0.
	 * This function does NOT actually free the memory or erase the data.
	 */
	void clear();

	/**
	 * Free the memory.
	 */
	void destroy();

	/**
	 * Returns the capacity value.
	 */
	int capacity() const;

	/**
	 * Returns the current size.
	 */
	int size() const;

	/**
	 * Tells whether the array is full (i.e., current size = capacity) or not.
	 */
	bool is_full() const;

	/**
	 * Exposes the array.
	 */
	const Pair* getData() const;

	/**
	 * Inserts a Pair (id, val) into the array.
	 * The insertion has not effect when (i) the array is full and (ii) val is larger
	 * than the largest value in the array.
	 * @return 1: the Pair (id, val) is inserted into the array;
	 *         0: the Pair (id, val) is NOT inserted into the array;
	 */
	int insert(int id, float val);

	/**
	 * Returns current largest value in the array.
	 */
	float maxval() const;

	/**
	 * Print.
	 */
	void print(ostream &out) const;

protected:
	unsigned int m_capacity;
	unsigned int m_size;
	Pair *data;
};

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

		KDTreeNode(KDTreeNode *left_child, KDTreeNode *right_child,	KDTreeNode *parent, int split_dim, Point dom_elt, _HyperRectangle range_hr);

		KDTreeNode(const KDTreeNode &rhs);

		KDTreeNode& operator=(const KDTreeNode &rhs);		  //���ء�=�������

		Point& getDomElt();									  //��ȡ�ָ��

		_HyperRectangle& getHyperRectangle();				 //��ȡ���ݷ�Χ

		int& splitDim();									 //��ȡ�ָ�ά��

		void create(KDTreeNode *left_child, KDTreeNode *right_child, 		 //����һ���ڵ�
			KDTreeNode *parent, int split_dim, Point dom_elt,  _HyperRectangle range_hr);
	};

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


#endif
