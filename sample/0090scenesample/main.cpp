#include"../../source/framework/gameframework.h"

#include"app.h"


using namespace Maid;

int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new App) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

