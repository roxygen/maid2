/*!
	  @file
	  @brief windows用フォント管理クラス
 */

#ifndef maid2_graphics_core_win32_dxgitypedef_h
#define maid2_graphics_core_win32_dxgitypedef_h

#include"../../../config/define.h"
#include<dxgi.h>

namespace Maid { namespace Graphics {

  typedef com_ptr<IDXGIFactory>       SPDXGIFACTORY;
  typedef com_ptr<IDXGIAdapter>       SPDXGIADAPTER;
  typedef com_ptr<IDXGISwapChain>     SPDXGISWAPCHAIN;


}}


#endif