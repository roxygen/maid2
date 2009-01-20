#include"storage.h"

#include"storagelocal.h"

namespace Maid
{

  /*!
      @class  Storage storage.h
      @brief  ストレージ管理クラス
  \n          ストレージとはオンメモリ以外のデータをさすものとします
  \n          ネットワークもストレージ
  */

Storage*  GlobalPointer<Storage>::s_pPointer;


Storage::Storage()
{
}

Storage::~Storage()
{
  Finalize();
  Clear();
}


//! 初期化
/*!
 */
void Storage::Initialize()
{
  GlobalPointer<Storage>::Set();
  m_AccessThread.SetFunc( MakeThreadObject(&Storage::ThreadFunction,this) );
  m_AccessThread.Execute();
}


//! 解放
/*!
 */
void Storage::Finalize()
{
  m_AccessThread.Close();
}


//! 書き込みオブジェクトを発行する
/*!
 */
SPSTORAGEOBJECT Storage::CreateFileWriter()
{
  return SPSTORAGEOBJECT( new StorageObjectFileWriter );
}


//! 読み込みオブジェクトを発行する
/*!
 */
SPSTORAGEOBJECT Storage::CreateFileReader()
{
  return SPSTORAGEOBJECT( new StorageObjectFileReader );
}


//! オブジェクトにメッセージを送る
/*!
 */
void Storage::SendMessage( const SPSTORAGEMESSAGE& pMess )
{
  const unt id = ThreadController::GetCurrentID();
  if( id==m_AccessThread.GetID() )
  {
    //  同じスレッドだったら、その場で処理する。
    ExecuteMessage( pMess );
  }else
  {
    //  違う場合はキューにためておく
     ThreadMutexLocker lock(m_MessageMutex);
     m_MessageQue.push_back( pMess );

     pMess->pObject->m_MessageCount += 1;
  }
}

}
