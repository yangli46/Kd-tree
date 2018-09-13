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

	Point& operator=(const Point& rhs);	//重载"="运算符
		
	float& dataAt(int dim);	//定义访问控制函数
	
	float& operator[](int dim);	//重载"[]"运算符，实现下标访问

	const float& dataAt(int dim) const;	//定义只读访问函数

	const float& operator[](int dim) const;	//重载"[]"运算符，实现下标只读访问

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

	short int& dataAt(int dim);	//定义访问控制函数
	
	short int& operator[](int dim);	//重载"[]"运算符，实现下标访问
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
  *定义表示数据范围的超矩形结构
  */
	struct DllAPI _HyperRectangle	
	{
		Point min;		//统计数据集中所有数据向量每个维度上最小值组成的一个数据向量
		Point max;		//统计数据集中所有数据向量每个维度上最大值组成的一个数据向量
		_HyperRectangle();
		_HyperRectangle(Point mx, Point mn);
		_HyperRectangle(const _HyperRectangle& rhs);
		_HyperRectangle& operator= (const _HyperRectangle& rhs);
		int create(Point mx, Point mn);
	};

/**
  *整个数据集类
  */
	class DllAPI ExamplarSet	
	{
	private:
		Point *_ex_set;		//定义含有若干个Point类数据向量的数据集
		SourceData *data;				 //其他数据
		int _size;							//数据集大小

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

		KDTreeNode(KDTreeNode *left_child, KDTreeNode *right_child,	KDTreeNode *parent, int split_dim, Point dom_elt, _HyperRectangle range_hr);

		KDTreeNode(const KDTreeNode &rhs);

		KDTreeNode& operator=(const KDTreeNode &rhs);		  //重载“=”运算符

		Point& getDomElt();									  //获取分割点

		_HyperRectangle& getHyperRectangle();				 //获取数据范围

		int& splitDim();									 //获取分割维度

		void create(KDTreeNode *left_child, KDTreeNode *right_child, 		 //创建一个节点
			KDTreeNode *parent, int split_dim, Point dom_elt,  _HyperRectangle range_hr);
	};

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


#endif
