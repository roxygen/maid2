/*
  3Dモデルの描画
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics3drender.h"

#include"../../source/graphics/modelmqo.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
     return true;
  }

  void Initialize()
  {
    m_Model.Load( MAIDTEXT("box.mqo") );
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();


  }

  void UpdateDraw()
  {
  }

  void Finalize()
  {

  }

private:
  ModelMQO        m_Model;
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

