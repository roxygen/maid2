#include"iguiparts.h"

namespace Maid
{

IGUIParts::IGUIParts()
  :m_ID(PARTSID_NONE)
  ,m_pParent(NULL)
  ,m_IsEnable(true)
  ,m_IsVisible(true)
  ,m_Position(0,0)
  ,m_ZOrder(0)
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
  m_pParent->ReturnMessage( m_ID, Param );
}


void IGUIParts::UpdateFrame()
{
  if( !IsEnable() ) { return ; }
  LocalUpdateFrame();
}

void IGUIParts::UpdateDraw( RenderTargetBase& Target )
{
  if( !IsVisible() ) { return ; }
  LocalUpdateDraw( Target, GetPosition() );
}


void IGUIParts::SetPosition( const POINT2DI& pos )
{
  m_Position = pos;

  GUIMESSAGE_SETPOSITION m;
  m.Position = pos;
  SendMessage( m );
}

void IGUIParts::SetZOrder( int z )
{
  m_ZOrder = z;
  GUIMESSAGE_SETZORDER m;
  m.Z = z;
  SendMessage( m );
}

void IGUIParts::SetEnable ( bool IsEnable )
{
  m_IsEnable = IsEnable;
  GUIMESSAGE_ENABLE m;
  m.IsEnable = IsEnable;
  SendMessage( m );
}

void IGUIParts::SetVisible( bool IsVisible )
{
  m_IsVisible = IsVisible;
  GUIMESSAGE_VISIBLE m;
  m.IsVisible = IsVisible;
  SendMessage( m );
}

const POINT2DI& IGUIParts::GetPosition() const
{
  return m_Position;
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

void IGUIParts::SetDefaultListner( const boost::shared_ptr<Listner>& pListner )
{
  m_pListner = pListner;
}


void GUIButton::SendMessage( const IGUIParam& Param )
{
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
      m_pListner->Finalize();

    }break;
  case IGUIParam::MESSAGE_SETZORDER:
    {

    }break;
  case IGUIParam::MESSAGE_ENABLE:
    {

    }break;
  case IGUIParam::MESSAGE_VISIBLE:
    {

    }break;
  }
}

}

