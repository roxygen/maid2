#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/font.h"
#include"../../source/input/mouse.h"
#include"../../source/input/keybord.h"
#include"../../source/sound/soundfile.h"

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

    m_SoundSE.LoadFile( MAIDTEXT("tm2_bird001.wav") );
    m_SoundBGM.LoadFile( MAIDTEXT("waru.wav") );
  }

  void UpdateFrame()
  {
    if( m_Render.IsInitializing() ) { return ; }
    if( m_SoundSE.IsLoading() ) { return ; }
    if( m_SoundBGM.IsLoading() ) { return ; }

    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();


    if( k.IsIn('A') )
    {
      m_SoundBGM.Play();
    }
    if( k.IsIn('B') )
    {
      m_SoundBGM.Stop();
    }
    if( k.IsIn('C') )
    {
      m_SoundSE.Play();
    }
    if( k.IsIn('D') )
    {
      m_SoundSE.Stop();
    }
  }

  void UpdateDraw()
  {
    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  Font            m_FontS;
  SoundFile       m_SoundBGM;
  SoundFile       m_SoundSE;
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

