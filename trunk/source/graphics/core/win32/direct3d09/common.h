#ifndef graphic_core_driver_win32_direct3d09_common_h
#define graphic_core_driver_win32_direct3d09_common_h

/*!	
    @file
    @brief	このディレクトリの共通ヘッダ
*/


#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include<d3d9.h>

#include"../../../../auxiliary/macro.h"
#include"../../../../framework/win32/com_ptr.h"
#include"../../../../auxiliary/debug/assert.h"
#include"../../../../auxiliary/debug/warning.h"

#include"../../../customvertextemplate.h"
#include"../../../pixelformat.h"
#include"../../idevice.h"

#include<map>


namespace Maid { namespace Graphics {

  typedef com_ptr<IDirect3D9>         SPD3D09;
  typedef com_ptr<IDirect3DDevice9>   SPD3D09DEVICE;
  typedef com_ptr<IDirect3DSurface9>  SPD3D09SURFACE;
  typedef com_ptr<IDirect3DTexture9>  SPD3D09TEXTURE;
  typedef com_ptr<IDirect3DIndexBuffer9>        SPD3D09INDEXBUFFER;
  typedef com_ptr<IDirect3DVertexBuffer9>       SPD3D09VERTEXBUFFER;
  typedef com_ptr<IDirect3DVertexShader9>       SPD3D09VERTEXSHADER;
  typedef com_ptr<IDirect3DVertexDeclaration9>  SPD3D09VERTEXDECLARATION;
  typedef com_ptr<IDirect3DPixelShader9>        SPD3D09PIXELSHADER;
  typedef com_ptr<IDirect3DSwapChain9>          SPD3D09SWAPCHAIN;


  typedef std::map<hObject::INSTANCEID,SPOBJECTDESC> OBJECTDESCMAP;

  /*!
      @fn D3DFORMAT PIXELFORMATtoD3DFORMAT( PIXELFORMAT eFormat )
   
      PIXELFORMAT を Direct3D9 で使用されている
  \n	ピクセルフォーマットに変換します
   
      @param	eFormat	[i ]	変換したいフォーマット
   
      @return	変換後のフォーマット
  \n		    	判別できなかったら D3DFMT_UNKNOWN
   */
  inline D3DFORMAT PIXELFORMATtoD3DFORMAT( PIXELFORMAT eFormat )
  {
    switch( eFormat )
    {
    case PIXELFORMAT_P08A08R08G08B08I:	{ return D3DFMT_P8;			}break;
    case PIXELFORMAT_P08X08R08G08B08I:	{ return D3DFMT_P8;			}break;

    case PIXELFORMAT_R05G06B05I:		{ return D3DFMT_R5G6B5;		}break;
    case PIXELFORMAT_A04R04G04B04I:	{ return D3DFMT_A4R4G4B4;	}break;
    case PIXELFORMAT_X01R05G05B05I:	{ return D3DFMT_X1R5G5B5;	}break;
    case PIXELFORMAT_A01R05G05B05I:	{ return D3DFMT_A1R5G5B5;	}break;

    case PIXELFORMAT_R08G08B08I:	  	{ return D3DFMT_R8G8B8;		}break;

    case PIXELFORMAT_A08R08G08B08I:		{ return D3DFMT_A8R8G8B8;	}break;
    case PIXELFORMAT_X08R08G08B08I:		{ return D3DFMT_X8R8G8B8;	}break;

    case PIXELFORMAT_A02R10G10B10I:		{ return D3DFMT_A2R10G10B10;	}break;
    case PIXELFORMAT_A02B10G10R10I:		{ return D3DFMT_A2B10G10R10;	}break;
    case PIXELFORMAT_A16B16G16R16I:		{ return D3DFMT_A16B16G16R16;	}break;
    case PIXELFORMAT_A16B16G16R16F:		{ return D3DFMT_A16B16G16R16F;	}break;
    case PIXELFORMAT_A32B32G32R32F:		{ return D3DFMT_A32B32G32R32F;	}break;

    case PIXELFORMAT_DXT1:				{ return D3DFMT_DXT1;		}break;
    case PIXELFORMAT_DXT2:				{ return D3DFMT_DXT2;		}break;
    case PIXELFORMAT_DXT3:				{ return D3DFMT_DXT3;		}break;
    case PIXELFORMAT_DXT4:				{ return D3DFMT_DXT4;		}break;
    case PIXELFORMAT_DXT5:				{ return D3DFMT_DXT5;		}break;
    }

    MAID_ASSERT( true, "識別できないPIXELFORMATです %d" << eFormat );

    return D3DFMT_UNKNOWN;
  }

