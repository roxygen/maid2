#include"storage.h"

#include"storagelocal.h"
#include"storagemessage3.h"

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
  GlobalPointer<Storage>::Set();
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

SPSTORAGEOBJECT Storage::ExecuteFunction( const SPSTORAGEFUNCTION& pFunc )
{
  boost::shared_ptr<StorageObjectFunction> p( new StorageObjectFunction );

  p->pFunction = pFunc;

  {
    boost::shared_ptr<StorageMessage::Function> pMess( new StorageMessage::Function );
    pMess->pObject = p;
    SendMessage( pMess );
  }

  return p;
}

void Storage::SendMessage( const SPSTORAGEMESSAGE& pMess )
{
  if( ThreadController::GetCurrentID()==m_AccessThread.GetID() )
  {
    //  同じスレッドだったら、その場で処理する。
    ExecuteMessage( pMess );
  }else
  {
    //  違う場合はキューにためておく
     ThreadMutexLocker lock(m_MessageMutex);
     m_MessageQue.push_back( pMess );

     pMess->pObject->MessageCount += 1;
  }
}

}
