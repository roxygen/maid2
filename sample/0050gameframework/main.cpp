#include"../../source/framework/application.h"
#include"../../source/framework/gameframework.h"



using namespace Maid;


class MyApp : public GameThread
{
protected:
    virtual void LocalOnSetup( APPCONFIG& conf )
    {

    }

    virtual void LocalOnBegin( const BEGINPARAM& param )
    {

    }

    virtual void LocalOnLoop()
    {

    }

    virtual void LocalOnEnd()
    {

    }
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

