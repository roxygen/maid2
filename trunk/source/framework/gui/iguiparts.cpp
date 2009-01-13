#include"iguiparts.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_Offset(0,0)
{

}


IGUIParts::~IGUIParts()
{
  MAID_ASSERT( m_ID!=PARTSID_NONE, "IGUIParam::MESSAGE_FINALIZE が来ていないのに、開放されようとしてます " );
}



void IGUIParts::SendMessageParent( const IGUIParam& Param )
{
  if( m_pParent==NULL ) { return ; }
  m_pParent->ReciveMessage( m_ID, Param );
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
  GUIMESSAGE_SETOFFSET m;
  m.Offset = offset;
  SendMessage( m );
}

void IGUIParts::SetEnable ( bool IsEnable )
{
  GUIMESSAGE_ENABLE m;
  m.IsEnable = IsEnable;
  SendMessage( m );
}

void IGUIParts::SetVisible( bool IsVisible )
{
  GUIMESSAGE_VISIBLE m;
  m.IsVisible = IsVisible;
  SendMessage( m );
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

  return LocalFindChildParts( p );
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


void IGUIParts::SendMessage( const IGUIParam& Param )
{
  //  まずは自分で解釈する。・・・んだけど、いつも使いそうなメッセージは
  //  勝手に実装してしまう。　このswitch の処理を変更したい場合はこのクラスを再実装すべき
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_INITIALIZE:
    {
      const GUIMESSAGE_INITIALIZE& p = static_cast<const GUIMESSAGE_INITIALIZE&>(Param);
      MAID_ASSERT( p.ID==PARTSID_NONE, "このIDは使用できません " << p.ID );

      m_ID = p.ID;
      m_pParent = p.pParent;

      if( OnInitialize( p.ID, p.pParent )==MESSAGERETURN_NONE ) { return ; }
    }break;

  case IGUIParam::MESSAGE_FINALIZE:
    {
      const GUIMESSAGE_FINALIZE& p = static_cast<const GUIMESSAGE_FINALIZE&>(Param);

      m_pParent = NULL;
      m_ID = PARTSID_NONE;

      if( OnFinalize()==MESSAGERETURN_NONE ) { return ; }
    }break;

  case IGUIParam::MESSAGE_SETOFFSET:
    {
      const GUIMESSAGE_SETOFFSET& p = static_cast<const GUIMESSAGE_SETOFFSET&>(Param);
      m_Offset = p.Offset;
    }break;

  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(Param);
      m_IsEnable = p.IsEnable;
    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(Param);
      m_IsVisible = p.IsVisible;
    }break;

  case IGUIParam::MESSAGE_MOUSEIN:
    {
      const GUIMESSAGE_MOUSEIN& p = static_cast<const GUIMESSAGE_MOUSEIN&>(Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      m_IsMouseIn = true;
      if( OnMouseIn( pos )==MESSAGERETURN_NONE ) { return ; }
    }break;

  case IGUIParam::MESSAGE_MOUSEOUT:
    {
      const GUIMESSAGE_MOUSEOUT& p = static_cast<const GUIMESSAGE_MOUSEOUT&>(Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      m_IsMouseIn = false;
      if( OnMouseOut( pos )==MESSAGERETURN_NONE ) { return ; }
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      if( OnMouseMove( pos )==MESSAGERETURN_NONE ) { return ; }
    }break;

  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();
      if( OnMouseDown( pos, p.Button )==MESSAGERETURN_NONE ) { return ; }
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
      const POINT2DI pos = p.Position-CalcTotalOffset();

      if( OnMouseUp( pos, p.Button)==MESSAGERETURN_NONE ) { return ; }
    }break;
  }

  //  継承先でも判断してもらう
  if( OnSendMessage(Param)==MESSAGERETURN_NONE ) { return ; }

  //  イベントが起こったことを親に通知！
  SendMessageParent( Param );
}

}

