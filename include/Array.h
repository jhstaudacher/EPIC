#ifndef EPIC_ARRAY_H_
#define EPIC_ARRAY_H_

#include "types.h"

#include <cstdlib>

namespace epic {

/**
 * A one dimensional Array allowing an index range with offset.
 *
 * If one wants to store n elements but address them with an offset for more intuitive use, this class manages the offset calculations internally.
 */
template<class T> class ArrayOffset {
public:
	/**
	 * Creating an empty object without allocating any memory
	 *
	 * @note When using this constructor the alloc() method must be called before using the index operator()!
	 */
	ArrayOffset() {
		mPtr = nullptr;
		mDim = 0;
		mOffset = 0;
		mSelfAllocated = false;
	}

	/**
	 * Creating an object including memory allocations
	 *
	 * @param dim The address size
	 * @param offset The offset. The array will contain dim - offset elements. Addressed from offset - dim - 1
	 * @param value_initialize Defines whether the allocated values should be value initialized or not
	 */
	ArrayOffset(longUInt dim, longUInt offset, bool value_initialize = false) {
		alloc(dim, offset, value_initialize);
	}

	ArrayOffset(longUInt dim, longUInt offset, T* memory) {
		mPtr = memory;
		mSelfAllocated = false;

		mDim = dim;
		mOffset = offset;
	}

	/**
	 * Delete the object
	 *
	 * If memory is still allocated it gets freed
	 */
	~ArrayOffset() {
		free();
	}

	/**
	 * A function allowing intuitive access to the array elements
	 *
	 * @param i The element index
	 * @return Object reference to the array element at index i
	 */
	inline T& operator[](longUInt i) {
		return mPtr[i - mOffset];
	}

	/**
	 * allocating memory regarding the input parameters
	 *
	 * @param dim The address size
	 * @param offset The offset. The array will contain dim - offset elements. Addressed from offset - dim - 1
	 * @param value_initialize Defines whether the allocated values should be value initialized or not
	 */
	inline void alloc(longUInt dim, longUInt offset, bool value_initialize = false) {
		longUInt size = dim - offset;
		mPtr = value_initialize ? new T[size]() : new T[size];
		mSelfAllocated = true;

		mDim = dim;
		mOffset = offset;
	}

	/**
	 * frees allocated memory if any exists
	 */
	inline void free() {
		if (mSelfAllocated && mPtr != nullptr) {
			delete[] mPtr;
			mPtr = nullptr;
			mSelfAllocated = false;
		}
	}

	/**
	 * Getting the Pointer to the C-style array that gets managed internally.
	 *
	 * @note The array should only get allocated and deleted by the class's member methods!
	 *
	 * @return The C-style array
	 */
	inline T* getArrayPointer() const {
		return mPtr;
	}

	/**
	 * Getting the total number of elements contained in the array.
	 *
	 * @return The total number of array elements
	 */
	inline longUInt getNumberOfElements() const {
		return mDim - mOffset;
	}

private:
	T* mPtr;
	longUInt mDim, mOffset;
	bool mSelfAllocated;
}; /* class ArrayOffset */

/**
 * A two dimensional Array internally storing the data as a one dimensional array
 *
 * Doing so improves the caching performance since all data of the 2D array gets stored side by side in the memory.
 */
template<class T> class Array2d {
public:
	/**
	 * Creating an empty object without allocating any memory
	 *
	 * @note When using this constructor the alloc() method must be called before using the index operator()!
	 */
	Array2d() {
		mPtr = nullptr;
		mDimX = 0;
		mDimY = 0;
	}

	/**
	 * Creating an object including memory allocations
	 *
	 * @param dim_x defines the size of the first dimension
	 * @param dim_y defines the size of the second dimension
	 * @param value_initialize defines whether the allocated values should be value initialized or not
	 */
	Array2d(longUInt dim_x, longUInt dim_y, bool value_initialize = false) {
		alloc(dim_x, dim_y, value_initialize);
	}

	/**
	 * Delete the object
	 *
	 * If memory is still allocated it gets freed
	 */
	~Array2d() {
		free();
	}

	/**
	 * A Function allowing intuitive access to the array elements
	 * 
	 * @param x the index of the first dimension
	 * @param y the index of the second dimension
	 * @return Object reference to the array element at index x, y
	 *
	 * @note before using this operator the first time either the memory allocating constructor or the alloc() method must be called!
	 */
	inline T& operator()(longUInt x, longUInt y) {
		return mPtr[(x * mDimY) + y];
	}

	/**
	 * Get the x-th dimension as normal C-style array
	 *
	 * @param x the dimension
	 * @return C-style Array representing the x-th dimension
	 *
	 * @note The returned array must not be deleted, as it was internally allocated. Doing it anyway results in undefined behaviour (besides breaking this object).
	 */
	inline T* getArray(longUInt x) {
		return mPtr + (x * mDimY);
	}

	/**
	 * allocating memory regarding the input parameters
	 *
	 * @param dim_x defines the size of the first dimension
	 * @param dim_y defines the size of the second dimension
	 * @param value_initialize defines whether the allocated values should be value initialized or not
	 */
	inline void alloc(longUInt dim_x, longUInt dim_y, bool value_initialize = false) {
		longUInt size = dim_x * dim_y;
		mPtr = value_initialize ? new T[size]() : new T[size];
		mDimX = dim_x;
		mDimY = dim_y;
	}

	/**
	 * frees allocated memory if any exists
	 */
	inline void free() {
		if (mPtr != nullptr) {
			delete[] mPtr;
			mPtr = nullptr;
		}
	}

	/**
	 * Getting the Pointer to the C-style array that gets managed internally.
	 *
	 * @note The array should only get allocated and deleted by the class's member methods!
	 *
	 * @return The C-style array
	 */
	inline T* getArrayPointer() const {
		return mPtr;
	}

	/**
	 * Getting the total number of elements contained in the array.
	 *
	 * @return The total number of array elements
	 */
	inline longUInt getNumberOfElements() const {
		return mDimX * mDimY;
	}

private:
	T* mPtr;
	longUInt mDimX, mDimY;

}; /* class Array2d */

/**
 * A two dimensional Array internally storing the data as a one dimensional array allowing the usage of index offsets
 * 
 * If one wants to store n elements but address them with an offset for more intuitive use, this class manages the offset calculations internally.
 */
template<class T> class Array2dOffset {
public:
	/**
	 * Creating an empty object without allocating any memory
	 *
	 * @note When using this constructor the alloc() method must be called before using the index operator()!
	 */
	Array2dOffset() {
		mPtr = nullptr;
		mDimX = 0;
		mDimY = 0;
		mOffX = 0;
		mOffY = 0;
	}

	/**
	 * Creating an object including memory allocations
	 *
	 * @param dim_x defines the address size of the first dimension
	 * @param dim_y defines the address size of the second dimension
	 * @param offset_x defines the offset of the first dimension. The Array will contain dim_x - offset_x elements (indices from offset_x to dim_x - 1) in the first dimension
	 * @param offset_y defines the offset of the second dimension. The Array will contain dim_y - offset_y elements (indices from offset_y to dim_y - 1) in the second dimension
	 * @param value_initialize defines whether the allocated values should be value initialized or not
	 */
	Array2dOffset(longUInt dim_x, longUInt dim_y, longUInt offset_x, longUInt offset_y, bool value_initialize = false) {
		alloc(dim_x, dim_y, offset_x, offset_y, value_initialize);
	}

	/**
	 * Delete the object
	 *
	 * If memory is still allocated it gets freed
	 */
	~Array2dOffset() {
		free();
	}

	/**
	 * A Function allowing intuitive access to the array elements
	 * 
	 * @param x the index of the first dimension
	 * @param y the index of the second dimension
	 * @return Object reference to the array element at index x, y
	 */
	inline T& operator()(longUInt x, longUInt y) {
		return mPtr[((x - mOffX) * mDimY) + (y - mOffY)];
	}

	/**
	 * Get the x-th dimension as ArrayOffset-object
	 *
	 * @param x the dimension
	 * @return ArrayOffset-object representing the x-th dimension
	 */
	inline ArrayOffset<T> getArrayOffset(longUInt x) {
		return ArrayOffset<T>(mDimY, mOffY, mPtr + ((x - mOffX) * mDimY));
	}

	/**
	 * allocating memory regarding the input parameters
	 *
	 * @param dim_x defines the address size of the first dimension
	 * @param dim_y defines the address size of the second dimension
	 * @param offset_x defines the offset of the first dimension. The Array will contain dim_x - offset_x elements (indices from offset_x to dim_x - 1) in the first dimension
	 * @param offset_y defines the offset of the second dimension. The Array will contain dim_y - offset_y elements (indices from offset_y to dim_y - 1) in the second dimension
	 * @param value_initialize defines whether the allocated values should be value initialized or not
	 */
	inline void alloc(longUInt dim_x, longUInt dim_y, longUInt offset_x, longUInt offset_y, bool value_initialize = false) {
		longUInt size = (dim_x - offset_x) * (dim_y - offset_y);
		mPtr = value_initialize ? new T[size]() : new T[size];

		mDimX = dim_x;
		mDimY = dim_y;
		mOffX = offset_x;
		mOffY = offset_y;
	}

	/**
	 * frees allocated memory if any exists
	 */
	inline void free() {
		if (mPtr != nullptr) {
			delete[] mPtr;
			mPtr = nullptr;
		}
	}

	/**
	 * Getting the Pointer to the C-style array that gets managed internally.
	 *
	 * @note The array should only get allocated and deleted by the class's member methods!
	 *
	 * @return The C-style array
	 */
	inline T* getArrayPointer() const {
		return mPtr;
	}

	/**
	 * Getting the total number of elements contained in the array.
	 *
	 * @return The total number of array elements
	 */
	inline longUInt getNumberOfElements() const {
		return (mDimX - mOffX) * (mDimY - mOffY);
	}

private:
	T* mPtr;
	longUInt mDimX, mDimY;
	longUInt mOffX, mOffY;

}; /* class Array2dOffset */

} /* namespace epic */

#endif /* EPIC_ARRAY_H_ */
