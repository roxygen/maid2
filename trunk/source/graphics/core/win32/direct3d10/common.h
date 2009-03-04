#ifndef graphic_core_driver_win32_direct3d10_common_h
#define graphic_core_driver_win32_direct3d10_common_h

/*!	
    @file
    @brief	このディレクトリの共通ヘッダ
*/


#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../../../auxiliary/macro.h"
#include"../../../../auxiliary/debug/assert.h"
#include"../../../../auxiliary/debug/warning.h"

#include"../../../../framework/win32/com_ptr.h"

#include"../../../pixelformat.h"
#include"../../idevice.h"
#include"../dxgitypedef.h"

#include<d3d10_1.h>
#include<string>

namespace Maid { namespace Graphics {

  typedef com_ptr<IUnknown>           SPD3D10UNKNOWN;
  typedef com_ptr<ID3D10Device>       SPD3D10DEVICE;

  typedef com_ptr<ID3D10Buffer>       SPD3D10BUFFER;
  typedef com_ptr<ID3D10Texture2D>    SPD3D10TEXTURE2D;
  typedef com_ptr<ID3D10DepthStencilView>    SPD3D10DEPTHSTENCILVIEW;
  typedef com_ptr<ID3D10RenderTargetView>    SPD3D10RENDERTARGETVIEW;
  typedef com_ptr<ID3D10ShaderResourceView>  SPD3D10SHADERRESOURCEVIEW;

  typedef com_ptr<ID3D10InputLayout >   SPD3D10INPUTLAYOUT;
  typedef com_ptr<ID3D10SamplerState>   SPD3D10SAMPLERSTATE;
  typedef com_ptr<ID3D10VertexShader>   SPD3D10VERTEXSHADER;
  typedef com_ptr<ID3D10PixelShader>    SPD3D10PIXELSHADER;
  typedef com_ptr<ID3D10BlendState>     SPD3D10BLENDSTATE;
  typedef com_ptr<ID3D10DepthStencilState>  SPD3D10DEPTHSTENCILSTATE;
  typedef com_ptr<ID3D10RasterizerState>    SPD3D10RASTERIZERSTATE;



  /*!
      @fn D3DFORMAT PIXELFORMATtoD3DFORMAT( PIXELFORMAT eFormat )
   
      PIXELFORMAT を Direct3D10 で使用されている
  \n	ピクセルフォーマットに変換します
   
      @param	eFormat	[i ]	変換したいフォーマット
   
      @return	変換後のフォーマット
  \n		    	判別できなかったら D3DFMT_UNKNOWN
   */
  inline DXGI_FORMAT PIXELFORMATtoDXGIFORMAT( PIXELFORMAT eFormat )
  {
    switch( eFormat )
    {
    case PIXELFORMAT_A08R08G08B08I:		{ return DXGI_FORMAT_R8G8B8A8_UNORM;	}break;
    case PIXELFORMAT_A02R10G10B10I:		{ return DXGI_FORMAT_R10G10B10A2_UNORM;	}break;
    case PIXELFORMAT_A16B16G16R16I:		{ return DXGI_FORMAT_R16G16B16A16_UNORM;	}break;
    case PIXELFORMAT_A16B16G16R16F:		{ return DXGI_FORMAT_R16G16B16A16_FLOAT;	}break;
    case PIXELFORMAT_A32B32G32R32F:		{ return DXGI_FORMAT_R32G32B32A32_FLOAT;	}break;

    case PIXELFORMAT_DXT1:				{ return DXGI_FORMAT_BC1_UNORM;		}break;
    case PIXELFORMAT_DXT2:				{ return DXGI_FORMAT_BC2_UNORM;		}break;
    case PIXELFORMAT_DXT3:				{ return DXGI_FORMAT_BC3_UNORM;		}break;
    case PIXELFORMAT_DXT4:				{ return DXGI_FORMAT_BC4_UNORM;		}break;
    case PIXELFORMAT_DXT5:				{ return DXGI_FORMAT_BC5_UNORM;		}break;

    case PIXELFORMAT_D16I:     { return DXGI_FORMAT_D16_UNORM;	  }break;
    case PIXELFORMAT_D24IS8:   { return DXGI_FORMAT_D24_UNORM_S8_UINT;  }break;
    }

    MAID_ASSERT( true, "識別できないPIXELFORMATです " << eFormat );

    return DXGI_FORMAT_UNKNOWN;
  }

