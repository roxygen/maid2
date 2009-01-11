#include"guimanager.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

GUIManager::GUIManager()
  :m_pMouse(NULL),m_pKeybord(NULL)
{

}

GUIManager::~GUIManager()
{
  Finalize();
}

void GUIManager::Initialize( const RECIVEFUNCTION& Func, const Mouse* pMouse, const Keybord* pKey )
{
  Finalize();
  m_pRoot.reset( new IGUIParts );
  m_pMouse   = pMouse;
  m_pKeybord = pKey;
  m_ReciveFunction = Func;
}

void GUIManager::Finalize()
{
  m_pRoot.reset();
  m_PartsList.clear();
  m_pMouse   = NULL;
  m_pKeybord = NULL;
  m_ReciveFunction = RECIVEFUNCTION();
}




void GUIManager::OnUpdateFrame()
{
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


void GUIManager::OnUpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const VECTOR2DI& offset )
{
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    (*ite)->UpdateDraw(Target, Depth );
  }
}

bool GUIManager::LocalIsMouseIn( const POINT2DI& pos ) const
{
  for( PARTSLIST::const_iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    if( (*ite)->IsMouseIn(pos) ) { return true; }
  }

  return false;
}



void GUIManager::RegisterParts( IGUIParts::ID id, const SPGUIPARTS& pParts )
{
  GUIMESSAGE_INITIALIZE m;
  m.pMouse = m_pMouse;
  m.pKey = m_pKeybord;
  m.pParent = this;
  m.ID = id;

  pParts->SendMessage( m );
  m_PartsList.push_front(pParts);
}

void GUIManager::DeleteParts( const SPGUIPARTS& pParts )
{
  MAID_ASSERT( !IsExitstParts(pParts), "指定したパーツは存在していません " << pParts->GetID() );

  for( PARTSLIST::const_iterator ite = m_PartsList.begin();
                                  ite != m_PartsList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==pParts->GetID() ) 
    {
      m_PartsList.erase( ite );
      break; 
    }
  }

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
  //  そういうときは何もしないクラスを渡す
  return m_pRoot;
}


void GUIManager::ReciveMessage( ID src, const IGUIParam& Param )
{
  if( !m_ReciveFunction ) { return ; }
  m_ReciveFunction( src, Param );
}


}




