
//#define DllAPI _declspec(dllexport)

#include "examplarset.h"


Point::Point(): id(0), x(0.0), y(0.0) {}

Point::Point(int a, float b, float c): id(a), x(b), y(c) {}

 Point&  Point::operator=(const Point& rhs)	//重载"="运算符
{
	if(this == &rhs) 
		return *this;
	x = rhs.x;
	y = rhs.y;
	id = rhs.id;

	return *this;
}

float& Point::dataAt(int dim)	//定义访问控制函数
{
	assert(dim < 2);
	return dim==0? x : y;
}

float& Point::operator[](int dim)	//重载"[]"运算符，实现下标访问
{
	return dataAt(dim);
}

const float& Point::dataAt(int dim) const	//定义只读访问函数
{
	assert(dim < 2);
	return dim==0? x : y;
}

const float& Point::operator[](int dim) const	//重载"[]"运算符，实现下标只读访问
{
	return dataAt(dim);
}

ostream& operator<< (ostream &out, const Point &p)
{
	out << "(" << p.id << ", " << p.x << ", " << p.y << ")";
	return out;
}

SourceData::SourceData(): output(0.0), month(0), day(0), year(0), hour(0), minute(0)
{}

SourceData::SourceData(float o, short int mo, short int d, short int y, short int h, short int mi):
	output(o), month(mo), day(d), year(y), hour(h), minute(mi)
{}

string SourceData::getString() const
{
	stringstream ss;
	ss << year << "-";
	if (month < 10) ss << 0;
	ss << month << "-";
	if (day < 10) ss << 0;
	ss << day << " ";
	if (hour < 10) ss << 0;
	ss << hour << ":";
	if (minute < 10) ss << 0;
	ss << minute;

	return ss.str();
}

short int& SourceData::dataAt(int dim)	//定义访问控制函数
{
	assert(dim < 5);
	if(dim==0)
		return month;
	if(dim==1)
		return day;
	if(dim==2)
		return year;
	if(dim==3)
		return hour;
	else
		return minute;
}

short int& SourceData::operator[](int dim)	//重载"[]"运算符，实现下标访问
{
	return dataAt(dim);
}


_HyperRectangle::_HyperRectangle() {}

_HyperRectangle::_HyperRectangle(Point mx, Point mn)
{
	min = mn;
	max = mx;
}

_HyperRectangle::_HyperRectangle(const _HyperRectangle& rhs)
{
	min = rhs.min;
	max = rhs.max;
}

_HyperRectangle& _HyperRectangle::operator= (const _HyperRectangle& rhs)
{
	if(this == &rhs)
		return *this;
	min = rhs.min;
	max = rhs.max;
	return *this;
}

int _HyperRectangle::create(Point mx, Point mn)
{
	min = mn;
	max = mx;
	return 0;
}


ExamplarSet::ExamplarSet():data(NULL),_ex_set(NULL),_size(0){}

ExamplarSet::~ExamplarSet(){releaseExamplarSetMem();}

void ExamplarSet::destroy(){releaseExamplarSetMem();}

 Point& ExamplarSet::examplarAt(int idx)
{ 
	assert(idx < _size);
	return _ex_set[idx]; 
}

Point& ExamplarSet::operator[](int idx)
{
	return examplarAt(idx);
}

const Point& ExamplarSet::examplarAt(int idx) const
{
	assert(idx < _size);
	return _ex_set[idx];
}

const Point& ExamplarSet::operator[](int idx) const
{
	return examplarAt(idx);
}

 ExamplarSet&  ExamplarSet::operator=( const ExamplarSet& rhs )
{

	if(this == &rhs) 
		return *this;

	releaseExamplarSetMem();

	if(rhs._size > 0)
		_size = rhs._size;
	else
		_size = 0;

	if(_size>0)
	{
		_ex_set = new Point[_size];
		data = new SourceData[_size];
	}

	for(int i=0;i<_size;i++)
	{
		_ex_set[i] = rhs._ex_set[i];
		data[i] = rhs.data[i];
	}

	return *this;
}

int  ExamplarSet::create( int size)
{
	releaseExamplarSetMem();

	if(size > 0)
	{  
		_size = size;
		if((_ex_set = new Point[size])==NULL) return 1;
		if((data = new SourceData[size])==NULL) return 1;
		
		return 0;
	}
	return 1;
}

int ExamplarSet::getSize() const { return _size;}

