#include"guibutton.h"



namespace Maid
{

GUIButton::GUIButton()
  :m_pListner(new Listner)
{

}

void GUIButton::SetListner( const boost::shared_ptr<Listner>& pListner )
{
  IGUIParts::SetListner( pListner );
  m_pListner = pListner;
}


void GUIButton::LocalSendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      if( IsMouseIn( p.Position ) )
      {
        m_pListner->m_ButtonDown = true;
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);
      if( IsMouseIn( p.Position ) )
      {
        if( m_pListner->m_ButtonDown )
        {
          const POINT2DI pos = p.Position - GetOffset();
          m_pListner->ButtonClick( pos );

          GUIMESSAGE_BUTTON_CLICK m;
          m.Position = p.Position;
          SendMessageParent( m );
        }
      }
      m_pListner->m_ButtonDown = false;
    }break;
  }
}

}
