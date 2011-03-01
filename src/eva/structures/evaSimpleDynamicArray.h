#ifndef EVA_SIMPLEDYNAMICARRAY_H_
#define EVA_SIMPLEDYNAMICARRAY_H_

#include "eva/Typedefs.h"
#include "string.h"

namespace eva
{
	template <class T>
	/**
	 * \brief Provides simple functions to manage basic arrays
	 *
	 */
	class SimpleDynamicArray
	{
	public:
		/**
		 * Creates a new SimpleDynamicArray that is empty and holds no elements
		 */
		SimpleDynamicArray():mElements(0),mArray(0){};
		/**
		 * Creates a new SimpleDynamicArray that will store 'size' number of elements
		 */
		SimpleDynamicArray(e_uint32 size);
		/**
		 * Creates a new SimpleDynamicArray, does not make a copy of 'array' that holds 'size' number of elements,
		 * only assumes control of it
		 */
		SimpleDynamicArray(T* array, e_uint32 size):mElements(size),mArray(array){};

		/**
		 * Destructor, frees the memory of the array being controlled
		 */
		~SimpleDynamicArray();

		/**
		 * Returns the number elements to be stored in the array
		 */
		e_uint32 getSize() const { return mElements; };
		/**
		 * Returns a pointer to the array being controlled
		 */
		T* getArray() { return mArray; };
		/**
		 * Returns a const-pointer to the array being controlled
		 */
		const T* getArray() const { return mArray; };

		/**
		 * Resizes amount of elements that can be stored in the array
		 * If 'size' is less the the current size of the array, data is truncated
		 */
		T* resize(e_uint32 size);

		/**
		 * Halves the amount of elements that can be stored in the array
		 */
		T* halfSize();
		/**
		 * Doubles the amount of elements that can be stored in the array
		 */
		T* doubleSize();

		/**
		 * Releases control of the array, returning a pointer to the data once managed by the class
		 */
		T* release();

		/**
		 * Returns the size, in bytes, of the memory allocated by the array being controlled
		 */
		e_uint32 getMemoryUsage() const { return mElements*sizeof(T); };

		/**
		 * Returns a reference to the element at index 'index' in the array
		 */
		T& operator[] (unsigned int index) { return mArray[index]; };
		/**
		 * Returns a const-reference to the element at index 'index' in the array
		 */
		const T& operator[] (unsigned int index) const { return mArray[index]; };

	private:
		e_uint32 mElements;
		T* mArray;
	};

	template <class T>
	SimpleDynamicArray<T>::SimpleDynamicArray(e_uint32 size)
	: mElements(size)
	  {
		mArray = new T[mElements];
	  }

	template <class T>
	SimpleDynamicArray<T>::~SimpleDynamicArray()
	{
		delete mArray;
	}

	template <class T>
	T* SimpleDynamicArray<T>::resize(e_uint32 size)
	{
		if(size == 0)
		{
			delete mArray;
			mArray = 0;
			mElements = 0;
		}
		else
		{
			e_uint32 min = size;
			if(mElements < size)
				min = mElements;

			T* newArray = new T[size];
			memcpy(newArray,mArray,min*sizeof(T));
			mArray = newArray;
			mElements = size;
		}
		return mArray;
	}

	template <class T>
	T* SimpleDynamicArray<T>::halfSize()
	{
		return this->resize(mElements/2);
	}

	template <class T>
	T* SimpleDynamicArray<T>::doubleSize()
	{
		if(mElements > 0)
			return this->resize(mElements*2);
		return this->resize(1);
	}

	template <class T>
	T* SimpleDynamicArray<T>::release()
	{
		T* array = mArray;
		mArray = 0;
		mElements = 0;
		return array;
	}
}
#endif
