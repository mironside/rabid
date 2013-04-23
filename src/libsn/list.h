#ifndef _LIST_H_
#define _LIST_H_

#include <memory.h>
void *operator new(size_t s, const char *filename, unsigned long line);

template<typename T>
struct snListNode
{
  T data;
  snListNode *next;
  snListNode *prev;
};


template<typename T>
class snListIterator
{
public:
  typedef snListIterator<T> ListIterator;
  snListNode<T> *node;
  
  void Increment() { node = node->next; }
  void Decrement() { node = node->prev; }

  bool operator== (const ListIterator& iter) const
    { return node == iter.node; }
  bool operator!= (const ListIterator& iter) const
    { return node != iter.node; }

  T& operator* () const {return node->data;}

  ListIterator& operator++ ()
    {this->Increment(); return *this;}

  ListIterator  operator++ (int) 
  {
    ListIterator tmp = *this;
    this->Increment();
    return tmp;
  }

  ListIterator& operator-- ()
    { this->Decrement(); return *this; }

  ListIterator  operator-- (int) 
  {
    ListIterator tmp = *this;
    this->Decrement();
    return tmp;
  }
};


template<class T>
class snList
{
protected:
  typedef snListNode<T> ListNode;
  snListNode<T>* begin;
  snListNode<T>* end;
  unsigned int size;

public:
  typedef snListIterator<T> Iterator;

  snList() : begin(0), end(0), size(0) {}

  ~snList()
  {
    Clear();
  }

  Iterator Begin()
  {
    Iterator iter;
    iter.node = begin;
    return iter;
  }

  Iterator End()
  {
    Iterator iter;
    iter.node = 0;
    return iter;
  }


  Iterator Back()
  {
    Iterator iter;
    iter.node = end;
    return iter;
  }

  T Front()
  {
    Iterator iter;
    iter.node = begin;
    return *iter;
  }

  void PopFront()
  {
    Iterator iter = Begin();
    Remove(iter);
  }


  unsigned int Size() { return size; }

  void PushBack(const T& listItem)
  {
    Iterator iter;
    iter.node = end;
    Insert(iter, listItem);
  }

  void PushFront(const T& listItem)
  {
    Insert(Begin(), listItem);
  }

  void Insert(Iterator iter, T data)
  {
    // Create new node
    ListNode *newNode = new ListNode;
    newNode->data = data;

    if(!begin && !end)
    {
      newNode->prev = 0;
      newNode->next = 0;
      begin = end = newNode;
    }
    else if(iter.node == end)
    {
      // Set end's next node to new node 
      end->next = newNode;
      // Set new node's prev node to end node
      newNode->prev = end;
      // Set new iterator's next node to NULL
      newNode->next = 0;
      // make new iterator the end
      end = newNode;
    }
    else
    {
      // Set the previous node of iterator's next node to the new iterator
      iter.node->next->prev = newNode;
      // Set new iterator's next node to iterator's next node
      newNode->next = iter.node->next;
      // Set iterator's next node to new iterator's node
      iter.node->next = newNode;
      // Set new iterator's prev node to iterator's node 
      newNode->prev = iter.node;
    }

    size++;
  }

  Iterator Remove(Iterator& iter)
  {
    if(begin && (begin == iter.node))
    {
      if(begin == end)
      { 
        // Delete node
        delete iter.node;
        iter.node = 0;
        // Reset list
        begin = end = 0;
        size = 0;

        // Return a null iterator
        Iterator nullIter;
        nullIter.node = 0;
        return nullIter;
      }
      else
      {
        // Create new iterator pointing at next node
        Iterator nextIter;
        nextIter.node = begin->next;

        // Set list beginning to next node
        ListNode* oldBegin = begin;
        begin = begin->next;
        // Unlink old node
        delete oldBegin;
        size--;
        oldBegin = 0;

        return nextIter;
      }
    }
    else if(end && (end == iter.node))
    {
      if(end == begin)
      {
        delete iter.node;
        iter.node = 0;
        end = begin = 0;
        size = 0;

        Iterator nullIter;
        nullIter.node = 0;
        return nullIter;
      }
      else
      {
        ListNode* newEnd = iter.node->prev;
        delete iter.node;
        iter.node = 0;
        size--;
        newEnd->next = 0;
        end = newEnd;

        Iterator nullIter;
        nullIter.node = 0;
        return nullIter;
      }
    }
    else
    {
      Iterator nextIter;
      nextIter.node = iter.node->next;

      ListNode *nodePrev = 0;
      ListNode *nodeNext = 0;

      nodePrev = iter.node->prev;
      nodePrev->next = iter.node->next;

      nodeNext = iter.node->next;
      nodeNext->prev = iter.node->prev;

      delete iter.node;
      iter.node = 0;
      size--;

      return nextIter;
    }

    Iterator nullIter;
    nullIter.node = 0;
    return nullIter;
  }

  void Clear()
  {
    while(begin)
    {
      ListNode* tmp = begin;
      begin = begin->next;
      delete tmp;
    }

    begin = end = 0;
    size = 0;
  }
};

#endif /*_LINKED_LIST_H_*/

