#ifndef EVA_BLOCKARRAY_H_
#define EVA_BLOCKARRAY_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaSimpleDynamicArray.h"

namespace eva
{
	template <class T>
	class BlockArray
	{
		public:
			BlockArray(e_uint32 blockSize = 256):mBlockPointers(),mBlockSize(blockSize),mBlockCount(0),mElements(0){};
			BlockArray(e_uint32 size, e_uint32 blockSize = 256);

			e_uint32 getBlockSize() const { return mBlockSize; };
			e_uint32 getSize() const { return mElements; };
			e_uint32 getBlockCount() const { return mBlockCount; };

			void resize(e_uint32 size);

			/**
			 *   Gets the block number which the current index resides in
			 */
			e_uint32 getBlockIndex(e_uint32 index) const;
			
			void swapBlocks(e_uint32 indexFrom, e_uint32 indexTo);
			void moveBlock(e_uint32 indexFrom, e_uint32 indexTo);
			void deleteBlock(e_uint32 index);

			e_uint32 getElementsPerBlock() const;
			e_uint32 getBlockCount() const;
			e_uint32 getMemoryUsage() const;

			T& at(e_uint32 index);
			const T& at(e_uint32 index) const;

			T& operator[] (e_uint32 index) { return this->at(index); };
			const T& operator[] (e_uint32 index) const { return this->at(index); };

		private:
			T* initializeNewBlock() const;

			SimpleDynamicArray<T*> mBlockPointers;
			e_uint32 mBlockSize, mBlockCount, mElements;
	};

	template <class T>
	BlockArray<T>::BlockArray(e_uint32 size, e_uint32 blockSize)
	: mBlockPointers(),mBlockSize(blockSize),mBlockCount(0),mElements(size)
	{
		e_float32 blockCountF = (e_float32)size/(e_float32)this->getElementsPerBlock();
		mBlockCount = (e_uint32)blockCountF;
		if(blockCountF > mBlockCount)
			++mBlockCount;
		mBlockPointers->resize(mBlockCount);
		for(e_uint32 i = 0; i < mBlockPointers.getSize(); ++i)
			mBlockPointers[i] = this->initalizeNewBlock();
	}

	template <class T>
	void BlockArray<T>::resize(e_uint32 size)
	{
		e_float32 blockCountF = (e_float32)size/(e_float32)this->getElementsPerBlock();
		e_uint32 newBlockCount = (e_uint32)blockCountF;
		if(blockCountF > newBlockCount)
				++newBlockCount;

		// Clear the blocks if data would be truncated
		if(newBlockCount < mBlockCount)
			for(e_uint32 i = mBlockCount-1; i < (newBlockCount-1); ++i)
			{
				//TODO: Wrong, need to merely reset the data
				//delete mBlockPointers[i];
				//mBlockPointers[i] = 0;
			}

		// If there are going to be more blocks, initialize them
		if(newBlockCount > mBlockCount)
		{
			// Only resize the number of pointers if block count is greater
			mBlockPointers->resize(newBlockCount);

			for(e_uint32 i = mBlockCount; i < newBlockCount; ++i)
				mBlockPointers[i] = this->initalizeNewBlock();
		}

		mBlockCount = newBlockCount;
		mElements = size;
	}

	template <class T>
	e_uint32 BlockArray<T>::getBlockIndex(e_uint32 index) const
	{
		return index/this->getElementsPerBlock();
	}

	template <class T>
	void BlockArray<T>::swapBlocks(e_uint32 indexFrom, e_uint32 indexTo)
	{
		T* temp = mBlockPointers[indexFrom];
		mBlockPointers[indexFrom] = mBlockPointers[indexTo];
		mBlockPointers[indexTo] = mBlockPointers[temp];
	}

	template <class T>
	void BlockArray<T>::moveBlock(e_uint32 indexFrom, e_uint32 indexTo)
	{
		T* temp;
		if(indexFrom > indexTo)
		{
			temp = mBlockPointers[indexFrom];
			for(e_uint32 i = indexTo; i <= indexFrom; ++i)
			{
				T* tempTwo = mBlockPointers[i];
				mBlockPointers[i] = temp;
				temp = tempTwo;
			}
		}
		else
		{
			temp = mBlockPointers[indexFrom];
			for(e_uint32 i = indexFrom+1; i <= indexTo; ++i)
				mBlockPointers[i-1] = mBlockPointers[i];
			mBlockPointers[indexTo] = temp;
		}
	}

	template <class T>
	void BlockArray<T>::deleteBlock(e_uint32 index)
	{
		T* temp = mBlockPointers[index];
		for(e_uint32 i = index+1; i < mBlockCount; ++i)
		{
			T* tempTwo = mBlockPointers[i];
			mBlockPointers[i] = temp;
			temp = tempTwo;
		}
		--mBlockCount;

	}

	template <class T>
	e_uint32 BlockArray<T>::getElementsPerBlock() const
	{
		return mBlockSize/sizeof(T);
	}

	template <class T>
	T* BlockArray<T>::initializeNewBlock() const
	{
		return new T[this->getElementsPerBlock()];
	}

}
#endif
