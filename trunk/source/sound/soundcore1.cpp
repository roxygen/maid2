#include"soundcore.h"


namespace Maid {

SoundCore*  GlobalPointer<SoundCore>::s_pPointer;


SoundCore::SoundCore()
{

}

SoundCore::~SoundCore()
{

}

void SoundCore::Initialize( const INITIALIZEPARAM& param )
{
  GlobalPointer<SoundCore>::Set();

  m_CreateCount = 0;
  m_Param = param;

  if( m_Param.IsBackGround )
  {
    m_AccessThread.SetFunc( MakeThreadObject(&SoundCore::ThreadFunction,this) );
    m_AccessThread.Execute();
  }else
  {
    m_Param.pDevice->Initialize();
  }
}

void SoundCore::Finalize()
{
  if( m_Param.IsBackGround )
  {
    m_AccessThread.Close();
  }else
  {
    m_Param.pDevice->Finalize();
  }
  m_Param.pDevice.reset();

}

void SoundCore::Update()
{
  if( m_Param.IsBackGround ) { return ; }

  for( int i=0; i<(int)m_MessageQue.size(); ++i )
  {
    const SPSOUNDMESSAGE& pMess = m_MessageQue[i];
    ExecuteMessage( *pMess );
  }
  m_MessageQue.clear();
}

SPSOUNDOBJECTINFO    SoundCore::CreateObjectInfo()
{
  ThreadMutexLocker lock(m_ObjectMutex);
  const unt ret = m_CreateCount;
  ++m_CreateCount;
  return SPSOUNDOBJECTINFO( new SoundObjectInfo(ret) );
}

void SoundCore::PostMessage( const SPSOUNDOBJECTINFO& pInfo, const SPSOUNDMESSAGE& pMess )
{
  ThreadMutexLocker lock(m_ObjectMutex);
  pMess->pInfo = pInfo;
  m_MessageQue.push_back(pMess);
}





}


