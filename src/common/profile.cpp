/**
  @file profile.cpp
  @author Christopher Olsen
  @date Fri Sep 24 13:10:55 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "profile.h"
#include "log.h"
#include <stdio.h>

#include "libsn/vector.h"
#include "common/global.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <string.h>

ProfileNode  ProfileManager::root("root", 0);
ProfileNode* ProfileManager::curNode = &ProfileManager::root;

static char textBuf[8192];
static int bufPos = 0;



void ProfileManager::StartProfile(char* name)
{
  curNode = curNode->GetChild(name);
  curNode->Call();
}



void ProfileManager::StopProfile()
{
  curNode->Return();
  curNode = curNode->GetParent();
}



void ProfileManager::StartFrame()
{
  root.Return();
  root.Call();
  root.StartFrame();
}



char* ProfileManager::PrintTree()
{
  memset(textBuf, 0, 8192);
#ifdef PROFILING
  bufPos = 0;
  bufPos += sprintf(textBuf, "Profiler\n");
  bufPos += sprintf(&textBuf[bufPos], "Name               %%%%Parent %%%%Total TotalTime CallTime Calls\n");
  root.Print(0);
  return textBuf;
#endif
  sprintf(textBuf, "Profiling Disabled\n");
  return textBuf;
}



double ProfileManager::GetTotalFrameTime()
{
  return root.prevTotalTime;
}



void ProfileManager::StopProfiling()
{
  for(unsigned int i = 0; i < root.children.Size(); i++)
    root.children[i]->Destroy();
}


















ProfileNode::ProfileNode(char* name, ProfileNode* parent) :
  nodeName(name), startTime(0), updateTime(0),
  totalTime(0), callTime(0), timesCalled(0), nodeParent(parent), 
  prevCallTime(0), prevTotalTime(0), prevTimesCalled(0)
{
}



void ProfileNode::Print(int depth)
{
  for(int i = 0; i < depth; i++)
    bufPos += sprintf(&textBuf[bufPos], " ");

  float percentParent = 100;
  if(nodeParent)
    percentParent = (float)(prevTotalTime / nodeParent->prevTotalTime * 100.0);

  float percentTotal = (float)(prevTotalTime / ProfileManager::GetTotalFrameTime() * 100.0);
  bufPos += sprintf(&textBuf[bufPos], "%s", nodeName);
  for(int i = 0; i < 20 - depth - (int)strlen(nodeName); i++)
    bufPos += sprintf(&textBuf[bufPos], " ");
  if(percentParent < 100.0f) bufPos += sprintf(&textBuf[bufPos], " ");
  if(percentParent < 10.0f)  bufPos += sprintf(&textBuf[bufPos], " ");
  bufPos += sprintf(&textBuf[bufPos], "%2.2f ", percentParent);
  
  if(percentTotal < 100.0f) bufPos += sprintf(&textBuf[bufPos], " ");
  if(percentTotal < 10.0f)  bufPos += sprintf(&textBuf[bufPos], " ");
  bufPos += sprintf(&textBuf[bufPos], "%2.2f ", percentTotal);
  
  if(prevTotalTime * 1000.0 < 100.0f) bufPos += sprintf(&textBuf[bufPos], " ");
  if(prevTotalTime * 1000.0 < 10.0f)  bufPos += sprintf(&textBuf[bufPos], " ");
  bufPos += sprintf(&textBuf[bufPos], "%2.2f ms", prevTotalTime * 1000.0);
  
  if(prevCallTime * 1000.0 < 100.0f) bufPos += sprintf(&textBuf[bufPos], " ");
  if(prevCallTime * 1000.0 < 10.0f)  bufPos += sprintf(&textBuf[bufPos], " ");
  bufPos += sprintf(&textBuf[bufPos], "%2.2f ms ", prevCallTime * 1000.0);
  
  bufPos += sprintf(&textBuf[bufPos], "%3d\n", prevTimesCalled);


  for(unsigned int i = 0; i < children.Size(); i++)
  {
    children[i]->Print(depth+1);
  }
}



ProfileNode* ProfileNode::GetChild(char* name)
{
  for(unsigned int i = 0; i < children.Size(); i++)
  {
    if(children[i]->GetName() == name)
    {
      return children[i];
    }
  }

  LOG(LOG_BLUE, "%s : %s (created)\n", nodeName, name);
  ProfileNode* n = new ProfileNode(name, this);
  n->StartFrame();
  children.PushBack(n);
  return n;
}



void ProfileNode::Call()
{
#ifdef WIN32
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
#else
	struct timeval  tv;
	gettimeofday(&tv, 0);
	startTime = (double)(tv.tv_sec) + ((double)(tv.tv_usec) / 1000000.0f);
#endif
  timesCalled++;
}



void ProfileNode::Return()
{
#ifdef WIN32
  __int64 curTime;
  __int64 freq;
  QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
  QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
  callTime = (float)(curTime - startTime) / (float)freq;
#else
	struct timeval  tv;
	gettimeofday(&tv, 0);
	double curTime = ((double)(tv.tv_sec) + ((double)(tv.tv_usec) / 1000000.0f));
  callTime = curTime - startTime;
#endif
  totalTime += callTime;
}



void ProfileNode::StartFrame()
{
#ifdef WIN32
  __int64 curTime;
  __int64 freq;
  QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
  QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
  float time = (float)(curTime - updateTime) / (float)freq;
  if(time > 0.25)
  {
    QueryPerformanceCounter((LARGE_INTEGER*)&updateTime);
    prevTimesCalled = timesCalled;
    prevTotalTime = totalTime;
    prevCallTime = callTime;
  }

  timesCalled = 0;
  totalTime = 0;
  callTime = 0;

  for(unsigned int i = 0; i < children.Size(); i++)
  {
    children[i]->StartFrame();
  }
#endif
}


void ProfileNode::Destroy()
{
  for(unsigned int i = 0; i < children.Size(); i++)
    children[i]->Destroy();

  delete this;
}
