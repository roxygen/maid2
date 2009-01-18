#include"iguitextbox.h"

namespace Maid
{

IGUITextBox::IGUITextBox()
  :m_CursorPos(0)
  ,m_TextLengthByte(0)
  ,m_IMCursorPos(0)
  ,m_IsIMOpen(false)
{

}


void IGUITextBox::SetText( const String& text )
{
  m_InputText = text;
}

const String& IGUITextBox::GetText() const
{
  return m_InputText;

}

void IGUITextBox::SetTextLengthByte( int Length )
{
  m_TextLengthByte = Length;
}
/*
	switch( KeyCode )
	{
	case CKeybord::BUTTON_LEFT: 
		{
			m_CursorPos = max( 0, m_CursorPos-1 );
		}break;

	case CKeybord::BUTTON_RIGHT:
		{
			int32 NewPos = min( int32(m_InputText.length()), m_CursorPos+1 );
			m_CursorPos = NewPos;
		}break;

	case CKeybord::BUTTON_DELETE:
		{
			if( !(m_CursorPos<(int)m_InputText.length()) ) { break; }
			m_InputText.erase( m_CursorPos, 1 );
			CallBack(GUIMESSAGE_TEXTBOX_CHANGETEXT);
		}break;
	}
*/

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 設定されている文字の長さにクリッピングする。
/*!
 */
void	IGUITextBox::ClipText()
{
	if( String::CountByte(m_InputText) <= (int)m_TextLengthByte ) { return ; }

  {
    int byte=0;
    for( int i=0; i<(int)m_InputText.length(); ++i )
    {
      if( String::IsHankaku(m_InputText[i]) )	{ byte += 1; }
      else                                    { byte += 2; }
      if( (int)m_TextLengthByte < byte )
      {
	      m_InputText.resize(i);
	      break;
      }
    }
  }

  m_CursorPos = std::min( m_CursorPos, (int)m_InputText.length() );
}

IGUITextBox::MESSAGERETURN IGUITextBox::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_TEXTBOX_UPDATESTATE:
    {     
      const GUIMESSAGE_TEXTBOX_UPDATESTATE& m = static_cast<const GUIMESSAGE_TEXTBOX_UPDATESTATE&>(*pParam);

      {
        if( m.CharaCode.length()==1 )
        {
          switch( m.CharaCode[0] )
          {
          case '\b':
            {
				      const int DelPos = m_CursorPos-1;

				      if( DelPos<0 ) { return IGUIParts::MessageExecuting( pParam ); }

				      m_InputText.erase( DelPos, 1 );
				      m_CursorPos -= 1;
            }break;
          case '\t':
          case '\n':
          case '\r':
          case 0x1b: { }break;
          default:
	          {
		          m_InputText.insert( m_CursorPos, m.CharaCode );
		          m_CursorPos += 1;
	          }break;
          }

        }else
        {
          m_InputText.insert( m_CursorPos, m.CharaCode );
          m_CursorPos += (int)m.CharaCode.length();
        }
      }
      {
        m_IMText = m.IMCompString;
        m_IMCursorPos = m.IMCompPosition;
        m_IsIMOpen = m.IsIMOpen;
      }

      ClipText();
    }break;
  }

  return IGUIParts::MessageExecuting( pParam );
}

}

