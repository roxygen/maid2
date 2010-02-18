﻿#include"id3d09object.h"

namespace Maid { namespace Graphics {

std::set<ID3D09Object*>  ID3D09Object::s_ObjectList;
ThreadMutex  ID3D09Object::s_Mutex;


ID3D09Object::ID3D09Object()
{
  ThreadMutexLocker lock(s_Mutex);
  s_ObjectList.insert(this);
}

ID3D09Object::~ID3D09Object()
{
  ThreadMutexLocker lock(s_Mutex);
  s_ObjectList.erase(this);
}

void ID3D09Object::EscapeAll()
{
  ThreadMutexLocker lock(s_Mutex);
  for( std::set<ID3D09Object*>::iterator ite  = s_ObjectList.begin();
                                         ite != s_ObjectList.end();
                                         ++ite )
  {
    (*ite)->Escape();
  }

}

void ID3D09Object::RestoreAll( DeviceD3D09& Device )
{
  ThreadMutexLocker lock(s_Mutex);
  for( std::set<ID3D09Object*>::iterator ite  = s_ObjectList.begin();
                                         ite != s_ObjectList.end();
                                         ++ite )
  {
    (*ite)->Restore( Device );
  }
}


}}

