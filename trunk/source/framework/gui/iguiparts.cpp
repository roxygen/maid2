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

void IGUIParts::SetParent( IGUIParts* pParent )
{
  m_pParent = pParent;
}

IGUIParts::ID   IGUIParts::GetID() const
{
  return m_ID;
}

void IGUIParts::SendMessageParent( const IGUIParam& Param )
{
  if( m_pParent==NULL ) { return ; }
  m_pParent->ReciveMessage( m_ID, Param );
}


void IGUIParts::UpdateFrame()
{
  if( !IsEnable() ) { return ; }

  LocalUpdateFrame();
}

void IGUIParts::UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth )
{
  if( !IsVisible() ) { return ; }
  LocalUpdateDraw( Target, Depth, m_Offset );
}


void IGUIParts::SetPosition( const POINT2DI& pos )
{
  GUIMESSAGE_SETPOSITION m;
  m.Position = pos;
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

POINT2DI IGUIParts::GetPosition() const
{
  return POINT2DI(m_Offset.x,m_Offset.y);
}

bool IGUIParts::IsEnable () const
{
  return m_IsEnable;
}

bool IGUIParts::IsVisible() const
{
  return m_IsVisible;
}

bool IGUIParts::IsMouseIn( const POINT2DI& pos ) const
{
  const POINT2DI p = pos-m_Offset;
  return LocalIsMouseIn( p );
}

const VECTOR2DI& IGUIParts::GetOffset() const
{
  return m_Offset;
}

void IGUIParts::SendMessage( const IGUIParam& Param )
{
    SendMessageParent( Param );
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_SETPOSITION:
    {
      const GUIMESSAGE_SETPOSITION& p = static_cast<const GUIMESSAGE_SETPOSITION&>(Param);
      m_Offset.Reset( p.Position );
      OnPosition( p.Position );
    }break;

  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(Param);
      m_IsEnable = p.IsEnable;
      OnEnable( p.IsEnable );
    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(Param);
      m_IsVisible = p.IsVisible;
      OnVisible( p.IsVisible );
    }break;

  default:
    {
      LocalSendMessage(Param);
    }break;
  }

}

}

