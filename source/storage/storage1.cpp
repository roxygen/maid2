#include"storage.h"

#include"storagelocal.h"

namespace Maid
{

Storage*  GlobalPointer<Storage>::s_pPointer;


Storage::Storage()
{
  Set();
}

Storage::~Storage()
{
  Finalize();
  Clear();
}

void Storage::Initialize()
{
  m_AccessThread.SetFunc( MakeThreadObject(&Storage::ThreadFunction,this) );
  m_AccessThread.Execute();
}

void Storage::Finalize()
{
  m_AccessThread.Close();

}

SPSTORAGEOBJECT Storage::CreateFileWriter()
{
  return SPSTORAGEOBJECT( new StorageObjectFileWriter );
}

SPSTORAGEOBJECT Storage::CreateFileReader()
{
  return SPSTORAGEOBJECT( new StorageObjectFileReader );
}

void Storage::SendMessage( const SPSTORAGEMESSAGE& pMess )
{
   ThreadMutexLocker lock(m_MessageMutex);
   m_MessageQue.push_back( pMess );

   pMess->pObject->MessageCount += 1;
}

}
