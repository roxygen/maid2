#include"gameframework.h"

#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/exception.h"

#include"../osmessageimpl1.h"


namespace Maid
{
using namespace OSMessage;

Window::CALLBACKCODE GameFrameWork::ExecMessage( const OSMessage::Base& mess )
{
  const OSMESSAGESTATE ret = OnOSMessage( mess );

  if( ret==IGameThread::OSMESSAGESTATE_RETURN ) { return Window::CALLBACKCODE_RETURN; }

  return Window::CALLBACKCODE_DEFAULT;
}

Window::CALLBACKCODE GameFrameWork::wmClose( WindowsMessage& msg )
{
	return ExecMessage(Base(Base::PUSHCLOSEBUTTON));
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WM_ACTIVE がきたときの処理
/*!
 *	\param	Param	[i ]	メッセージ内容
 */
Window::CALLBACKCODE GameFrameWork::wmActivate( WindowsMessage& msg )
{
	const bool IsMinimized = msg.GetWPARAM(WindowsMessage::PARAM_HI) != 0;

	bool IsActive = false;

	switch( msg.GetWPARAM(WindowsMessage::PARAM_LO) )
	{
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		{
			if( IsMinimized )
			{
				return Window::CALLBACKCODE_DEFAULT;
			}else
			{
				IsActive = true;
			}
		}break;


	case WA_INACTIVE:
		{
			IsActive = false;
		}break;
	}

	OSMessage::Activate Mess;
	Mess.IsActive = IsActive;

	return ExecMessage(Mess);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WM_NCLBUTTONDBLCLKがきたときの処理
/*!
 *	\param	Param	[i ]	メッセージ内容
 */
Window::CALLBACKCODE	GameFrameWork::wmNcLButtonDblClk( WindowsMessage& msg ) 
{
	//　タイトルバーをダブルクリックしたときの最大化は
	//	WM_SYSCOMMAND::SC_MAXIMIZE　を通らないのでここではじく
	if( (INT)msg.GetWPARAM()==HTCAPTION ) { return Window::CALLBACKCODE_RETURN; }

	return Window::CALLBACKCODE_DEFAULT;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WM_SYSCOMMAND がきたときの処理
/*!
 *	\param	Param	[i ]	メッセージ内容
 */
Window::CALLBACKCODE	GameFrameWork::wmSysCommand( WindowsMessage& msg ) 
{
	switch( msg.GetWPARAM() )
	{
	case SC_MAXIMIZE:	//　最大化ボタンを押したとき
		{
    	ExecMessage(Base(Base::PUSHMAXIMIZEBUTTON));
			return Window::CALLBACKCODE_RETURN;
		}
		break;

	case SC_MINIMIZE:	//	最小化ボタンを押したとき
		{
		}
		break;

	case SC_RESTORE:
		{
		}
		break;
	}

	return Window::CALLBACKCODE_DEFAULT;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WM_KEYDOWN がきたときの処理
/*!
 *	\param	Param	[i ]	メッセージ内容
 */
Window::CALLBACKCODE	GameFrameWork::wmKeyDown( WindowsMessage& msg )
{
	switch( msg.GetWPARAM() )
	{
	case VK_F1:
		{	// F1 でフルスクリーンからウィンドウモードへ切り替える
			if( !IsBitOn(msg.GetLPARAM(),30) )
			{
			}
		}
		break;
	}

	return Window::CALLBACKCODE_DEFAULT;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WM_PAINT がきたときの処理
/*!
 *	\param	Param	[i ]	メッセージ内容
 */
Window::CALLBACKCODE GameFrameWork::wmSysKeyDown( WindowsMessage& msg )
{
/*
	switch( Param.GetWPARAM() )
	{
	case VK_RETURN:
		{
			if( IsBitOn(Param.GetLPARAM(),29) )
			{
				const CVideoCardDriverD3D9* pDriver = static_cast<const CVideoCardDriverD3D9*>(m_Graphic.GetVideoCardDriver().get());
				const IVideoCardDriver::SCREENMODE     bpp = m_Graphic.GetInitState().ScreenMode;
				if( bpp!=IVideoCardDriver::SCREENMODE_WINDOW )
				{
					ChangeScreenMode( IVideoCardDriver::SCREENMODE_WINDOW, false );
				}else
				{
					if( pDriver->GetBackBufferFormat(IVideoCardDriver::SCREENMODE_FULL_32BIT)!=D3DFMT_UNKNOWN )	{ ChangeScreenMode( IVideoCardDriver::SCREENMODE_FULL_32BIT, false );	}
					ef( pDriver->GetBackBufferFormat(IVideoCardDriver::SCREENMODE_FULL_16BIT)!=D3DFMT_UNKNOWN )	{ ChangeScreenMode( IVideoCardDriver::SCREENMODE_FULL_16BIT, false );	}
				}

			}
		}break;
	}
*/
	return Window::CALLBACKCODE_DEFAULT;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WM_PAINT がきたときの処理
/*!
 *	\param	Param	[i ]	メッセージ内容
 */
Window::CALLBACKCODE	GameFrameWork::wmPaint( WindowsMessage& msg )
{
	PAINTSTRUCT ps;

	::BeginPaint( msg.GetHWND(), &ps );

	if( ps.rcPaint.bottom-ps.rcPaint.top  == 0 ) { goto NOBLT; }
	if( ps.rcPaint.right -ps.rcPaint.left == 0 ) { goto NOBLT; }

 	ExecMessage(Base(Base::REDRAW));

NOBLT:
	::EndPaint( msg.GetHWND(), &ps );

	return Window::CALLBACKCODE_DEFAULT;
}


}
