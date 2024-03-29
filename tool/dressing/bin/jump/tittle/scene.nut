import("logo.nut")
import("keyhelp.nut")

class SceneTittle extends IScene
{
  _SceneEnd = false

  function Initialize()
  {
    IScene.Initialize();
    {
      local tmp = Logo();
      GetDrawObjectManager().InsertDrawObject( tmp );
    }
    {
      local tmp = KeyHelp();
      GetDrawObjectManager().InsertDrawObject( tmp );
    }

    _SceneEnd = false
  }

  function Finalize()
  {
    IScene.Finalize()
  }

  function IsExit()
  {
    return _SceneEnd
  }

  function UpdateFrame()
  {
    IScene.UpdateFrame();
    
    if( GetInput().IsIn(KEYBOARD.ENTER) ) { _SceneEnd = true }
    
  }

}


