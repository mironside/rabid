/**
  @file hashMap.h
  @author Christopher Olsen
  @date Tue Mar  8 01:53:12 2005

  Copyright (C) 2004 Christopher A Olsen


  NOTES ON USE

  This HashMap template supports either const char* (string literals)
  or integer values as keys.  The correct hashing and comparison
  functions are chosen based on what key type is given to the
  template.  Allocated strings are not currently handled completely.
  However, based on the use for this HashMap, they may not be
  necessary.

  When given a size for construction, the hash table finds the nearest
  prime that is big enough to hold the size requested.  The hash grows
  automatically when the table is filled to some percentage (50%), the
  size is expanded to the next prime that is closest to double the
  current size.

  The [] operator is used for both accessing and creating an item in
  the hash map.  One should be careful not to access non-existant
  items before they have been assigned in the hash.  Reading the value
  of an item created with operator [] before it has been assigned
  returns bad data.

  @todo
  need to free strings if they are non-constant
*/
#ifndef __SN_HASH_MAP__
#define __SN_HASH_MAP__

#include "vector.h"
void *operator new(size_t s, const char *filename, unsigned long line);



unsigned int defhash(int i);
unsigned int defhash(const char* str);
unsigned int defhash(char* str);

int defcmp(const char* str1, const char* str2);
int defcmp(char* str1, char* str2);
int defcmp(int i1, int i2);


static unsigned int primeTable[] = {7, 19, 37, 53, 97, 193, 389, 769,
                                    1543, 3079, 6151, 12289, 24593, 49157,
                                    98317, 196613, 393241, 786433, 1572869,
                                    3145739, 6291469, 12592917, 25165843};
#define MAX_PRIME 22







/**
   SHashMap
*/
template<typename T>
class snSHashMap
{
protected:
  typedef struct hashItem_s
  {
    char* key;
    T item;
    unsigned int hashValue;
    struct hashItem_s* llnext;
  } hashItem_t;

  unsigned int  hashSize;
  hashItem_t**  table;
  unsigned int  nItems;
  unsigned int  curPrime;
  snVector<const char*>   keys;





  /**
     Insert
  */
  void Insert(hashItem_t* h)
  {
    unsigned int idx = h->hashValue % hashSize;
    h->llnext = table[idx];
    table[idx] = h;
    nItems++;
    keys.PushBack(h->key);

    // resize on 50% fill
    if((nItems << 1) > hashSize)
    {
      curPrime++;
      if(curPrime > MAX_PRIME)
        curPrime = MAX_PRIME;
      else
        Resize(primeTable[curPrime]);
    }
  }


  /**
     Resize
  */
  void Resize(const unsigned int size)
  {
    hashItem_t**  newTable = new(__FILE__, __LINE__) hashItem_t*[size];
    memset(newTable, 0, sizeof(hashItem_t*) * size);
    nItems = 0;

    for(unsigned int i = 0; i < hashSize; i++)
    {
      for(hashItem_t* h = table[i]; h; )
      {
        // rehash item
        hashItem_t* temp = h->llnext;
        unsigned int idx = h->hashValue % size;
        h->llnext = newTable[idx];
        newTable[idx] = h;
        nItems++;
        h = temp;
      }
    }

    if(table)
      delete[] table;
    table = newTable;
    hashSize = size;
  }

public:
  snSHashMap(unsigned int size = 0) :
    hashSize(0), table(0), nItems(0), curPrime(0)
  {
    // find prime that accomidates size
    for(int i = 0; i < MAX_PRIME; i++)
    {
      if(size > primeTable[i] && size <= primeTable[i+1])
      {
        curPrime = i+1;
        break;
      }
    }

    Resize(primeTable[curPrime]);
  }


  snSHashMap(const snSHashMap<T>& rhs) :
    hashSize(0), table(0), nItems(0), curPrime(0)
  {
    *this = rhs;
  }



  ~snSHashMap()
  {
    Destroy();
  }


  snSHashMap<T>& operator = (const snSHashMap<T>& rhs)
  {
    Destroy();
    table = new hashItem_t*[rhs.hashSize];
    snVector<const char*> keys = rhs.GetKeys();

    for(unsigned int i = 0; i < keys.Size(); i++)
      (*this)[keys[i]] = rhs[keys[i]];

    return *this;
  }

  /**
     Destroy
  */
  void Destroy()
  {
    for(unsigned int i = 0; i < hashSize; i++)
    {
      for(hashItem_t* h = table[i]; h; )
      {
        hashItem_t* temp = h->llnext;
        delete h;
        h = temp;
      }
    }

    delete[] table;
    table = 0;
    hashSize = 0;

    for(unsigned int i = 0; i < keys.Size(); i++)
    {
      delete keys[i];
    }
    keys.Resize(0);
  }



  T& operator[](const char* key) const 
  {
    for(hashItem_t* h = table[defhash(key) % hashSize]; h; h = h->llnext)
    {
      if(strcmp(h->key, key) == 0)
        return h->item;
    }
    throw "Item not found in hashMap";
  }


  /**
    operator []
  */
  T& operator[](const char* key)
  {
    if(hashSize == 0)
      Resize(primeTable[0]);

    unsigned int idx = defhash(key);
    hashItem_t* h = 0;
    for(h = table[idx % hashSize]; h; h = h->llnext)
    {
      if(defcmp(h->key, key) == 0)
        return h->item;
    }

    h = new(__FILE__, __LINE__) hashItem_t;
    h->hashValue = idx;

    // strdup
    unsigned int len = 0;
    while(key[len]) { len++; }
    h->key = new char[len+1];
    memcpy(h->key, key, len);

    Insert(h);

    return h->item;
  }



