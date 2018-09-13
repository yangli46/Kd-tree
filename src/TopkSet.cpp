/*
 * TopkSet.cpp
 *
 *  Created on: 2012-11-25
 *      Author: H. Wang && J. Song
 */

//#define DllAPI _declspec(dllexport)

#include "TopkSet.h"

Pair::Pair(): first(0), second(0) {}

Pair::Pair(int x, float y): first(x), second(y) {}

Pair& Pair::operator= (const Pair &p) {
	this->first = p.first;
	this->second = p.second;
	return *this;
}

ostream& operator<< (ostream &out, const Pair &p) {
	out << "(" << p.first << ", " << p.second << ") ";
	return out;
}

TopkSet::TopkSet(): m_capacity(0), m_size(0), data(NULL) {}

TopkSet& TopkSet::operator=(const TopkSet& rhs)
{
	 if(this == &rhs) 
		return *this;

	destroy();

	if(rhs.m_size > 0)
		m_size = rhs.m_size;
	else
		m_size = 0;

	if(rhs.m_capacity > 0)
		m_capacity = rhs.m_capacity;
	else
		m_capacity = 0;

	if(m_size > 0)
		data = new Pair[m_size];
	for(unsigned int i=0;i<m_size;i++)
	{
		data[i] = rhs.data[i];
	}

	return *this;
}

int TopkSet::construct(unsigned int k) {
	if (k == m_capacity) {
		m_size = 0;
		return 0;
	}

	if (data != NULL) delete [] data;
	if ((data = new Pair[k]) == NULL) {
		return 1;
	}
	m_capacity = k;
	m_size = 0;
	return 0;
}

TopkSet::~TopkSet() {
	if (data != NULL) delete [] data;
	data = NULL;
}

void TopkSet::clear() { m_size = 0; }

void TopkSet::destroy() {
	delete [] data;
	data = NULL;
	m_capacity = 0;
	m_size = 0;
}

int TopkSet::capacity() const { return m_capacity; }

int TopkSet::size() const { return m_size; }

const Pair* TopkSet::getData() const { return data; }

bool TopkSet::is_full() const { return m_size == m_capacity; }

float TopkSet::maxval() const { return data[m_size - 1].second; }

void TopkSet::print(ostream &out) const {
	for (size_t i = 0; i != m_size; ++i) {
		out << data[i] << " ";
	}
	out << " | ";
	for (size_t i = m_size; i != m_capacity; ++i) {
		out << data[i] << " ";
	}
}

int TopkSet::insert(int id, float val) {
	int i = 0;

	if (m_size < m_capacity) {
		i = m_size - 1;
		while (i >= 0) {
			if (val >= data[i].second) break;
			data[i + 1] = data[i];
			--i;
		}
		data[i + 1].first = id;
		data[i + 1].second = val;
		m_size += 1;
		return 1;
	}

	if (val >= data[m_size - 1].second) return 0;

	i = m_size - 2;
	while (i >= 0) {
		if (val >= data[i].second) break;
		data[i + 1] = data[i];
		--i;
	}
	data[i + 1].first = id;
	data[i + 1].second = val;

	return 1;
}
