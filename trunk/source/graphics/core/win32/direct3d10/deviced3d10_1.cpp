#include"deviced3d10_1.h"


namespace Maid { namespace Graphics {

DeviceD3D10_1::DeviceD3D10_1( const DllWrapper& dll, const SPDXGIFACTORY& pFactory, const SPDXGIADAPTER& pAdapter, Window& Windw )
  :DeviceD3D10_0( dll, pFactory, pAdapter, Windw )
{

}

SPD3D10DEVICE DeviceD3D10_1::CreateDevice( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,D3D10_FEATURE_LEVEL1,UINT,ID3D10Device1**);
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDevice1"));

  if( createdevice==NULL ) { MAID_WARNING("load失敗"); return SPD3D10DEVICE(); }

  ID3D10Device1* pDev = NULL;

  static const D3D10_FEATURE_LEVEL1 levelAttempts[] = 
  { 
      D3D10_FEATURE_LEVEL_10_1, 
      D3D10_FEATURE_LEVEL_10_0, 
//      D3D10_FEATURE_LEVEL_9_3, 
//      D3D10_FEATURE_LEVEL_9_2, 
//      D3D10_FEATURE_LEVEL_9_1, 
  };

  for ( int i=0; i<NUMELEMENTS(levelAttempts); ++i ) 
  { 
    const HRESULT ret = createdevice( 
        pAdapter.get(), 
        D3D10_DRIVER_TYPE_HARDWARE, 
        NULL, 
        0, 
        levelAttempts[i], 
        D3D10_1_SDK_VERSION, 
        &pDev 
        );

    if( SUCCEEDED(ret) ) { break; }
  }

  if( pDev==NULL ) 
  {
    MAID_WARNING("D3D10CreateDevice1()");
    return SPD3D10DEVICE(); 
  }

  return SPD3D10DEVICE(pDev);
}

SPMATERIAL     DeviceD3D10_1::CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM& param )
{

  return SPMATERIAL();
}

SPBLENDSTATE   DeviceD3D10_1::CreateBlendState( const BLENDSTATEPARAM& Option )
{

  return SPBLENDSTATE();
}


}}


