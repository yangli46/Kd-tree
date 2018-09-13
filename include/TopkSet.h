/*
 * TopkSet.h
 *
*  Created on: 2014-12-15
 *      Author: H. Wang & J. Song
 */

#ifndef TOPKSET_H_
#define TOPKSET_H_

#include "examplarset.h"

using namespace std;

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

#endif /* TOPKSET_H_ */
