/**
  @file handle.h
  Template for generic handle manager.  Paramaterized by type of data stored
  internally and referenced by the handle.
*/
#ifndef __HANDLE_MGR__
#define __HANDLE_MGR__

#include "common/log.h"
#include "libsn/vector.h"

template<typename T>
class HandleMgr
{
public:
  snVector<T*> entries;
  snVector<unsigned int> freeEntries;



public:
  HandleMgr() :
    entries(0), freeEntries(0)
  {
    LOG(LOG_BLUE, "Create HandleMgr %p", this);
  }

  ~HandleMgr()
  {
    for(unsigned int i = 0; i < entries.Size(); i++)
      delete entries[i];

    freeEntries.Resize(0);
    entries.Resize(0);
  }



  int Create(T** entry)
  {
    int index = 0;

    // no free entries, create one
    if(!freeEntries.Size())
    {
      // create new entry
      T* newEntry = new T;
      memset(newEntry, 0, sizeof(T));
      index = (int)entries.Size();

      // add entry to file list
      entries.PushBack(newEntry);
      *entry = newEntry;
      return index;
    }
    else
    {
      index = freeEntries[freeEntries.Size()-1];
      freeEntries.PopBack();
      memset(entries[index], 0, sizeof(T));
      *entry = entries[index];
      return index;
    }
  }



  void Destroy(int handle)
  {
    if(handle >= (int)entries.Size())
      return;

    memset(entries[handle], 0, sizeof(T));
    freeEntries.PushBack(handle);
  }



  T* Dereference(int handle)
  {
    if(handle < 0 ||
       handle >= (int)entries.Size())
    {
      return 0;
    }

    return entries[handle];
  }


  snVector<T*>& GetActiveList()
  {
    return entries;
  }

  int GetNumActive()
  {
    return entries.Size() - freeEntries.Size();
  }
};

#endif

