#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"

#include"../../source/graphics/texture2d.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
    //  ここでは引数で渡されたデバイスリストをユーザーに選択させる処理を書く
    //  ユーザーの選択結果は conf に設定するんだけど、設定しなくても自動的になんとかしてくれる

    //  ここでアプリケーションを終了したいときは return false です。
    return true;
  }

  void LocalInitialize( const BEGINPARAM& param )
  {
    //  LocalSelectDevice() で指定した設定で、各種デバイスデータが param に入ってます
    //  なので、もらったデータを元に、初期化する

    m_Render.Initialize( param.ScreenSize, param.DrawCommand[0], param.pFontDevice );

    m_pFontS = param.pFontDevice->CreateDefaultFont( SIZE2DI(8,16) );
    m_pFontM = param.pFontDevice->CreateDefaultFont( SIZE2DI(16,32) );
    m_pFontL = param.pFontDevice->CreateDefaultFont( SIZE2DI(32,64) );

    m_FrameCount = 0;
    m_DrawCount  = 0;
  }

  void LocalUpdateFrame()
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ
    if( m_Render.IsInitializing() ) { return ; }

    ++m_FrameCount;
  }

  void LocalUpdateDraw()
  {
    //  定期的に描画するとこ

    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Render.Begin();

    char buf[256];
    sprintf( buf, "frame:%04d,draw:%04d", m_FrameCount, m_DrawCount );

    m_Render.BltText( POINT2DI(0,0), m_pFontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );
    m_Render.BltText( POINT2DI(0,100), m_pFontS, MAIDTEXT("テキスト表示"), COLOR_R32G32B32A32F(1,1,1,1) );
    m_Render.BltText( POINT2DI(0,200), m_pFontM, MAIDTEXT("色かえもＯＫ"), COLOR_R32G32B32A32F(1,0,1,1) );
    m_Render.BltText( POINT2DI(0,300), m_pFontM, MAIDTEXT("半透明もできる"), COLOR_R32G32B32A32F(1,0,1,0.5f) );
    m_Render.BltText( POINT2DI(0,400), m_pFontL, MAIDTEXT("改行も\nサポート"), COLOR_R32G32B32A32F(0,1,1,1) );

    m_Render.End();
    ++m_DrawCount;
  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  SPFONT          m_pFontS;
  SPFONT          m_pFontM;
  SPFONT          m_pFontL;

  int   m_FrameCount;
  int   m_DrawCount;
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new MyApp) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

