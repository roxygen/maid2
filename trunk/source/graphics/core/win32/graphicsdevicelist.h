/*!
	@file
	@brief Win32用 VideoCard列挙ヘッダ
 */

#ifndef graphic_core_win32_graphicsdevicelist_h
#define graphic_core_win32_graphicsdevicelist_h


#include"../../../config/define.h"
#include"../../../config/win32.h"

#include<d3d9.h>

#include"../igraphicsdevicelist.h"

#include"../../../framework/win32/dllwrapper.h"
#include"../../../framework/win32/com_ptr.h"
#include"../../../framework/win32/window.h"

#include"direct3d09/d3d09include.h"

namespace Maid
{
  class GraphicsDeviceList : public IGraphicsDeviceList
  {
  public:
    GraphicsDeviceList( Window& window );
    void Initialize();

    void GetList( std::vector<INFO>& info );
    SPGRAPHICSDEVICE Create( int DeviceID, const CREATEPARAM& Info  );

  private:
    Window&  m_Window;

  private:
    DllWrapper   m_D3D9DLL;
    SPD3D09      m_pDirect3D09;
  };
}



#endif
