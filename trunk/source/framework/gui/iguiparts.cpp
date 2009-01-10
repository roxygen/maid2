#include"iguiparts.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_Offset(0,0)
  ,m_ZOrder(0)
  ,m_pListner(new IGUIListner)
  ,m_IsMouseIn(false)
{

}

void IGUIParts::SetParent( IGUIParts* pParent )
{
  m_pParent = pParent;
}

void IGUIParts::SetID( ID id )
{
  MAID_ASSERT( id==PARTSID_NONE,   "この値は使えません" );

  m_ID = id;
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

  m_pListner->UpdateFrame();
  LocalUpdateFrame();
}

void IGUIParts::UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth )
{
  if( !IsVisible() ) { return ; }
  m_pListner->UpdateDraw( Target, Depth, GetPosition() );
  LocalUpdateDraw( Target, Depth, GetPosition() );
}


void IGUIParts::SetPosition( const POINT2DI& pos )
{
  GUIMESSAGE_SETPOSITION m;
  m.Position = pos;
  SendMessage( m );
}

void IGUIParts::SetZOrder( int z )
{
  GUIMESSAGE_SETZORDER m;
  m.Z = z;
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

int  IGUIParts::GetZOrder() const
{
  return m_ZOrder;
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
  return m_pListner->IsMouseIn( p ) || LocalIsMouseIn( p );
}


void IGUIParts::SetListner( const boost::shared_ptr<IGUIParts::IGUIListner>& pListner )
{
  m_pListner = pListner;
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
  case IGUIParam::MESSAGE_INITIALIZE:
    {
      m_pListner->Initialize();
    }break;

  case IGUIParam::MESSAGE_FINALIZE:
    {
      m_pListner->Finalize();
    }break;

  case IGUIParam::MESSAGE_SETPOSITION:
    {
      const GUIMESSAGE_SETPOSITION& p = static_cast<const GUIMESSAGE_SETPOSITION&>(Param);
      m_Offset.Reset( p.Position );
      m_pListner->SetPosition( p.Position );
    }break;

  case IGUIParam::MESSAGE_SETZORDER:
    {
      const GUIMESSAGE_SETZORDER& p = static_cast<const GUIMESSAGE_SETZORDER&>(Param);
      m_ZOrder = p.Z;
      m_pListner->SetZOrder( p.Z );
    }break;

  case IGUIParam::MESSAGE_ENABLE:
    {
      const GUIMESSAGE_ENABLE& p = static_cast<const GUIMESSAGE_ENABLE&>(Param);
      m_IsEnable = p.IsEnable;
      m_pListner->SetEnable( p.IsEnable );
    }break;

  case IGUIParam::MESSAGE_VISIBLE:
    {
      const GUIMESSAGE_VISIBLE& p = static_cast<const GUIMESSAGE_VISIBLE&>(Param);
      m_IsVisible = p.IsVisible;
      m_pListner->SetVisible( p.IsVisible );
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(Param);
      const POINT2DI pos = p.Position-m_Offset;
      const bool in = IsMouseIn( p.Position );

      if( !m_IsMouseIn && in ) 
      {
        m_pListner->MouseIn( pos );
        GUIMESSAGE_MOUSEIN m;
        m.Position = p.Position;
        SendMessageParent( m );
      }
      ef( m_IsMouseIn && !in )
      {
        m_pListner->MouseOut( pos ); 
        GUIMESSAGE_MOUSEOUT m;
        m.Position = p.Position;
        SendMessageParent( m );
      }

      m_IsMouseIn = in;
    }break;

  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
    //  if( IsMouseIn( p.Position ) )
      {
        const POINT2DI pos = p.Position-m_Offset;
        m_pListner->MouseDown( pos, p.Button );
      }
    }break;


  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
    //  if( IsMouseIn( p.Position ) )
      {
        const POINT2DI pos = p.Position-m_Offset;
        m_pListner->MouseUp( pos, p.Button );
      }
    }break;
  }

  LocalSendMessage(Param);
}

}

