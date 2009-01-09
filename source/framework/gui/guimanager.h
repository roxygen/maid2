#ifndef maid2_framework_gui_guimanager_h
#define maid2_framework_gui_guimanager_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../input/mouse.h"
#include"../../input/keybord.h"
#include"../../auxiliary/mathematics.h"
#include"iguiparts.h"
#include"guimessage.h"
#include<map>

namespace Maid
{
  class GUIManager
  {
  public:
    GUIManager();
    ~GUIManager();
    void Initialize( const Mouse* pMouse, const Keybord* pKey /*const FEP* pFep */);
    void Finalize();

    void UpdateFrame();
    void UpdateDraw( RenderTargetBase& Target );

    void SetEnable( bool IsEnable );
    void SetVisible( bool IsVisible );

    void RegisterParts( const SPGUIPARTS& pParts );
    void DeleteParts( const SPGUIPARTS& pParts );
    bool IsExitstParts( const SPGUIPARTS& pParts )const;
    SPGUIPARTS FindParts( IGUIParts::ID id )const; 

  private:
    const Mouse*    m_pMouse;
    const Keybord*  m_pKeybord;

    bool      m_IsEnable;
    bool      m_IsVisible;

    typedef std::map<IGUIParts::ID,SPGUIPARTS> PARTSLIST;
    PARTSLIST  m_PartsList;
  };
}




#endif
