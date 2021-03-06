#pragma once
#include "../debug.h"

template<typename TItem>
class TArray
{
public:
	class Iterator;

private:
	const int kDefaultSize = 0x4;
	const int kMaxSize = 0xFFFF;

private:
	TItem* arr;
	int size;
	int max_size;
	// equal last item index + 1
	int count;
public:
	TArray();
	~TArray();

public:
	void Add(const TItem item);
	void Remove(const TItem item);
	void RemoveAt(const int index);
	// Not shrink if new_size less zero
	void Clear(int new_size);
	void Shrink();
	Iterator Begin();
	Iterator End();

private:
	void ResizeTo(const int new_size);

public:
	class Iterator
	{
	private:
		TItem *ptr;

	public:
		Iterator(TItem *item);
		Iterator(const Iterator &obj);

		TItem& operator*()
		{
			return *ptr;
		}

		Iterator& operator++()
		{
			ptr++;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator tmp = *this;
			ptr++;
			return tmp;
		}

		friend bool operator==(const Iterator& rhs, const Iterator& lhs)
		{
			return rhs.ptr == lhs.ptr;
		}

		friend bool operator!=(const Iterator& rhs, const Iterator& lhs)
		{
			return rhs.ptr != lhs.ptr;
		}
	};
};

template<typename TItem>
inline TArray<TItem>::TArray()
{
	size = kDefaultSize;
	max_size = kMaxSize;
	count = 0;

	arr = static_cast<TItem*>(std::malloc(sizeof(TItem) * size));
	FASSERT(arr, "malloc failed");
}

template<typename TItem>
inline TArray<TItem>::~TArray()
{
	std::free(arr);
}

template<typename TItem>
inline void TArray<TItem>::Add(const TItem item)
{
	if (count >= size)
	{
		// TODO Maybe can let the user decide
		// But i dont know how to implement. use function pointer?
		ResizeTo(size * 2);
	}

	arr[count++] = item;
}

template<typename TItem>
inline void TArray<TItem>::Remove(const TItem item)
{
	for (TItem *iter = arr; iter != (arr + count); iter++)
	{
		if (*iter == item)
		{
			memmove(iter, iter + 1, sizeof(TItem) * ((arr + count) - iter));

			// Must be at the end, because End() dependent count
			count--;
			break;
		}
	}
}

template<typename TItem>
inline void TArray<TItem>::RemoveAt(const int index)
{
	FASSERT(index > 0 && index < count && count > 0, "index out of range");

	TItem *removed = arr + index;
	memmove(removed, removed + 1, sizeof(TItem) * (End() - removed));

	// Must be at the end, because End() dependent count
	count--;
}

template<typename TItem>
inline void TArray<TItem>::Clear(const int new_size)
{
	memset(arr, 0, sizeof(TItem) * count);
	count = 0;
}

template<typename TItem>
inline void TArray<TItem>::Shrink()
{
	if (count > 0)
	{
		ResizeTo(count);
	}
}

template<typename TItem>
inline typename TArray<TItem>::Iterator TArray<TItem>::Begin()
{
	return Iterator(arr);
}

template<typename TItem>
inline typename TArray<TItem>::Iterator TArray<TItem>::End()
{
	return Iterator(arr + count);
}

template<typename TItem>
inline void TArray<TItem>::ResizeTo(int new_size)
{
	if (new_size > max_size)
	{
		new_size = max_size;
	}

	FASSERT(new_size >= count, "new_size less then count");
	
	arr = static_cast<TItem*>(realloc(arr, sizeof(TItem) * new_size));
	FASSERT(arr, "realloc failed");

	size = new_size;
	if (count > size)
	{
		// UNDONE Need warning log
		count = size;
	}
}

template<typename TItem>
inline TArray<TItem>::Iterator::Iterator(TItem *item)
	: ptr(item)
{
}

template<typename TItem>
inline TArray<TItem>::Iterator::Iterator(const Iterator &obj)
	: ptr(obj.ptr)
{
}