  /**
    Remove
  */
  bool Remove(const char* key)
  {
    unsigned int idx = defhash(key) % hashSize;

    hashItem_t* prev = 0;
    for(hashItem_t* h = table[idx]; h; h = h->llnext)
    {
      if(defcmp(h->key, key) == 0)
      {
        if(!prev)
          table[idx] = h->llnext;
        else
          prev->llnext = h->llnext;

        delete h;

        for(unsigned int i = 0; i < keys.Size(); i++)
        {
          if(defcmp(key, keys[i]) == 0)
            keys.Remove(i);
        }
        return true;
      }
      prev = h;
    }

    return false;
  }



  /**
    Exists
  */
  bool Exists(const char* key) const
  {
    if(!table)
      return false;

    unsigned int idx = defhash(key) % hashSize;
    for(hashItem_t* h = table[idx]; h; h = h->llnext)
    {
      if(defcmp(h->key, key) == 0)
        return true;
    }

    return false;
  }


  /**
     GetKeys
  */
  const snVector<const char*>& GetKeys() const
  { return keys; }
};



/**
   IHashMap
*/
template<typename T>
class snIHashMap
{
protected:
  typedef struct hashItem_s
  {
    int key;
    T item;
    unsigned int hashValue;
    struct hashItem_s* llnext;
  } hashItem_t;

  unsigned int  hashSize;
  hashItem_t**  table;
  unsigned int  nItems;
  unsigned int  curPrime;
  snVector<int>   keys;





  /**
     Insert
  */
  void Insert(hashItem_t* h)
  {
    unsigned int idx = h->hashValue % hashSize;
    h->llnext = table[idx];
    table[idx] = h;
    nItems++;
    keys.PushBack(h->key);

    // resize on 50% fill
    if((nItems << 1) > hashSize)
    {
      curPrime++;
      if(curPrime > MAX_PRIME)
        curPrime = MAX_PRIME;
      else
        Resize(primeTable[curPrime]);
    }
  }


  /**
     Resize
  */
  void Resize(const unsigned int size)
  {
    hashItem_t**  newTable = new(__FILE__, __LINE__) hashItem_t*[size];
    memset(newTable, 0, sizeof(hashItem_t*) * size);
    nItems = 0;

    for(unsigned int i = 0; i < hashSize; i++)
    {
      for(hashItem_t* h = table[i]; h; )
      {
        // rehash item
        hashItem_t* temp = h->llnext;
        unsigned int idx = h->hashValue % size;
        h->llnext = newTable[idx];
        newTable[idx] = h;
        nItems++;
        h = temp;
      }
    }

    if(table)
      delete[] table;
    table = newTable;
    hashSize = size;
  }

public:
  snIHashMap(unsigned int size = 0) :
    hashSize(0), table(0), nItems(0), curPrime(0)
  {
    // find prime that accomidates size
    for(int i = 0; i < MAX_PRIME; i++)
    {
      if(size > primeTable[i] && size <= primeTable[i+1])
      {
        curPrime = i+1;
        break;
      }
    }

    Resize(primeTable[curPrime]);
  }


  snIHashMap(const snIHashMap<T>& rhs) :
    hashSize(0), table(0), nItems(0), curPrime(0)
  {
    *this = rhs;
  }



  ~snIHashMap()
  {
    Destroy();
  }


  snIHashMap<T>& operator = (const snSHashMap<T>& rhs)
  {
    Destroy();
    table = new hashItem_t*[rhs.hashSize];
    snVector<int> keys = rhs.GetKeys();

    for(unsigned int i = 0; i < keys.Size(); i++)
      (*this)[keys[i]] = rhs[keys[i]];

    return *this;
  }

  /**
     Destroy
  */
  void Destroy()
  {
    for(unsigned int i = 0; i < hashSize; i++)
    {
      for(hashItem_t* h = table[i]; h; )
      {
        hashItem_t* temp = h->llnext;
        delete h;
        h = temp;
      }
    }

    delete[] table;
    table = 0;
    hashSize = 0;

    keys.Resize(0);
  }



  T& operator[](int key) const 
  {
    for(hashItem_t* h = table[defhash(key) % hashSize]; h; h = h->llnext)
    {
      if(h->key == 0)
        return h->item;
    }
    throw "Item not found in hashMap";
  }


  /**
    operator []
  */
  T& operator[](int key)
  {
    if(hashSize == 0)
      Resize(primeTable[0]);

    unsigned int idx = defhash(key);
    hashItem_t* h = 0;
    for(h = table[idx % hashSize]; h; h = h->llnext)
    {
      if(h->key == key)
        return h->item;
    }

    h = new(__FILE__, __LINE__) hashItem_t;
    h->hashValue = idx;
    h->key = key;
    Insert(h);

    return h->item;
  }



  /**
    Remove
  */
  bool Remove(int key)
  {
    unsigned int idx = defhash(key) % hashSize;

    hashItem_t* prev = 0;
    for(hashItem_t* h = table[idx]; h; h = h->llnext)
    {
      if(h->key == key)
      {
        if(!prev)
          table[idx] = h->llnext;
        else
          prev->llnext = h->llnext;

        delete h;

        for(unsigned int i = 0; i < keys.Size(); i++)
        {
          if(key == keys[i])
            keys.Remove(i);
        }
        return true;
      }
      prev = h;
    }

    return false;
  }



  /**
    Exists
  */
  bool Exists(int key) const
  {
    if(!table)
      return false;

    unsigned int idx = defhash(key) % hashSize;
    for(hashItem_t* h = table[idx]; h; h = h->llnext)
    {
      if(h->key == key)
        return true;
    }

    return false;
  }


  /**
     GetKeys
  */
  const snVector<int>& GetKeys() const
  { return keys; }
};

#endif

