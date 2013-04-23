#include "libsn/string.h"
#include <string.h>
#include <stdio.h>

snString::snString() :
  length(0),
  data(0),
  allocatedSize(0),
  resized(false)
{
  data = initialBuffer;
  memset(initialBuffer, 0, sizeof(initialBuffer));
}

snString::snString(int size) :
  length(0),
  data(0),
  allocatedSize(0),
  resized(false)
{
  data = initialBuffer;
  memset(initialBuffer, 0, sizeof(initialBuffer));
  
  if(size)
  {
    CheckResize(size);
    length = size;
  }
}

snString::snString(const char *text) :
  length(0),
  data(0),
  allocatedSize(0),
  resized(false)
{
  data = initialBuffer;
  memset(initialBuffer, 0, sizeof(initialBuffer));
  
  if(text)
  {
    int len = strlen(text);
    CheckResize(len);
    strcpy(data, text);
    length = len;
  }
}

snString::snString(const snString& str) :
  length(0),
  data(0),
  allocatedSize(0),
  resized(false)
{
  data = initialBuffer;
  memset(initialBuffer, 0, sizeof(initialBuffer));
  
  if(str.CString())
  {
    int len = str.Length();
    CheckResize(len);
    strcpy(data, str.CString());
    length = len;
  }
}

snString::~snString()
{
  if(resized)
  {
    delete data;
    data = 0;
  }
}

void snString::CheckResize(unsigned int size)
{
  if(size > allocatedSize)
    Resize(size);
}

void snString::Resize(unsigned int newSize)
{
  char *newData;
  int actualNewSize;
  
  int mod = newSize % INITIAL_BUFFER_SIZE;
  if(!mod)
    actualNewSize = newSize;
  else
    actualNewSize = newSize + INITIAL_BUFFER_SIZE - mod;

  allocatedSize = actualNewSize;

  newData = new char[allocatedSize];
  strcpy(newData, data);
  
  if(resized)
  {
    delete data;
    data = 0;
  }

  data = newData;
}

void snString::Append(const snString& str)
{
  Append(str.CString());
}

void snString::Append(const char c)
{
  CheckResize(length + 2);
  data[length] = c;
  length++;
  data[length] = '\0';
}
void snString::Append(const char *text)
{
  if(!text)
    return;

  int newLength = length + strlen(text);
  CheckResize(newLength+1);

  for(int i = 0; text[i] != '\0'; i++)
    data[i+length] = text[i];

  length = newLength;
  data[length] = '\0';   
}

void snString::Insert(unsigned int index, const char c)
{
  if(index < 0)
    index = 0;
  else if(index > length)
    index = length;

  CheckResize(length + 1);

  for(unsigned int i = length; i >= index; i--)
    data[i+1] = data[i];

  data[index] = c;

  length++;
}

void snString::Insert(unsigned int index, const char *text)
{
  if(index < 0)
    index = 0;
  else if(index > length)
    index = length;

  int textlen = strlen(text);
  CheckResize(length + textlen + 1);

  for(unsigned int i = length; i >= index; i--)
    data[i+textlen] = data[i];

  for(int i = 0; i < textlen; i++)
    data[i+index] = text[i];

  length += textlen;
}

void snString::Insert(unsigned int index, const snString& str)
{
  const char* text = str.CString();
  
  if(index < 0)
    index = 0;
  else if(index > length)
    index = length;

  unsigned int textlen = str.Length();
  CheckResize(length + textlen + 1);

  for(unsigned int i = length; i >= index; i--)
    data[i+textlen] = data[i];

  for(unsigned int i = 0; i < textlen; i++)
    data[i+index] = text[i];

  length += textlen;
}

void snString::Remove(unsigned int index)
{
  if(index < 0)
    index = 0;
  else if(index > length)
    index = length;

  for(unsigned int i = index; i <= length; i++)
    data[i] = data[i+1];

  length--;
}


snString& snString::operator=(const char *text)
{
  int len = strlen(text);

  CheckResize(len+1);

  sprintf(data, text);

  data[len] = '\0';
  length = len;
  
  return *this;
}

snString& snString::operator=(const snString& text)
{
  const char* str = text.CString();
  int len = text.Length();
  CheckResize(len+1);
  sprintf(data, str);
  data[len] = '\0';
  length = len;
  return *this;
}

bool snString::operator==(const snString& text)
{
  return (strcmp(text.CString(), data) == 0);
}

bool snString::operator==(const char *text)
{
  return (strcmp(text, data) == 0);
}


const char snString::operator[](unsigned int i) const
{
  if(i >= length)
    throw "String index out of bounds";

  return data[i];
}

char& snString::operator[](unsigned int i)
{
  if(i >= length)
    throw "String index out of bounds";

  return data[i];
}

