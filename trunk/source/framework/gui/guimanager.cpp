#include"guimanager.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

GUIManager::GUIManager()
  :m_pMouse(NULL),m_pKeybord(NULL),m_IsEnable(false),m_IsVisible(false)
{

}

GUIManager::~GUIManager()
{
  Finalize();
}

void GUIManager::Initialize( const Mouse* pMouse, const Keybord* pKey )
{
  Finalize();

  m_pRoot.reset( new IGUIParts );
  m_pMouse   = pMouse;
  m_pKeybord = pKey;
  m_IsEnable  = true;
  m_IsVisible = true;
}

void GUIManager::Finalize()
{
  m_pRoot.reset();
  m_IsEnable  = false;
  m_IsVisible = false;
  m_PartsList.clear();
  m_pMouse   = NULL;
  m_pKeybord = NULL;
}


void GUIManager::UpdateFrame()
{
  if( !m_IsEnable ) { return ; }

  //  とりあえず管理してるフレームを進める
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    (*ite)->UpdateFrame();
  }


  //  次に起こっているメッセージを探す



  MESSAGELIST MessageList;

  GenerateMouseMessage( MessageList );

  for( size_t i=0; i < MessageList.size(); ++i )
  {
    const MESSAGE& m = MessageList[i];
    m.pParts->SendMessage( *(m.pParam) );
  }
}


void GUIManager::GenerateMouseMessage( MESSAGELIST& list )
{
  if( m_pMouse==NULL ) { return ; }

  const Mouse& m = *m_pMouse;
  const POINT2DI pos( m.GetX(), m.GetY() );


  if( m_pMouseDownParts.get()==NULL )
  {
    //  m_pMouseDownParts==NULL つまりマウス左ボタンが離しっぱなし
    SPGUIPARTS pCurrent = FindParts( pos );
    const SPGUIPARTS& pPrev = m_pMousePrevParts;
    const bool IsChange = pPrev.get()!=NULL && (pPrev.get()!=pCurrent.get());

    if( m.GetDeltaX()!=0 || m.GetDeltaY()!=0 )
    {
      boost::shared_ptr<GUIMESSAGE_MOUSEMOVE> p( new GUIMESSAGE_MOUSEMOVE );
      p->Position = pos;
      list.push_back( MESSAGE(pCurrent,p) );

      if( IsChange ) { list.push_back( MESSAGE(pPrev,p) ); }
    }

    if( m.IsInL() )
    {
      boost::shared_ptr<GUIMESSAGE_MOUSEDOWN> p( new GUIMESSAGE_MOUSEDOWN );
      p->Position = pos;
      p->Button = 0;
      list.push_back( MESSAGE(pCurrent,p) );
      m_pMouseDownParts = pCurrent;
    }

    m_pMousePrevParts = pCurrent;
  }else
  {
    //  こっちはマウス左ボタンが押しっぱなしのときにくる
    SPGUIPARTS& pCurrent = m_pMouseDownParts;

    if( m.GetDeltaX()!=0 || m.GetDeltaY()!=0 )
    {
      boost::shared_ptr<GUIMESSAGE_MOUSEMOVE> p( new GUIMESSAGE_MOUSEMOVE );
      p->Position = pos;
      list.push_back( MESSAGE(pCurrent,p) );
    }

    if( m.IsOutL() )
    {
      boost::shared_ptr<GUIMESSAGE_MOUSEUP> p( new GUIMESSAGE_MOUSEUP );
      p->Position = pos;
      p->Button = 0;
      list.push_back( MESSAGE(pCurrent,p) );
      pCurrent.reset();
    }
  }
}



void GUIManager::UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth )
{
  if( !m_IsVisible ) { return ; }
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    (*ite)->UpdateDraw(Target, Depth );
  }
}

void GUIManager::SetEnable( bool IsEnable )
{
  m_IsEnable = IsEnable;
}

void GUIManager::SetVisible( bool IsVisible )
{
  m_IsVisible = IsVisible;
}

void GUIManager::RegisterParts( const SPGUIPARTS& pParts )
{
  pParts->SendMessage( GUIMESSAGE_INITIALIZE() );
  m_PartsList.insert(pParts);
}

void GUIManager::DeleteParts( const SPGUIPARTS& pParts )
{
  MAID_ASSERT( !IsExitstParts(pParts), "指定したパーツは存在していません " << pParts->GetID() );

  m_PartsList.erase( pParts );
  pParts->SendMessage( GUIMESSAGE_FINALIZE() );
}

bool GUIManager::IsExitstParts( const SPGUIPARTS& pParts ) const
{
  for( PARTSLIST::const_iterator ite = m_PartsList.begin();
                                  ite != m_PartsList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==pParts->GetID() ) { return true; }
  }

  return false;
}

SPGUIPARTS GUIManager::FindParts( IGUIParts::ID id ) const
{
  for( PARTSLIST::const_iterator ite = m_PartsList.begin();
                                  ite != m_PartsList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==id ) { return (*ite); }
  }

  MAID_ASSERT( true, "指定したパーツは存在していません " << id );

  return SPGUIPARTS();
}



SPGUIPARTS GUIManager::FindParts( const POINT2DI& pos ) const
{
  for( PARTSLIST::const_iterator  ite  = m_PartsList.begin();
                                  ite != m_PartsList.end();
                                  ++ite )
  {
    if( (*ite)->IsMouseIn( pos ) ) { return (*ite); }
  }

  //  ここまできたってことは指定している座標に登録してあるオブジェクトがなかったってことなんだけど
  //  そういうときはデスクトップ宛にメッセージを送ることにする
  return m_pRoot;
}


}




