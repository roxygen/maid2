/*
  キー入力サンプル
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/font.h"
#include"../../source/input/mouse.h"
#include"../../source/input/keybord.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    return true;
  }

  void Initialize()
  {
    m_Render.Initialize();

    m_FontS.Create( SIZE2DI(16,32), true );
  }

  void UpdateFrame()
  {
    if( m_Render.IsInitializing() ) { return ; }

  }

  void UpdateDraw()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Render.Begin();

    char buf[256];
    sprintf( buf, "MouseX:%04d,MouseY:%04d", m.GetX(), m.GetY() );
    m_Render.BltText( POINT2DI(0,0), m_FontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );

    sprintf( buf, "DeltaX:%04d,DeltaY:%04d", m.GetDeltaX(), m.GetDeltaY() );
    m_Render.BltText( POINT2DI(0,50), m_FontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );

    if( m.IsDownL() )
    {
      m_Render.BltText( POINT2DI(0,100), m_FontS, MAIDTEXT("左が押されています"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    if( m.IsInL() )
    {
      m_Render.BltText( POINT2DI(0,150), m_FontS, MAIDTEXT("左が押されました"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    if( k.IsDown('A') )
    {
      m_Render.BltText( POINT2DI(0,200), m_FontS, MAIDTEXT("Aが押されています"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    if( k.IsIn('B') )
    {
      m_Render.BltText( POINT2DI(0,250), m_FontS, MAIDTEXT("Bが押されました"), COLOR_R32G32B32A32F(1,1,1,1) );
    }

    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  Graphics2DRender  m_Render;
  Font            m_FontS;
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

