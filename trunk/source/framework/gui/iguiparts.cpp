#include"iguiparts.h"
#include"guimessageque.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_Offset(0,0)
  ,m_pPostTarget(NULL)
{

}


IGUIParts::~IGUIParts()
{
  MAID_ASSERT( m_ID!=PARTSID_NONE, "IGUIParam::MESSAGE_FINALIZE が来ていないのに、開放されようとしてます " );
}

  void IGUIParts::UpdateFrame()
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsEnable() ) { return ; }
    OnUpdateFrame();
  }
}

void IGUIParts::UpdateDraw( const RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos )
{
  {
    //  この処理は SendMessage() 内でやるべきなんだけど、パフォーマンスに影響でそうなのでここでやる
    if( !IsVisible() ) { return ; }
    OnUpdateDraw( Target, Depth, pos + m_Offset );
  }
}


void IGUIParts::SetOffset( const VECTOR2DI& offset )
{
  boost::shared_ptr<GUIMESSAGE_SETOFFSET> p( new GUIMESSAGE_SETOFFSET );
  p->Offset = offset;
  SendMessage( p );
}

void IGUIParts::SetEnable ( bool IsEnable )
{
  boost::shared_ptr<GUIMESSAGE_ENABLE> p( new GUIMESSAGE_ENABLE );
  p->IsEnable = IsEnable;
  SendMessage( p );
}

void IGUIParts::SetVisible( bool IsVisible )
{
  boost::shared_ptr<GUIMESSAGE_VISIBLE> p( new GUIMESSAGE_VISIBLE );
  p->IsVisible = IsVisible;
  SendMessage( p );
}

const VECTOR2DI& IGUIParts::GetOffset() const
{
  return m_Offset;
}


bool IGUIParts::IsEnable () const
{
  return m_IsEnable;
}

bool IGUIParts::IsVisible() const
{
  return m_IsVisible;
}

IGUIParts::ID   IGUIParts::GetID() const
{
  return m_ID;
}


bool IGUIParts::IsCollision( const POINT2DI& pos ) const
{
  if( !IsEnable() ) { return false; }

  const POINT2DI p = pos-m_Offset;
  return LocalIsCollision( p );
}



IGUIParts* IGUIParts::FindChildParts( const POINT2DI& pos )
{
  const POINT2DI p = pos-m_Offset;

  for( CHILDLIST::iterator ite =m_ChildList.begin();
                           ite!=m_ChildList.end();
                           ++ite )
  {
    IGUIParts* pChild = (*ite)->FindChildParts( pos );
    if( pChild!=NULL ) { return pChild; }
  }

  if( LocalIsCollision(pos) ) { return this; }

  return NULL;
}



void IGUIParts::AddChildParts( IGUIParts::ID id, IGUIParts* p )
{

}

void IGUIParts::DelChildParts( IGUIParts* p )
{

}


void IGUIParts::ChangeZOrder( IGUIParts* p, ZORDER type )
{

}


VECTOR2DI IGUIParts::CalcTotalOffset() const
{
  VECTOR2DI ret(0,0);
  const IGUIParts* p = this;

  while( true )
  {
    if( p==NULL ){ break; }

    ret += p->GetOffset();
    p = p->GetParent();
  }

  return ret;
}

void IGUIParts::PostMessage( const SPGUIPARAM& pParam )
{
  MAID_ASSERT( m_pPostTarget!=NULL, "投稿先がありません" );
  m_pPostTarget->PostMessage( this, pParam );
}


void IGUIParts::SendMessage( const SPGUIPARAM& pParam )
{
  SPGUIPARAM p = pParam;

  if( MessageExecuting( p )==MESSAGERETURN_SENDPARENT )
  {
    if( m_pParent==NULL ) { return; }
    m_pParent->ReciveMessage( m_ID, *p );
  }
}


IGUIParts::MESSAGERETURN IGUIParts::MessageExecuting( SPGUIPARAM& Param )
{
  //  ここでメッセージの処理を行います
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_ADDCHILED:
    {
      const GUIMESSAGE_ADDCHILED& p = static_cast<const GUIMESSAGE_ADDCHILED&>(*Param);
      MAID_ASSERT( p.ID==PARTSID_NONE, "このIDは使用できません " << p.ID );

      m_ID = p.ID;
      m_pParent = p.pParent;
      m_pPostTarget = p.pPostTarget;

      OnInitialize( p.ID, p.pParent );
    }break;

  case IGUIParam::MESSAGE_DELCHILED:
    {
      const GUIMESSAGE_DELCHILED& p = static_cast<const GUIMESSAGE_DELCHILED&>(*Param);

      m_pParent = NULL;
      m_ID = PARTSID_NONE;

      OnFinalize();
    }break;

  case IGUIParam::MESSAGE_SETOFFSET:
    {
      const GUIMESSAGE_SETOFFSET& p = static_cast<const GUIMESSAGE_SETOFFSET&>(*Param);
      m_Offset = p.Offset;
      OnSetOffset( p.Offset );
    }break;

  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(*Param);
      m_IsEnable = p.IsEnable;
      OnEnable( p.IsEnable );
    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(*Param);
      m_IsVisible = p.IsVisible;
      OnVisible( p.IsVisible );
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(*Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      m_IsMouseIn = true;
      OnMouseIn( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(*Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      m_IsMouseIn = false;
      OnMouseOut( pos );
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      OnMouseMove( pos );
    }break;
  }

  return MESSAGERETURN_SENDPARENT;
}


}

