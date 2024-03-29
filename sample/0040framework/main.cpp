﻿/*
  普通のアプリケーションの雛形サンプル
*/

#include"../../source/framework/application.h"



using namespace Maid;


class MyApp : public Application
{
protected:
  virtual void Initialize()
  {
		//	とりあえずウィンドウの作成
		//	ウィンドウのｘボタンを押したらアプリが終了するようにする
		m_Window.RegistCallBack( WM_CLOSE, MakeCallBackObject(&MyApp::OnClose,this) );
    m_Window.Create( MAIDTEXT("window"), POINT2DI(0,0), SIZE2DI(400,300), WS_OVERLAPPEDWINDOW );
		m_Window.Show();
  }

  virtual void OnLoop()
  {
  
  }
  virtual void Finalize(){}

private:
	Window::CALLBACKCODE OnClose( WindowsMessage& msg )
	{
		OnExit(0);
		return Window::CALLBACKCODE_DEFAULT;
	}

private:
  Window  m_Window;
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  MyApp app;
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

