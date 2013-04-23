/**
  @file profile.h
  @author Christopher Olsen
  @date Fri Sep 24 12:55:03 2004

  Copyright (C) 2004 Christopher A Olsen

  @todo need a way to traverse profile tree
*/
#ifndef __PROFILE__
#define __PROFILE__




#include "libsn/vector.h"
class ProfileNode;

class ProfileManager
{
private:
  static ProfileNode root;
  static ProfileNode* curNode;

public:
  static void StartProfile(char* name);
  static void StopProfile();
  static void StartFrame();
  static ProfileNode* FindNode(ProfileNode* n, char* name);
  static char* PrintTree();
  static double GetTotalFrameTime();
  static void StopProfiling();
};



class ProfileNode
{
public:
  char* nodeName;

#ifdef WIN32
  __int64 startTime;
  __int64 updateTime;
#else
  double startTime;
  double updateTime;
#endif
  double totalTime;
  double callTime;

  unsigned int timesCalled;
  ProfileNode* nodeParent;

  // previous frame info
  double prevCallTime;
  double prevTotalTime;
  unsigned int prevTimesCalled;
  

public:
  ProfileNode(char* name, ProfileNode* parent);
  ProfileNode* GetChild(char* name);

  void Call();
  void Return();
  void Print(int depth);
  void StartFrame();
  void Destroy();

  char* GetName()          { return nodeName; }
  double GetTotalTime()    { return prevTotalTime; }
  double GetAverageTime()  { return prevTotalTime / (double)prevTimesCalled; }
  ProfileNode* GetParent() { return nodeParent; }

  snVector<ProfileNode*> children;
};



class ProfileSample
{
public:
  ProfileSample(char* name)
  {
    ProfileManager::StartProfile(name);
  }


  ~ProfileSample()
  {
    ProfileManager::StopProfile();
  }
};



#ifdef PROFILING
#define PROFILE(name) ProfileSample __profileSample(name);
#else
#define PROFILE(name)
#endif

#endif
