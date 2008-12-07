/*!
    @file
    @brief ウィンドウクラス
 */


#ifndef framework_win32_window_h
#define framework_win32_window_h

#include"../../config/define.h"
#include"../../config/win32.h"

#include<map>
#include<boost/function.hpp>
#include<boost/bind.hpp>

#include"../../auxiliary/string.h"
#include"../../auxiliary/mathematics/point.h"
#include"../../auxiliary/mathematics/size.h"
#include"../../auxiliary/globalpointer.h"

#include"messagehook.h"
#include"osdevice.h"

namespace Maid
{ 
  class Application;
  class Window : public MessageHook , public GlobalPointer<OSDevice>
	{
	public:
		enum CALLBACKCODE
		{
			CALLBACKCODE_RETURN=0,   //!<	メッセージは処理されたのでこれ以降は呼ばない
			CALLBACKCODE_DEFAULT,    //!<	メッセージをOSに戻す
		};
		typedef boost::function1<CALLBACKCODE, WindowsMessage& >	MESSAGEFUNCTION;

	public:
		Window();
		virtual ~Window();

		void RegistCallBack( UINT Msg, const MESSAGEFUNCTION& Function );
		void DeleteCallBack( UINT Msg );

		void Show();
		void Hide();

		void SetPosition( const POINT2DI& pos );
		void SetSize( const SIZE2DI& pos );
		void SetStyle( DWORD Style );
		void SetDragDropMode( bool IsAccept );
		void SetCaption( const String& Name );


		HWND    GetHWND()const;
		SIZE2DI GetSize()const;
		DWORD   GetStyle()const;


		void Create();
		void Destory();
    bool IsCreated()const;

		virtual MessageHook::RETURNCODE OnMessage( WindowsMessage& Param );

	private:
		String    m_Caption;  //!<	ウィンドウキャプション
		POINT2DI  m_Pos;      //!<	ウィンドウの現在座標
		SIZE2DI   m_Size;     //!<	クライアント領域の大きさ
    DWORD     m_Style;    //!<  ウィンドウスタイル

		HWND	m_hWnd;     //!< m_hWnd==NULL まだウィンドウを作ってない

		bool	m_IsDandD;  //!<	Drag&Drop を許可するか？
		bool	m_IsShow;   //!<	表示中か？
		std::map<UINT,MESSAGEFUNCTION>	m_MessageMap;	//!<	メッセージマップ
	};

	//!	省略的な書き方
	#define MakeCallBackObject(pFunc,pObject) boost::bind( pFunc, pObject, _1 )
}

#endif
