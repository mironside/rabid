/**
  @file vector.h
  @author Christopher Olsen
  @date Thu Mar 10 01:38:05 2005

  Copyright (C) 2004 Christopher A Olsen

  NOTES
  This vector template manages an internal array.  Items pushed onto the back
  of the vector may result in doubling the internal array for space.  The user
  never knows the capacity of the internal structure, only the number of items
  in the vector.
*/
#ifndef __SN_VECTOR__
#define __SN_VECTOR__

#include <memory.h>
extern void *operator new[](unsigned int s, const char *filename, int line);
extern void *operator new(unsigned int s, const char *filename, int line);
void operator delete(void* p, const char *filename, int line);
void operator delete[](void* p, const char *filename, int line);


template<typename T>
class snVector
{
protected:
  unsigned int size;
  unsigned int capacity;
  T* items;

public:
  snVector(unsigned int s = 0) :
    size(s), capacity(0), items(0)
  {
    Resize(size);
  }

  snVector(const snVector<T>& rhs) :
    size(0), capacity(0), items(0)
  {
    *this = rhs;
  }

  ~snVector()
  {
    if(items)
      delete[] items;
  }



  /**
     operator =
  */
  snVector<T>& operator = (const snVector<T>& rhs)
  {
    Resize(rhs.Size());
    for(unsigned int i = 0; i < rhs.Size(); i++)
      items[i] = rhs[i];

    return *this;
  }

  /**
     Resize
  */
  void Resize(unsigned int newSize)
  {
//    printf("resize %d\n", newSize);
    SetCapacity(newSize);
    size = newSize;
  }



  /**
     Capacity
  */
  void SetCapacity(unsigned int newCapacity)
  {
    T* newItems = 0;
    if(newCapacity > 0)
    {
      newItems = new(__FILE__, __LINE__) T[sizeof(T)*newCapacity];
      memset(newItems, 0, newCapacity * sizeof(T));

      if(items)
      {
        if(size < newCapacity)
          memcpy(newItems, items, size * sizeof(T));
        else
          memcpy(newItems, items, newCapacity * sizeof(T));
      }
    }
    capacity = newCapacity;
    if(items)
      delete[] items;
    items = newItems;
  }


  /**
     Size
  */
  unsigned int Size() const
  { return size; }


  /**
    operator []
  */
  T& operator[](unsigned int i)
  {
    if(i >= size)
      throw "Vector index out of bounds";

    return items[i];
  }


  /**
    operator []
  */
  const T& operator[](unsigned int i) const
  {
    if(i >= size)
      throw "Vector index out of bounds";

    return items[i];
  }


  /**
    PushBack
  */
  T& PushBack(T& item)
  {
    // double capacity
    if(++size >= capacity)
      SetCapacity(capacity*2+1);

    return items[size-1] = item;
  }

  /**
    PushBack
  */
  T& PushBack(const T& item)
  {
    // double capacity
    if(++size >= capacity)
      SetCapacity(capacity*2+1);

    return items[size-1] = item;
  }

  /**
     PopBack
  */
  void PopBack()
  {
    size--;
    if(size < 0)
      size = 0;
  }


  int Remove(unsigned int i)
  {
    if(i < size)
    {
      memcpy(&items[i], &items[i+1], (size-1 - i) * sizeof(T));
      size--;
      return i - 1;
    }

    return i;
  }
};

#endif
