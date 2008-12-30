#include"sceneadmin.h"
#include"scene/text/scenetext.h"
#include"scene/image/sceneimage.h"

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
  //  こんなとこにstatic あるけど、本当は App にゲームルーチンなクラスを持たせて
  //  そいつが行うべき

  static int count;

  ++count;

  SPSCENEINPUT pInput;
  SPSCENE pScene;

  switch( count%2 )
  {
  case 0: { pScene.reset( new SceneText ); }break;
  case 1: { pScene.reset( new SceneImage ); }break;
  }

  pScene->Initialize( pInput );

  return pScene;
}