  /*!
    @fn PIXELFORMAT D3DFORMATtoPIXELFORMAT( D3DFORMAT eFormat )
   
    DirectGraphic9 で使用されているピクセルフォーマットを
    PIXELFORMAT に変換します
   
    @param	Format	[i ]	変換したい頂点フォーマット
   
    @return	変換後のフォーマット
  \n			判別できなかったら PIXELFORMAT_NONE
   */
  inline PIXELFORMAT DXGI_FORMATtoPIXELFORMAT( DXGI_FORMAT Format )
  {
    switch( Format )
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM:		{ return PIXELFORMAT_A08R08G08B08I;	}break;
    case DXGI_FORMAT_R10G10B10A2_UNORM:	{ return PIXELFORMAT_A02R10G10B10I;	}break;
    case DXGI_FORMAT_R16G16B16A16_UNORM:{ return PIXELFORMAT_A16B16G16R16I;	}break;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:{ return PIXELFORMAT_A16B16G16R16F;	}break;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:{ return PIXELFORMAT_A32B32G32R32F;	}break;

    case DXGI_FORMAT_BC1_UNORM:		    	{ return PIXELFORMAT_DXT1;	}break;
    case DXGI_FORMAT_BC2_UNORM:			    { return PIXELFORMAT_DXT2;	}break;
    case DXGI_FORMAT_BC3_UNORM:			    { return PIXELFORMAT_DXT3;	}break;
    case DXGI_FORMAT_BC4_UNORM:			    { return PIXELFORMAT_DXT4;	}break;
    case DXGI_FORMAT_BC5_UNORM:			    { return PIXELFORMAT_DXT5;	}break;

    case DXGI_FORMAT_D16_UNORM:			    { return PIXELFORMAT_D16I;	}break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:	{ return PIXELFORMAT_D24IS8;	}break;
    }

    return PIXELFORMAT_NONE;
  }


  inline D3D10_USAGE RESOURCEUSAGEtoD3D10_USAGE( RESOURCEUSAGE use )
  {
    D3D10_USAGE ret;
    switch( use )
    {
    case RESOURCEUSAGE_DEFAULT: { ret = D3D10_USAGE_DEFAULT; }break;
    case RESOURCEUSAGE_IMMUTABLE: { ret = D3D10_USAGE_IMMUTABLE; }break;
    case RESOURCEUSAGE_DYNAMIC: { ret = D3D10_USAGE_DYNAMIC; }break;
    case RESOURCEUSAGE_STAGING: { ret = D3D10_USAGE_STAGING; }break;
    case RESOURCEUSAGE_GPUWRITE: { ret = D3D10_USAGE_DEFAULT; }break;

    }
    return ret;
  }

