#include"iguibutton.h"



namespace Maid
{

	/*!
	 	  @class	IGUIButton iguibutton.h
	 	  @brief	ボタンをあらわすクラス
  \n          押せるだけってことですね。
	 */ 

//! ボタンをクリックした時に呼ばれる
/*!
    @param  pos [i ]  クリックした座標
 */
void IGUIButton::OnMouseClick( const POINT2DI& pos ){}



IGUIButton::IGUIButton()
  :m_IsButtonDown(false)
{
}


IGUIButton::MESSAGERETURN IGUIButton::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*Param);
      m_IsButtonDown = true;
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*Param);

      if( m_IsButtonDown )
      {
        m_IsButtonDown = false;

        if( IsCollision(m.Position) )
        {
          OnMouseClick( CalcLocalPosition(m.Position) );

          GUIMESSAGE_BUTTON_CLICK mess;
          mess.Position = m.Position;
          mess.Button   = m.Button;

          PostMessage( mess );
        }
      }
    }break;
  }

  return IGUIParts::MessageExecuting( Param );
}

}
