/**
  @file hardwareBuffer.h
  @author Christopher Olsen
  @date Fri Apr  8 01:18:36 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __HARDWARE_BUFFER__
#define __HARDWARE_BUFFER__

class HardwareBuffer
{
protected:
  unsigned int size;
  unsigned int locked;
  unsigned int usage;

  virtual void* _Lock(unsigned int lockType) = 0;
  virtual void  _Unlock() = 0;

public:
  HardwareBuffer(unsigned int _usage) : usage(_usage)
    { }
  virtual ~HardwareBuffer() {}

  unsigned int GetSize()  { return size; }
  unsigned int IsLocked() { return locked; }
  unsigned int GetUsage() { return usage; }

  void* Lock(unsigned int lockType)
  {
    if(!locked)
      return _Lock(lockType);
    return 0;
  }


  void Unlock()
  {
    if(locked)
      _Unlock();
  }
};

#endif