  inline D3D10_RTV_DIMENSION RENDERTARGETDIMENSIONtoD3D10_RTV_DIMENSION( CREATERENDERTARGETPARAM::DIMENSION src )
  {
    D3D10_RTV_DIMENSION ret;
    switch( src )
    {
    case CREATERENDERTARGETPARAM::DIMENSION_BUFFER:           { ret = D3D10_RTV_DIMENSION_BUFFER; }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE1D:        { ret = D3D10_RTV_DIMENSION_TEXTURE1D; }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D10_RTV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D:        { ret = D3D10_RTV_DIMENSION_TEXTURE2D;        }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D10_RTV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D10_RTV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D10_RTV_DIMENSION_TEXTURE2DMSARRAY; }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE3D:        { ret = D3D10_RTV_DIMENSION_TEXTURE3D;        }break;
    }
    return ret;
  }

  inline D3D10_DSV_DIMENSION DEPTHSTENCILDIMENSIONtoD3D10_DSV_DIMENSION( CREATEDEPTHSTENCILPARAM::DIMENSION src )
  {
    D3D10_DSV_DIMENSION ret;
    switch( src )
    {
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE1D:        { ret = D3D10_DSV_DIMENSION_TEXTURE1D; }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D10_DSV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D:        { ret = D3D10_DSV_DIMENSION_TEXTURE2D;        }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D10_DSV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D10_DSV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D10_DSV_DIMENSION_TEXTURE2DMSARRAY; }break;
    }
    return ret;
  }

  inline D3D10_SRV_DIMENSION MATERIALDIMENSIONtoD3D10_SRV_DIMENSION( CREATEMATERIALPARAM::DIMENSION src )
  {
    D3D10_SRV_DIMENSION ret;
    switch( src )
    {
    case CREATEMATERIALPARAM::DIMENSION_BUFFER:           { ret = D3D10_SRV_DIMENSION_BUFFER; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE1D:        { ret = D3D10_SRV_DIMENSION_TEXTURE1D; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D10_SRV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2D:        { ret = D3D10_SRV_DIMENSION_TEXTURE2D;        }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D10_SRV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE3D:        { ret = D3D10_SRV_DIMENSION_TEXTURE3D;        }break;
    }
    return ret;
  }

  inline D3D10_SRV_DIMENSION1 MATERIALDIMENSIONtoD3D10_SRV_DIMENSION1( CREATEMATERIALPARAM::DIMENSION src )
  {
    D3D10_SRV_DIMENSION1 ret;
    switch( src )
    {
    case CREATEMATERIALPARAM::DIMENSION_BUFFER:           { ret = D3D10_1_SRV_DIMENSION_BUFFER; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE1D:        { ret = D3D10_1_SRV_DIMENSION_TEXTURE1D; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D10_1_SRV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2D:        { ret = D3D10_1_SRV_DIMENSION_TEXTURE2D;        }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D10_1_SRV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D10_1_SRV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D10_1_SRV_DIMENSION_TEXTURE2DMSARRAY; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE3D:        { ret = D3D10_1_SRV_DIMENSION_TEXTURE3D;        }break;
    }
    return ret;
  }

  inline D3D10_COMPARISON_FUNC CMPTYPEtoD3D10_COMPARISON_FUNC( COMPARISON Type )
  {
    D3D10_COMPARISON_FUNC ret = D3D10_COMPARISON_LESS;

    switch( Type )
    {
    case COMPARISON_NEVER:       { return D3D10_COMPARISON_NEVER;         }break;
    case COMPARISON_LESS:        { return D3D10_COMPARISON_LESS;          }break;
    case COMPARISON_EQUAL:       { return D3D10_COMPARISON_EQUAL;         }break;
    case COMPARISON_LESSEQUAL:   { return D3D10_COMPARISON_LESS_EQUAL;	  }break;
    case COMPARISON_GREATER:     { return D3D10_COMPARISON_GREATER;       }break;
    case COMPARISON_NOTEQUAL:    { return D3D10_COMPARISON_NOT_EQUAL;     }break;
    case COMPARISON_GREATEREQUAL:{ return D3D10_COMPARISON_GREATER_EQUAL; }break;
    case COMPARISON_ALWAYS:      { return D3D10_COMPARISON_ALWAYS;        }break;
    }

    return ret;
  }


  inline D3D10_FILL_MODE FILLtoD3D10_FILL_MODE( RASTERIZERSTATEPARAM::FILL Type )
  {
    D3D10_FILL_MODE ret = D3D10_FILL_SOLID;

    switch( Type )
    {
    case RASTERIZERSTATEPARAM::FILL_WIREFRAME: { ret = D3D10_FILL_WIREFRAME; }break;
    case RASTERIZERSTATEPARAM::FILL_SOLID:     { ret = D3D10_FILL_SOLID; }break;
    }

    return ret;
  }

  inline D3D10_CULL_MODE CULLINGtoD3D10_CULL_MODE( RASTERIZERSTATEPARAM::CULLING Type )
  {
    D3D10_CULL_MODE ret = D3D10_CULL_NONE;

    switch( Type )
    {
    case RASTERIZERSTATEPARAM::CULLING_NONE:  { ret = D3D10_CULL_NONE; }break;
    case RASTERIZERSTATEPARAM::CULLING_LEFT:  { ret = D3D10_CULL_FRONT;}break;
    case RASTERIZERSTATEPARAM::CULLING_RIGHT: { ret = D3D10_CULL_BACK; }break;
    }

    return ret;
  }


  inline D3D10_BLEND TARGETBLENDtoD3D10_BLEND( BLENDSTATEPARAM::TARGET::BLEND b )
  {
    D3D10_BLEND ret = D3D10_BLEND_ZERO;

    switch( b )
    {
    case BLENDSTATEPARAM::TARGET::BLEND_ZERO:          { ret = D3D10_BLEND_ZERO; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_ONE:           { ret = D3D10_BLEND_ONE; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_COLOR:     { ret = D3D10_BLEND_SRC_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_COLOR: { ret = D3D10_BLEND_INV_SRC_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA:     { ret = D3D10_BLEND_SRC_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_ALPHA: { ret = D3D10_BLEND_INV_SRC_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_DST_ALPHA:    { ret = D3D10_BLEND_DEST_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_DST_ALPHA:{ ret = D3D10_BLEND_INV_DEST_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_DST_COLOR:    { ret = D3D10_BLEND_DEST_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_DST_COLOR:{ ret = D3D10_BLEND_INV_DEST_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA_SAT: { ret = D3D10_BLEND_SRC_ALPHA_SAT; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_BLEND_FACTOR:  { ret = D3D10_BLEND_BLEND_FACTOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_BLEND_FACTOR:{ ret = D3D10_BLEND_INV_BLEND_FACTOR; }break;
    }
    return ret;
  }

  inline D3D10_BLEND_OP TARGETOPERATIONtoD3D10_BLEND_OP( BLENDSTATEPARAM::TARGET::OPERATION op )
  {
    D3D10_BLEND_OP ret = D3D10_BLEND_OP_ADD;

    switch( op )
    {
    case BLENDSTATEPARAM::TARGET::OPERATION_ADD:         { ret = D3D10_BLEND_OP_ADD; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_SUBTRACT:    { ret = D3D10_BLEND_OP_SUBTRACT; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_REV_SUBTRACT:{ ret = D3D10_BLEND_OP_REV_SUBTRACT; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_MIN:         { ret = D3D10_BLEND_OP_MIN ; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_MAX:         { ret = D3D10_BLEND_OP_MAX; }break;
    }

    return ret;
  }

  inline DXGI_FORMAT INPUT_ELEMENTTYPEtoDXGI_FORMAT( INPUT_ELEMENT::TYPE t )
  {
    DXGI_FORMAT ret;

    switch( t )
    {
    case INPUT_ELEMENT::TYPE_FLOAT1: { ret = DXGI_FORMAT_R32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT2: { ret = DXGI_FORMAT_R32G32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT3: { ret = DXGI_FORMAT_R32G32B32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT4: { ret = DXGI_FORMAT_R32G32B32A32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_COLOR:  { ret = DXGI_FORMAT_R8G8B8A8_UNORM; }break;
    }
    return ret;
  }



  inline BOOL booltoD3D10BOOL( bool b )
  {
    if( b ) { return TRUE; }
    return FALSE;
  }


  inline D3D10_TEXTURE_ADDRESS_MODE ADDRESStoD3D10_TEXTURE_ADDRESS_MODE( SAMPLERSTATEPARAM::ADDRESS type )
  {
    D3D10_TEXTURE_ADDRESS_MODE ret = D3D10_TEXTURE_ADDRESS_CLAMP;

    switch( type )
    {
    case SAMPLERSTATEPARAM::ADDRESS_WRAP:       { ret = D3D10_TEXTURE_ADDRESS_WRAP;     }break;
    case SAMPLERSTATEPARAM::ADDRESS_MIRROR:     { ret = D3D10_TEXTURE_ADDRESS_MIRROR;   }break;
    case SAMPLERSTATEPARAM::ADDRESS_CLAMP:      { ret = D3D10_TEXTURE_ADDRESS_CLAMP;    }break;
    case SAMPLERSTATEPARAM::ADDRESS_BORDER:     { ret = D3D10_TEXTURE_ADDRESS_BORDER;   }break;
    case SAMPLERSTATEPARAM::ADDRESS_MIRROR_ONCE:{ ret = D3D10_TEXTURE_ADDRESS_MIRROR_ONCE; }break;
    }

    return ret;
  }

  inline D3D10_FILTER FILTERtoD3D10_FILTER( SAMPLERSTATEPARAM::FILTER min, SAMPLERSTATEPARAM::FILTER mag, SAMPLERSTATEPARAM::FILTER mip  )
  {
    D3D10_FILTER ret = D3D10_FILTER_MIN_MAG_MIP_LINEAR;

    if(  min==SAMPLERSTATEPARAM::FILTER_ANISOTROPIC
      || mag==SAMPLERSTATEPARAM::FILTER_ANISOTROPIC
      || mip==SAMPLERSTATEPARAM::FILTER_ANISOTROPIC
      )
    {
      ret = D3D10_FILTER_ANISOTROPIC;
    }
    else
    {
      if( min==SAMPLERSTATEPARAM::FILTER_POINT )
      {
        if( mag==SAMPLERSTATEPARAM::FILTER_POINT )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D10_FILTER_MIN_MAG_MIP_POINT; }
          ef( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR; }
        }
        ef( mag==SAMPLERSTATEPARAM::FILTER_LINEAR )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D10_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT; }
          ef( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D10_FILTER_MIN_POINT_MAG_MIP_LINEAR; }
        }
      }
      ef( min==SAMPLERSTATEPARAM::FILTER_LINEAR )
      {
        if( mag==SAMPLERSTATEPARAM::FILTER_POINT )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D10_FILTER_MIN_LINEAR_MAG_MIP_POINT; }
          ef( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D10_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR; }
        }
        ef( mag==SAMPLERSTATEPARAM::FILTER_LINEAR )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT; }
          ef( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D10_FILTER_MIN_MAG_MIP_LINEAR; }
        }
      }
    }

    return ret;
  }

}}

#endif
