#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/vertexshader.h"
#include"../../source/graphics/vertex.h"
#include"../../source/graphics/inputlayout.h"
#include"../../source/graphics/customvertextemplate.h"
#include"../../source/graphics/rasterizerstate.h"
#include"../../source/graphics/blendstate.h"

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

    m_Render.Initialize( param.DrawCommand[0] );

  }

  void LocalUpdateFrame( float now, float delta )
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

  }

  void LocalUpdateDraw( const IRenderTarget& target,float now, float delta )
  {
    //  定期的に描画するとこ
    //  描画先は target 

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsLoading() ) { return ; }
    m_Render.SetVirtualScreenSize( SIZE2DI(800,600) );

    m_Render.Begin();
    m_Render.SetDefaultSetting();

    m_Render.SetBlendState( GraphicsRender::BLENDSTATE_ALPHA );

    //  普通に青い四角を塗る
    m_Render.Fill( POINT2DI(600,300), SIZE2DI(100,50), POINT2DI(50,25), Maid::COLOR_R32G32B32A32F(0,0,1,1) );

    //  赤い四角を横に伸ばして塗る
    m_Render.FillS( POINT2DI(200,300), SIZE2DI(100,50), POINT2DI(50,25), SIZE2DF(1.5f,1.0f), Maid::COLOR_R32G32B32A32F(1,0,0,1) );

    {
      //  緑の四角を回転させて塗る(加算合成)
      m_Render.SetBlendState( GraphicsRender::BLENDSTATE_ADD );

      const POINT2DI pos(400,300);
      const SIZE2DI  size(200,200);
      const POINT2DI center(size.w/2,size.h/2);

      static int rot;
      rot++;
      rot %= 360;
      m_Render.FillR( pos, size, center, DEGtoRAD(rot), Maid::VECTOR3DF(0,0,1), Maid::COLOR_R32G32B32A32F(0,1,0,1) );

      //  青の四角を回転させて塗る(加算合成)

      m_Render.FillSR( pos, size, center, SIZE2DF(2,0.3f), DEGtoRAD(-rot), Maid::VECTOR3DF(0,0,1), Maid::COLOR_R32G32B32A32F(0,0,1,1) );
    }
    m_Render.End();
  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
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

