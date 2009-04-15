#ifndef sample_0190graphicsconfigtest_app_h
#define sample_0190graphicsconfigtest_app_h
/*
  ゲーム用アプリケーションの雛形サンプル
*/


//#include"../../source/framework/application.h"
//#include"../../source/framework/gameframework.h"
#include"../../source/framework/igamethread.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/framework/gui.h"
#include"../../source/graphics/font.h"

#include"configwindow.h"
#include"textbutton.h"

class MyApp : public Maid::IGameThread
{
protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );
  void Initialize();
  void UpdateFrame();
  void UpdateDraw();
  void Finalize();

  void ReciveMessage( Maid::IGUIParts::ID id, const Maid::IGUIParam& param );

private:
  Maid::Graphics2DRender  m_Render;

  Maid::Font          m_FontM;
  int m_FrameCount;
  int m_DrawCount;
  float m_FPS;


  TextButton    m_ReloadButton;
  ConfigWindow  m_Window;

  Maid::GUIManager      m_Manager;

};

#endif

