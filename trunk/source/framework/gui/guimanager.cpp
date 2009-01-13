#include"guimanager.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

  class GUIPartsCalback : public IGUIParts
  {
  public:
    GUIPartsCalback( const GUIManager::RECIVEFUNCTION& Func )
      :m_Func(Func)
    {}

  protected:
    void ReciveMessage( ID src, const IGUIParam& Param )
    {
      if( !m_Func ) { return ; }
      m_Func( src, Param );
    }

  private:
    GUIManager::RECIVEFUNCTION  m_Func;
  };




/*!
 	@class	GUIManager guimanager.h
 	@brief	各種GUIPartsにメッセージを送るマネージャ
\n        だけど、各GUIパーツの寿命は管理していないため、気をつけてね

 	@code
  class TittleScene
  {
  public:

  private:
    GUIManager  m_Manager;  // m_Managerのデストラクタでm_Buttonを参照するのでガッ！されることもある(ハード依存)
    GUIButton   m_Button;   
  };

  class BGMScene
  {
  public:

  private:
    GUIButton   m_Button;   
    GUIManager  m_Manager;  こうすると正しくなる
  };
 	@endcode
 */


GUIManager::GUIManager()
:m_pMouseCapturingParts(NULL)
{

}

GUIManager::~GUIManager()
{
  DeletePartsAll();
}

void GUIManager::SetReviceFunction( const RECIVEFUNCTION& Func )
{
  m_pRoot.reset( new GUIPartsCalback(Func) );

  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts.get()!=NULL チェックの必要がなくなる
  m_pMousePrevParts = m_pRoot.get();
}

void GUIManager::UpdateFrame( const Mouse* pMouse, const Keybord* pKey )
{
  //  とりあえず管理してるフレームを進める
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    (*ite)->UpdateFrame();
  }


  //  次に起こっているメッセージを探す

  MESSAGELIST MessageList;

  if( pMouse!=NULL )
  {
    UpdateMouseMessage( *pMouse, MessageList );
  }


  for( size_t i=0; i<MessageList.size(); ++i )
  {
    const MESSAGE& m = MessageList[i];

    m.pParts->SendMessage( *(m.pParam) );
  }
}



void GUIManager::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  for( PARTSLIST::iterator ite =m_PartsList.begin();
                           ite!=m_PartsList.end(); ++ite )
  {
    (*ite)->UpdateDraw(Target, Depth, POINT2DI(0,0) );
  }
}



void GUIManager::RegisterParts( IGUIParts::ID id, IGUIParts& Parts )
{
  GUIMESSAGE_INITIALIZE m;
  m.pParent = m_pRoot.get();
  m.ID = id;

  Parts.SendMessage( m );
  m_PartsList.push_front(&Parts);
}

void GUIManager::DeleteParts( const IGUIParts& Parts )
{
  for( PARTSLIST::const_iterator ite = m_PartsList.begin();
                                  ite != m_PartsList.end(); 
                                  ++ite )
  {
    if( (*ite)->GetID()==Parts.GetID() ) 
    {
      (*ite)->SendMessage( GUIMESSAGE_FINALIZE() );
      m_PartsList.erase( ite );
      break; 
    }
  }

}

void GUIManager::DeletePartsAll()
{
  const GUIMESSAGE_FINALIZE m;

  for( PARTSLIST::const_iterator ite = m_PartsList.begin();
                                  ite != m_PartsList.end(); 
                                  ++ite )
  {
    (*ite)->SendMessage( m );
  }

  m_PartsList.clear();
}




IGUIParts* GUIManager::FindParts( const POINT2DI& pos ) const
{
  for( PARTSLIST::const_iterator  ite  = m_PartsList.begin();
                                  ite != m_PartsList.end();
                                  ++ite )
  {
    IGUIParts* pCurrent = *ite;
    IGUIParts* pChild = pCurrent->FindChildParts(pos);

    if( pChild!=NULL ) { return pChild; }

    if( pCurrent->IsCollision( pos ) ) { return pCurrent; }
  }

  //  ここまできたってことは指定している座標に登録してあるオブジェクトがなかったってことなんだけど
  //  そういうときは何もしないクラスを渡す
  //  これをやっておくことで UpdateMouseMessage() 内で
  //  m_pMousePrevParts.get()!=NULL チェックの必要がなくなる
  return m_pRoot.get();
}



void GUIManager::UpdateMouseMessage( const Mouse& mouse, MESSAGELIST& list )
{
  const POINT2DI pos  = POINT2DI( mouse.GetX(), mouse.GetY() );

  IGUIParts* pCapture = m_pMouseCapturingParts;
  IGUIParts* pNext    = FindParts( pos );
  IGUIParts* pPrev    = m_pMousePrevParts;

  const bool IsCapturing = pCapture!=NULL;


  if( pPrev!=pNext )
  {
    //  現在と今のパーツが違っていたら、パーツのインアウトメッセージ
    if( IsCapturing ) 
    {
      if( pCapture!=pNext )
      {
        //  キャプチャと現在のカーソルの位置のものが違う == キャプチャ先から出た
        boost::shared_ptr<GUIMESSAGE_MOUSEOUT> out( new GUIMESSAGE_MOUSEOUT );
        out->Position = pos;
        list.push_back( MESSAGE(pCapture,out) ); 
      }
      if( pCapture!=pPrev )
      {
        //  キャプチャと１ふれーむ前ののカーソルの位置のものが違う == キャプチャ先に入った
        boost::shared_ptr<GUIMESSAGE_MOUSEIN> in( new GUIMESSAGE_MOUSEIN );
        in->Position = pos;
        list.push_back( MESSAGE(pCapture,in) ); 
      }
    }else
    {
      boost::shared_ptr<GUIMESSAGE_MOUSEOUT> out( new GUIMESSAGE_MOUSEOUT );
      out->Position = pos;
      list.push_back( MESSAGE(pPrev,out) );

      boost::shared_ptr<GUIMESSAGE_MOUSEIN> in( new GUIMESSAGE_MOUSEIN );
      in->Position = pos;
      list.push_back( MESSAGE(pNext,in) );
    }
    m_pMousePrevParts = pNext;
  }




  if( mouse.GetDeltaX()!=0 || mouse.GetDeltaY()!=0 )
  {
    //  マウスが動いたら動いたらメッセージ
    boost::shared_ptr<GUIMESSAGE_MOUSEMOVE> m( new GUIMESSAGE_MOUSEMOVE );
    m->Position = pos;

    if( IsCapturing ) { list.push_back( MESSAGE(pCapture,m) );  }
    else              { list.push_back( MESSAGE(pNext,m) );     }
  }

  //  左ボタンを押したらキャプチャの開始
  if( mouse.IsInL() )
  {
    boost::shared_ptr<GUIMESSAGE_MOUSEDOWN> m( new GUIMESSAGE_MOUSEDOWN );
    m->Position = pos;
    m->Button = IGUIParam::BUTTON_LEFT;

    list.push_back( MESSAGE(pNext,m) );

    m_pMouseCapturingParts = pNext;
  }

  //  左ボタンを離したらキャプチャの終了
  if( mouse.IsOutL() )
  {
    boost::shared_ptr<GUIMESSAGE_MOUSEUP> m( new GUIMESSAGE_MOUSEUP );
    m->Position = pos;
    m->Button = IGUIParam::BUTTON_LEFT;
    list.push_back( MESSAGE(pCapture,m) );
    m_pMouseCapturingParts = NULL;
  }
}

}




