﻿/*
  矩形塗りつぶしのサンプル
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/graphics3drender.h"

#include"startupdialog.h"

using namespace Maid;

class MyApp : public IGameThread
{
public:
  MyApp()
    :m_Render(m_Command)
  {

  }

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
    return ret==GAMESTART;
  }

  void Initialize()
  {
    m_Command.Initialize();
    m_Render.Initialize();
    m_Texture.LoadFile( MAIDTEXT("nc1673.bmp") );

  }

  void UpdateFrame()
  {

  }

  void UpdateDraw()
  {
    m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if(  m_Render.IsInitializing()
      || m_Texture.IsLoading()
      )
    { return ; }

    const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
    const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

    m_Command.SetRenderTarget( rt, ds );
    m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0.5f,0,0) );
    m_Command.ClearDepthStencil( 1.0f, 0x00 );


//*
    Camera camera;

    camera.Set2DPosition( 800, 600, 40.0f, 1000.0f );

    m_Command.Begin();
    m_Render.SetDepthWrite( false );
    m_Render.SetSpriteBlendState( Graphics3DRender::BLENDSTATE_ALPHA );


    m_Render.SetCamera( camera );
//    m_Render._Fill();
//    m_Render.SpriteFill( POINT3DF(400,300,0), COLOR_R32G32B32A32F(1,1,1,0.5f), SIZE2DI(200,100), POINT2DI(0,0) );
//    m_Render.SpriteFillS( POINT3DF(400,300,0), COLOR_R32G32B32A32F(1,1,1,0.5f), SIZE2DI(200,100), POINT2DI(0,0), SIZE2DF(2,1) );
//    m_Render.SpriteFillR( POINT3DF(400,300,0), COLOR_R32G32B32A32F(1,1,1,0.5f), SIZE2DI(200,100), POINT2DI(0,0), DEGtoRAD(45), VECTOR3DF(0,1,0) );
//    m_Render.SpriteFillSR( POINT3DF(400,300,0), COLOR_R32G32B32A32F(1,1,1,0.5f), SIZE2DI(200,100), POINT2DI(0,0), SIZE2DF(2,1), DEGtoRAD(45), VECTOR3DF(0,1,0) );

    {
      const SIZE2DI& size = m_Texture.GetImageSize();

//      m_Render.SpriteBlt( POINT3DF(400,300,0), m_Texture, RECT2DI(0,0,size.w,size.h), POINT2DI(0,0), 0.5f );
//      m_Render.SpriteBltS( POINT3DF(400,300,0), m_Texture, RECT2DI(0,0,size.w,size.h), POINT2DI(0,0), 0.5f, SIZE2DF(2,1) );
//      m_Render.SpriteBltR( POINT3DF(400,300,0), m_Texture, RECT2DI(0,0,size.w,size.h), POINT2DI(0,0), 0.5f, DEGtoRAD(45), VECTOR3DF(0,1,0) );
//      m_Render.SpriteBltSR( POINT3DF(400,300,0), m_Texture, RECT2DI(0,0,size.w,size.h), POINT2DI(0,0), 0.5f, SIZE2DF(2,1), DEGtoRAD(45), VECTOR3DF(0,1,0) );
    }
    {
      const RECT2DI BeginRect(POINT2DI(0,0),m_Texture.GetImageSize());
      const RECT2DI EndRect(0,0,BeginRect.w/2,BeginRect.h/2);

      m_Render.SpriteMixBlt( POINT3DF(400,300,0), m_Texture, BeginRect, POINT2DI(0,0), 0.5f,  m_Texture, EndRect, 0.5f);

    }

    m_Command.End();
//*/

  }

  void LocalFinalize()
  {

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics3DRender  m_Render;
  Texture2D       m_Texture;
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