  inline D3DFORMAT DEPTHSTENCILFORMATtoD3DFORMAT( DEPTHSTENCILFORMAT Format )
  {
    switch( Format )
    {
    case DEPTHSTENCILFORMAT_D15IS1:	  { return D3DFMT_D15S1;  }break;
    case DEPTHSTENCILFORMAT_D16I:     { return D3DFMT_D16;	  }break;
    case DEPTHSTENCILFORMAT_D32I:     { return D3DFMT_D32;	  }break;
    case DEPTHSTENCILFORMAT_D24IX8:   { return D3DFMT_D24X8;  }break;
    case DEPTHSTENCILFORMAT_D24IS8:   { return D3DFMT_D24S8;  }break;
    case DEPTHSTENCILFORMAT_D24FS8:   { return D3DFMT_D24FS8;	}break;
    case DEPTHSTENCILFORMAT_D24IX4S4:	{ return D3DFMT_D24X4S4;}break;
    }
    MAID_ASSERT( true, "識別できないPIXELFORMATです %d" << Format );
    return D3DFMT_UNKNOWN;
  }


  /*!
    @fn PIXELFORMAT D3DFORMATtoPIXELFORMAT( D3DFORMAT eFormat )
   
    DirectGraphic9 で使用されているピクセルフォーマットを
    PIXELFORMAT に変換します
   
    @param	Format	[i ]	変換したい頂点フォーマット
   
    @return	変換後のフォーマット
  \n			判別できなかったら PIXELFORMAT_NONE
   */
  inline PIXELFORMAT D3DFORMATtoPIXELFORMAT( D3DFORMAT Format )
  {
    switch( Format )
    {
    case D3DFMT_P8:			{ return PIXELFORMAT_P08X08R08G08B08I;	}break;

    case D3DFMT_R5G6B5:   { return PIXELFORMAT_R05G06B05I;		}break;
    case D3DFMT_A4R4G4B4: { return PIXELFORMAT_A04R04G04B04I;		}break;
    case D3DFMT_X1R5G5B5: { return PIXELFORMAT_X01R05G05B05I;		}break;
    case D3DFMT_A1R5G5B5: { return PIXELFORMAT_A01R05G05B05I;		}break;

    case D3DFMT_R8G8B8:   { return PIXELFORMAT_R08G08B08I;		}break;

    case D3DFMT_A8R8G8B8:     { return PIXELFORMAT_A08R08G08B08I;		}break;
    case D3DFMT_X8R8G8B8:     { return PIXELFORMAT_X08R08G08B08I;		}break;
    case D3DFMT_A2R10G10B10:	{ return PIXELFORMAT_A02R10G10B10I;		}break;
    case D3DFMT_A2B10G10R10:	{ return PIXELFORMAT_A02B10G10R10I;		}break;
    case D3DFMT_A16B16G16R16:	{ return PIXELFORMAT_A16B16G16R16I;		}break;

    case D3DFMT_A16B16G16R16F:	{ return PIXELFORMAT_A16B16G16R16F;		}break;

    case D3DFMT_A32B32G32R32F:	{ return PIXELFORMAT_A32B32G32R32F;		}break;

    case D3DFMT_DXT1:		{ return PIXELFORMAT_DXT1;			}break;
    case D3DFMT_DXT2:		{ return PIXELFORMAT_DXT2;			}break;
    case D3DFMT_DXT3:		{ return PIXELFORMAT_DXT3;			}break;
    case D3DFMT_DXT4:		{ return PIXELFORMAT_DXT4;			}break;
    case D3DFMT_DXT5:		{ return PIXELFORMAT_DXT5;			}break;
    }

    MAID_ASSERT( true, "識別できないD3DFORMATです %d" << Format );

    return PIXELFORMAT_NONE;
  }
  inline DEPTHSTENCILFORMAT D3DFORMATtoDEPTHSTENCILFORMAT( D3DFORMAT Format )
  {
    switch( Format )
    {
    case D3DFMT_D15S1:	 { return DEPTHSTENCILFORMAT_D15IS1;  }break;
    case D3DFMT_D16:     { return DEPTHSTENCILFORMAT_D16I;	  }break;
    case D3DFMT_D32:     { return DEPTHSTENCILFORMAT_D32I;	  }break;
    case D3DFMT_D24X8:   { return DEPTHSTENCILFORMAT_D24IX8;  }break;
    case D3DFMT_D24S8:   { return DEPTHSTENCILFORMAT_D24IS8;  }break;
    case D3DFMT_D24FS8:  { return DEPTHSTENCILFORMAT_D24FS8;	}break;
    case D3DFMT_D24X4S4: { return DEPTHSTENCILFORMAT_D24IX4S4;}break;
    }
    MAID_ASSERT( true, "識別できないD3DFORMATです %d" << Format );
    return DEPTHSTENCILFORMAT_NONE;
  }


