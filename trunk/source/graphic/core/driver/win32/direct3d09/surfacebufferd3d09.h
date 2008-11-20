#ifndef graphic_core_driver_win32_direct3d09_surfacebufferd3d09_h
#define graphic_core_driver_win32_direct3d09_surfacebufferd3d09_h

#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include"../../surfacebuffer.h"

#include"d3d09include.h"

namespace Maid
{
  /*!
      @class	SurfaceBufferD3D09 surfacebufferd3d09.h
      @brief	サーフェスバッファ D3D9 Ver.
   */
  class SurfaceBufferD3D09 : public SurfaceBuffer
  {
  public:
    SurfaceBufferD3D09();
	  SurfaceBufferD3D09( IDirect3DSurface9* pCom );

	  virtual SIZE2DI GetSize() const;
	  virtual PIXELFORMAT GetPixelFormat() const;

    virtual void Lock( SurfaceBufferLockInfo& info );
	  virtual void Unlock();

	  SPD3D09SURFACEBUFFER	pSurface;

	  D3DSURFACE_DESC Desc;
  };
}


#endif