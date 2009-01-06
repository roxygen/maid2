#include"../../source/framework/application.h"
#include"../../source/framework/gameframework.h"
#include"../../source/framework/igamethread.h"
#include"../../source/graphics/graphicsrender.h"

using namespace Maid;


class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
    //  ここでは引数で渡されたデバイスリストをユーザーに選択させる処理を書く
    //  ユーザーの選択結果は conf に設定するんだけど、設定しなくても自動的になんとかしてくれる

    //  ここでアプリケーションを終了したいときは return false です。
    return true;
  }

  void Initialize()
  {
    //  ここまでくると、SelectDevice() で指定した設定で、各種初期化されています
    //  自分のメンバを初期化する

    m_Render.Initialize();

  }

  void UpdateFrame()
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,1,1,0) );

  }

  void Finalize()
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