  inline D3DCMPFUNC CMPTYPEtoD3DCMPFUNC( COMPARISON Type )
  {
    switch( Type )
    {
    case COMPARISON_NEVER:       { return D3DCMP_NEVER;      }break;
    case COMPARISON_LESS:        { return D3DCMP_LESS;       }break;
    case COMPARISON_EQUAL:       { return D3DCMP_EQUAL;      }break;
    case COMPARISON_LESSEQUAL:   { return D3DCMP_LESSEQUAL;	}break;
    case COMPARISON_GREATER:     { return D3DCMP_GREATER;    }break;
    case COMPARISON_NOTEQUAL:    { return D3DCMP_NOTEQUAL;   }break;
    case COMPARISON_GREATEREQUAL:{ return D3DCMP_GREATEREQUAL;}break;
    case COMPARISON_ALWAYS:      { return D3DCMP_ALWAYS;     }break;
    }

    MAID_ASSERT( true, "識別できないCMPTYPE " << Type );

    return D3DCMP_FORCE_DWORD;
  }


  inline D3DMULTISAMPLE_TYPE SAMPLETYPEtoD3DMULTISAMPLE_TYPE( int Type )
  {
    switch( Type )
    {
    case  0:
    case  1:  { return D3DMULTISAMPLE_NONE;       }break;
    case  2:  { return D3DMULTISAMPLE_2_SAMPLES;  }break;
    case  3:  { return D3DMULTISAMPLE_3_SAMPLES;  }break;
    case  4:  { return D3DMULTISAMPLE_4_SAMPLES;	}break;
    case  5:  { return D3DMULTISAMPLE_5_SAMPLES;  }break;
    case  6:  { return D3DMULTISAMPLE_6_SAMPLES;  }break;
    case  7:  { return D3DMULTISAMPLE_7_SAMPLES;  }break;
    case  8:  { return D3DMULTISAMPLE_8_SAMPLES;  }break;
    case  9:  { return D3DMULTISAMPLE_9_SAMPLES;  }break;
    case 10:  { return D3DMULTISAMPLE_10_SAMPLES;  }break;
    case 11:  { return D3DMULTISAMPLE_11_SAMPLES;	}break;
    case 12:  { return D3DMULTISAMPLE_12_SAMPLES;  }break;
    case 13:  { return D3DMULTISAMPLE_13_SAMPLES;  }break;
    case 14:  { return D3DMULTISAMPLE_14_SAMPLES;  }break;
    case 15:  { return D3DMULTISAMPLE_15_SAMPLES;  }break;
    case 16:  { return D3DMULTISAMPLE_16_SAMPLES;  }break;
    }

    MAID_ASSERT( true, "識別できないSAMPLE_TYPE " << Type );

    return D3DMULTISAMPLE_NONE;
  }
}}

#endif
