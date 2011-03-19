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
			BlockArray(e_uint32 size, e_uint32 blockSize);

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

			e_uint32 getIndexOfAddress(const T* const address) const;

			e_uint32 getElementsPerBlock() const;
			e_uint32 getMemoryUsage() const;

			T& at(e_uint32 index);
			const T& at(e_uint32 index) const;

			T& operator[] (e_uint32 index) { return this->at(index); };
			const T& operator[] (e_uint32 index) const { return this->at(index); };

		private:
			T* initializeNewBlock() const;
			void clearBlock(T* block);
			e_uint32 getBlockOffset(e_uint32 index) const;

			SimpleDynamicArray<T*> mBlockPointers;
			e_uint32 mBlockSize, mBlockCount, mElements;
	};

	template <class T>
	BlockArray<T>::BlockArray(e_uint32 size, e_uint32 blockSize)
	: mBlockPointers(),mBlockSize(blockSize),mBlockCount(0),mElements(size)
	{
		mBlockCount = (size/this->getElementsPerBlock())+1;
		mBlockPointers.resize(mBlockCount);
		for(e_uint32 i = 0; i < mBlockPointers.getSize(); ++i)
			mBlockPointers[i] = this->initializeNewBlock();
	}

	template <class T>
	void BlockArray<T>::resize(e_uint32 size)
	{
		e_uint32 newBlockCount = (size/this->getElementsPerBlock())+1;
		// Clear the data in blocks if data would be truncated by the resize
		if(newBlockCount < mBlockCount)
			for(e_uint32 i = mBlockCount-1; i < (newBlockCount-1); ++i)
				this->clearBlock(mBlockPointers[i]);

		// If there are going to be more blocks, initialize them
		if(newBlockCount > mBlockCount)
		{
			// Only resize the number of pointers if block count is greater
			mBlockPointers.resize(newBlockCount);

			for(e_uint32 i = mBlockCount; i < mBlockPointers.getSize(); ++i)
				mBlockPointers[i] = this->initializeNewBlock();
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
		mBlockPointers[indexTo] = temp;
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
		this->clearBlock(temp);
		for(e_uint32 i = index+1; i < mBlockCount; ++i)
			mBlockPointers[i-1] = mBlockPointers[i];
		--mBlockCount;
		mBlockPointers[mBlockCount] = temp;
	}

	template <class T>
	e_uint32 BlockArray<T>::getIndexOfAddress(const T* const address) const
	{
		const e_uint32 elemsPerBlock = this->getElementsPerBlock();
		for(e_uint32 i = 0; i < mBlockCount; ++i)
		{
			e_uint32 offset = address - mBlockPointers.at(i);
			if(offset < elemsPerBlock)
				return offset + (i*elemsPerBlock);
		}
		return mElements + 1;
	}

	template <class T>
	e_uint32 BlockArray<T>::getElementsPerBlock() const
	{
		return mBlockSize/sizeof(T);
	}

	template <class T>
	e_uint32 BlockArray<T>::getMemoryUsage() const
	{
		return mBlockSize*mBlockPointers.getSize();
	}

	template <class T>
	T& BlockArray<T>::at(e_uint32 index)
	{
		return mBlockPointers.at(this->getBlockIndex(index))[this->getBlockOffset(index)];
	}

	template <class T>
	const T& BlockArray<T>::at(e_uint32 index) const
	{
		return mBlockPointers.at(this->getBlockIndex(index))[this->getBlockOffset(index)];
	}

	template <class T>
	T* BlockArray<T>::initializeNewBlock() const
	{
		return new T[this->getElementsPerBlock()];
	}

	template <class T>
	void BlockArray<T>::clearBlock(T* block)
	{
		for(e_uint32 i = 0; i < this->getElementsPerBlock(); ++i)
			block[i] = T();
	}

	template <class T>
	e_uint32 BlockArray<T>::getBlockOffset(e_uint32 index) const
	{
		return index - this->getBlockIndex(index)*this->getElementsPerBlock();
	}

}
#endif