SourceData* ExamplarSet::getData() const {return data;}

 _HyperRectangle  ExamplarSet::calculateRange(int begin, int end)
{
	assert(_ex_set != NULL);
	assert(_size > 0);
	assert(end > begin);

	Point mn;

	Point mx;


	for(int j=0;j<2;j++)
	{
		mn.dataAt(j) = (*this)[0][j];	//初始化最小范围向量
		mx.dataAt(j) = (*this)[0][j];	//初始化最大范围向量
	}

	for(int i=begin+1;i<end;i++)
	{
		for(int j=0;j<2;j++)
		{
			if( (*this)[i][j] < mn[j] )
				mn[j] = (*this)[i][j];
			if( (*this)[i][j] > mx[j] )
				mx[j] = (*this)[i][j];
		}
	}
	_HyperRectangle hr(mx, mn);

	return hr;
}

 bool ExamplarSet::empty() const
{
	return (_size == 0);
}

int ExamplarSet::sortByDim( int dim, int begin, int end )
{
	ExamplarCompare cmp(dim);
	sort(_ex_set+begin, _ex_set+end, cmp);
	return 0;
}

int  ExamplarSet::readData( char *strFilePath )
{
	ifstream fin(strFilePath, ios::binary);
	if (!fin.is_open()) {
		cerr << "[RTree::construct] Cannot open file " << strFilePath << ".\n";
		exit(1);
	}
	assert(fin != NULL);

	int N=_size;
	fin.read(reinterpret_cast<char*>(&N), sizeof(unsigned int));

	releaseExamplarSetMem();
	create(N);

	for (int j = 0; j != N; ++j) _ex_set[j].id = j;

	float *f_buffer = new float [N];

	fin.read(reinterpret_cast<char*>(f_buffer), N * sizeof(float));
	for (int j = 0; j != N; ++j) 
		(*this)[j][0] = f_buffer[j];

	fin.read(reinterpret_cast<char*>(f_buffer), N * sizeof(float));
	for (int j = 0; j != N; ++j) (*this)[j][1] = f_buffer[j];

	fin.read(reinterpret_cast<char*>(f_buffer), N * sizeof(float));
	for (int j = 0; j != N; ++j) data[j].output = f_buffer[j];

	delete [] f_buffer;
	short int *i_buffer = new short int [N];

	fin.read(reinterpret_cast<char*>(i_buffer), N * sizeof(short int));
	for (int j = 0; j != N; ++j) data[j][0] = i_buffer[j];

	fin.read(reinterpret_cast<char*>(i_buffer), N * sizeof(short int));
	for (int j = 0; j != N; ++j) data[j][1] = i_buffer[j];

	fin.read(reinterpret_cast<char*>(i_buffer), N * sizeof(short int));
	for (int j = 0; j != N; ++j) data[j][2] = i_buffer[j];

	fin.read(reinterpret_cast<char*>(i_buffer), N * sizeof(short int));
	for (int j = 0; j != N; ++j) data[j][3] = i_buffer[j];

	fin.read(reinterpret_cast<char*>(i_buffer), N * sizeof(short int));
	for (int j = 0; j != N; ++j) data[j][4] = i_buffer[j];

	delete [] i_buffer;
	fin.close();

	return 0;
}

void ExamplarSet::releaseExamplarSetMem()		//清除现有数据集
{
	if(_ex_set!=NULL)
		delete[] _ex_set;
	if(data!=NULL)
		delete[] data; 

	_ex_set=NULL;
	data=NULL;
	_size = 0;
}

 Mahalanobis::Mahalanobis(float x, float y, float z): a11(x), a12(y), a22(z) {}

float  Distance_exm( const Point &x, const Point &y, const Mahalanobis &dist )
{
	//float dis;
	//if(x.getDomDims() == y.getDomDims() && x.getDomDims() > 0)
	//{
	//	dis = 0.0;
	//	for(int i=0;i<x.getDomDims();i++)
	//	{
	//		dis += (x[i] - y[i]) * (x[i] - y[i]);
	//	}
	//	dis = sqrt(dis);
	//}
	//else
	//	dis = -1.0;
	//return dis;
	return sqrt(dist.a11 * (x[0]-y[0]) * (x[0]-y[0]) + dist.a22 * (x[1]-y[1])*(x[1]-y[1])+2*dist.a12*(x[0]-y[0])*(x[1]-y[1]));
}
