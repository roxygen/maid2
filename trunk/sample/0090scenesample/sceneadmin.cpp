#include"sceneadmin.h"
#include"scene/text/scenetext.h"

using namespace Maid;


SPSCENE SceneAdmin::CreateFirstScene()
{
  SPSCENEINPUT pInput;
  SPSCENE pScene( new SceneText );

  pScene->Initialize( pInput );

  return pScene;
}

SPSCENE SceneAdmin::CreateNextScene( const Maid::SPSCENEOUTPUT& pOut )
{

  return SPSCENE();
}

