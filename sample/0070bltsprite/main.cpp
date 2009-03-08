/*
  画像を表示してみる
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/texture2d.h"

#include"startupdialog.h"


using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
    return ret==GAMESTART;
  }


  void Initialize()
  {
    m_Render.Initialize();

    m_Texture.LoadFile( MAIDTEXT("nc1673.bmp") );
    m_Texture2.LoadFile( MAIDTEXT("nc1429.png") );

  }

  void UpdateFrame()
  {
  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

    if(  m_Render.IsInitializing()
      || m_Texture.IsLoading()
      || m_Texture2.IsLoading())
    { return ; }

    const RECT2DI  rc( POINT2DI(0,0), m_Texture.GetImageSize() );
    const POINT2DI center(rc.w/2,rc.h/2);

    m_Render.Begin();

    //  矩形を普通に描画
		m_Render.Blt( POINT2DI(400,300), m_Texture, rc, center, 1.0f );

    //  横に伸ばして、縦に縮めて、不透明度５０％
		m_Render.BltS( POINT2DI(400,100), m_Texture, rc, center, 0.5f, SIZE2DF(1.5f,0.8f) );

    {

      static int rot;
      rot++;
      rot %= 360;
      //  (50,300)に回転させて描画(加算合成)
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ADD );
      m_Render.BltR( POINT2DI(50,300), m_Texture, rc, center, 1.0f, DEGtoRAD(rot), Maid::VECTOR3DF(0,0,1) );


      {
        const RECT2DI  rc( POINT2DI(0,0), m_Texture2.GetImageSize() );
        const POINT2DI center(rc.w/2,rc.h/2);
        //  (600,300)に大きさ50%,50%で回転させて描画(スクリーン合成)
        m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_SCREEN );
        m_Render.BltSR( POINT2DI(600,300), m_Texture2, rc, center, 1.0f, SIZE2DF(0.5f,0.5f), DEGtoRAD(-rot), Maid::VECTOR3DF(0,1,0) );
      }
    }
    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  Graphics2DRender  m_Render;
  Texture2D       m_Texture;
  Texture2D       m_Texture2;
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

