
//#define DllAPI _declspec(dllexport)

#include "kdtreenode.h"

KDTreeNode::KDTreeNode():_left_child(0), _right_child(0), _parent(0), _split_dim(0){}

KDTreeNode::KDTreeNode(KDTreeNode *left_child, KDTreeNode *right_child,	KDTreeNode *parent, int split_dim, Point dom_elt, _HyperRectangle range_hr):
		_left_child(left_child), _right_child(right_child), _parent(parent), _split_dim(split_dim), _dom_elt(dom_elt), _range_hr(range_hr){}


Point& KDTreeNode::getDomElt() { return _dom_elt; }

_HyperRectangle& KDTreeNode::getHyperRectangle(){ return _range_hr; }

int& KDTreeNode::splitDim(){ return _split_dim; }

void  KDTreeNode::create( KDTreeNode *left_child, KDTreeNode *right_child,
	KDTreeNode *parent, int split_dim, Point dom_elt,  _HyperRectangle range_hr)
{
	this->_left_child = left_child;
	this->_right_child = right_child;
	this->_parent = parent;
	this->_split_dim = split_dim;
	this->_dom_elt = dom_elt;
	this->_range_hr = range_hr;
}

 KDTreeNode::KDTreeNode( const KDTreeNode &rhs )
{
	this->_left_child = rhs._left_child;
	this->_right_child = rhs._right_child;
	this->_parent = rhs._parent;
	this->_split_dim = rhs._split_dim;
	this->_dom_elt = rhs._dom_elt;
	this->_range_hr = rhs._range_hr;
}

 KDTreeNode&  KDTreeNode::operator=( const KDTreeNode &rhs )
{
	if(this == &rhs) 
		return *this;
	this->_left_child = rhs._left_child;
	this->_right_child = rhs._right_child;
	this->_parent = rhs._parent;
	this->_split_dim = rhs._split_dim;
	this->_dom_elt = rhs._dom_elt;
	this->_range_hr = rhs._range_hr;

	return *this;
}
