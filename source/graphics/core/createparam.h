/*!
    @file
    @brief デバイスから作成する各種オブジェクトの初期値を定義する
 */

#ifndef maid2_graphics_core_createparam_h
#define maid2_graphics_core_createparam_h


#include"../../config/define.h"
#include"../pixelformat.h"

#include<vector>

namespace Maid { namespace Graphics {

  struct DISPLAYMODE
  {
    SIZE2DI     Size;         //!<  画面解像度
    PIXELFORMAT Format;       //!<  スクリーンモード
    int         RefreshRate;  //!<  リフレッシュレート
  };

  struct SCREENMODE
  {
    DISPLAYMODE DisplayMode;  //!<  解像度
    bool        IsFullScreen; //!<  フルスクリーンにするか？
    bool        IsWaitVSync;  //!<  VSync同期するか？
  };

  struct ENABLEFORMAT
  {
    std::vector<PIXELFORMAT>  Texture;              //!<  作成可能なテクスチャフォーマット
    std::vector<PIXELFORMAT>  RenderTargetTexture;  //!<  作成可能なレンダーテクスチャフォーマット
    std::vector<PIXELFORMAT>  RenderTargetSurface;  //!<  作成可能なレンダーサーフェスフォーマット
    std::vector<DEPTHSTENCILFORMAT>  DepthStencil;  //!<  作成可能なDepthStencilフォーマット
    std::vector<int>          IndexBit;             //!<  作成可能な１インデックスあたりのビット数
  };

  struct SUBRESOURCE
  {
    const void *pData;
    int Pitch;
    int SlicePitch;
  };

  struct SAMPLEDESC
  {
    int Count;
    int Quality;
  };

  enum RESOURCEUSAGE
  {
      RESOURCEUSAGE_DEFAULT,
      RESOURCEUSAGE_IMMUTABLE,
      RESOURCEUSAGE_DYNAMIC,
      RESOURCEUSAGE_STAGING,
  };

  enum RESOURCEBINDFLAG
  {
      RESOURCEBINDFLAG_VERTEX   = 0x01L,
      RESOURCEBINDFLAG_INDEX    = 0x02L,
      RESOURCEBINDFLAG_CONSTANT = 0x04L,
      RESOURCEBINDFLAG_MATERIAL = 0x08L,
      RESOURCEBINDFLAG_STREAMOUTPUT = 0x10L,
      RESOURCEBINDFLAG_RENDERTARGET = 0x20L,
      RESOURCEBINDFLAG_DEPTHSTENCIL = 0x40L,
  };

  enum CPUACCESSFLAG
  {
      CPUACCESSFLAG_WRITE = 0x10000L,
      CPUACCESSFLAG_READ  = 0x20000L,
  };

  enum RESOURCEMISCFLAG
  {
      RESOURCEMISCFLAG_GENERATE_MIPS = 0x1L,
      RESOURCEMISCFLAG_SHARED = 0x2L,
      RESOURCEMISCFLAG_TEXTURECUBE = 0x4L,
      RESOURCEMISCFLAG_SHARED_KEYEDMUTEX = 0x10L,
      RESOURCEMISCFLAG_GDI_COMPATIBLE = 0x20L,
  };

  struct CREATEBUFFERPARAM
  {
    size_t Length;      //  データの長さ(byte単位)
    RESOURCEUSAGE Usage;
    unt   BindFlags;
    unt   CPUAccessFlags;
    unt   MiscFlags;
  };


  struct CREATERETEXTURE2DPARAM
  {
    SIZE2DI Size;
    int MipLevels;
    int ArraySize;      //  何枚あるか？
    PIXELFORMAT Format; //  ピクセルフォーマット
    SAMPLEDESC  Sample;
    RESOURCEUSAGE Usage;
    unt   BindFlags;
    unt   CPUAccessFlags;
    unt   MiscFlags;
  };

  struct CREATERECONSTANTPARAM
  {
    const void* pData;  //  バッファデータ
    size_t  Length;     //  データの長さ(byte単位)
  };

  struct CREATERENDERTARGETPARAM
  {
    enum DIMENSION
    {
      DIMENSION_BUFFER,
      DIMENSION_TEXTURE1D,
      DIMENSION_TEXTURE1DARRAY,
      DIMENSION_TEXTURE2D,
      DIMENSION_TEXTURE2DARRAY,
      DIMENSION_TEXTURE2DMS,
      DIMENSION_TEXTURE2DMSARRAY,
      DIMENSION_TEXTURE3D,
    };

    PIXELFORMAT Format; //  想定させるフォーマット
    DIMENSION   Dimension;
    unt32       Param[4];
  };

  struct CREATEDEPTHSTENCILPARAM
  {
    enum DIMENSION
    {
      DIMENSION_TEXTURE1D,
      DIMENSION_TEXTURE1DARRAY,
      DIMENSION_TEXTURE2D,
      DIMENSION_TEXTURE2DARRAY,
      DIMENSION_TEXTURE2DMS,
      DIMENSION_TEXTURE2DMSARRAY,
    };
    enum FLAG
    {
      FLAG_READ_ONLY_DEPTH   = 0x01,
      FLAG_READ_ONLY_STENCIL = 0x02,
    };

    DEPTHSTENCILFORMAT Format; //  想定させるフォーマット
    DIMENSION          Dimension;
    unt32       ReadOnlyUsage;
    unt32       Param[4];
  };

  struct CREATESHADERMATERIALPARAM
  {
    enum DIMENSION
    {
      DIMENSION_BUFFER,
      DIMENSION_TEXTURE1D,
      DIMENSION_TEXTURE1DARRAY,
      DIMENSION_TEXTURE2D,
      DIMENSION_TEXTURE2DARRAY,
      DIMENSION_TEXTURE2DMS,
      DIMENSION_TEXTURE2DMSARRAY,
      DIMENSION_TEXTURE3D,
      DIMENSION_TEXTURECUBE,
      DIMENSION_TEXTURECUBEARRAY,
      DIMENSION_BUFFEREX,
    };
    enum FLAG
    {
      FLAG_READ_ONLY_DEPTH   = 0x01,
      FLAG_READ_ONLY_STENCIL = 0x02,
    };

    DEPTHSTENCILFORMAT Format; //  想定させるフォーマット
    DIMENSION          Dimension;
    unt32       Param[4];
  };

  enum COMPARISON
  {
    COMPARISON_NEVER        = 1,	//!<	常に通る
    COMPARISON_LESS         = 2,	//!<	o >  n
    COMPARISON_EQUAL		    = 3,	//!<	o == n
    COMPARISON_LESSEQUAL	  = 4,	//!<	o >= n
    COMPARISON_GREATER		  = 5,	//!<	o <  n
    COMPARISON_NOTEQUAL     = 6,	//!<	o != n
    COMPARISON_GREATEREQUAL = 7,	//!<	o <= n
    COMPARISON_ALWAYS       = 8,	//!<	常にダメ
  };

  struct SAMPLERSTATEPARAM
  {
    enum FILTER
    {
      FILTER_POINT,
      FILTER_LINEAR,
      FILTER_ANISOTROPIC,
    };

    enum ADDRESS
    {
      ADDRESS_WRAP,
      ADDRESS_MIRROR,
      ADDRESS_CLAMP,
      ADDRESS_BORDER,
      ADDRESS_MIRROR_ONCE,
    };



    FILTER  MinFilter;
    FILTER  MagFilter;
    FILTER  MipFilter;

    int   MaxAnisotropy;

    ADDRESS AddressU;
    ADDRESS AddressV;
    ADDRESS AddressW;

    float MipLODBias;
    COMPARISON   ComparisonFunc;
    COLOR_R32G32B32A32F BorderColor;
    float MinLOD;
    float MaxLOD;

    SAMPLERSTATEPARAM()
      :MinFilter(FILTER_POINT)
      ,MagFilter(FILTER_POINT)
      ,MipFilter(FILTER_POINT)
      ,MaxAnisotropy(16)
      ,AddressU(ADDRESS_CLAMP)
      ,AddressV(ADDRESS_CLAMP)
      ,AddressW(ADDRESS_CLAMP)
      ,MipLODBias(0.0f)
      ,ComparisonFunc(COMPARISON_NEVER)
      ,BorderColor(0,0,0,0)
      ,MinLOD(0.0f)
      ,MaxLOD(3.402823466e+38F)
    {
    }
  };


  struct RASTERIZERSTATEPARAM
  {
    enum FILL
    {
      FILL_WIREFRAME,
      FILL_SOLID,
    };

    enum CULLING
    {
      CULLING_NONE=0,
      CULLING_LEFT,	//!<	反時計回りにカリング
      CULLING_RIGHT	//!<	時計回りにカリング
    };

    FILL    Fill;
    CULLING Culling;
    bool MultiSample;

    RASTERIZERSTATEPARAM()
      :Fill(FILL_SOLID)
      ,Culling(CULLING_LEFT)
      ,MultiSample(false)
    {
    }
  };

  struct BLENDSTATEPARAM
  {
    struct TARGET
    {
      enum BLEND
      {
        BLEND_ZERO,
        BLEND_ONE,
        BLEND_SRC_COLOR,
        BLEND_INV_SRC_COLOR,
        BLEND_SRC_ALPHA,
        BLEND_INV_SRC_ALPHA,
        BLEND_DEST_ALPHA,
        BLEND_INV_DEST_ALPHA,
        BLEND_DEST_COLOR,
        BLEND_INV_DEST_COLOR,
        BLEND_SRC_ALPHA_SAT,
        BLEND_BLEND_FACTOR,
        BLEND_INV_BLEND_FACTOR,
      };

      enum OPERATION
      {
        OPERATION_ADD,
        OPERATION_SUBTRACT,
        OPERATION_REV_SUBTRACT,
        OPERATION_MIN,
        OPERATION_MAX,
      };

      enum MASK
      {
        MASK_RED  =0x01,
        MASK_GREEN=0x02,
        MASK_BLUE =0x04,
        MASK_ALPHA=0x08,
        MASK_ALL = MASK_RED|MASK_GREEN|MASK_BLUE|MASK_ALPHA
      };
      bool  Enable;

      BLEND ColorSrc;
      BLEND ColorDst;
      OPERATION ColorOp;
      BLEND AlphaSrc;
      BLEND AlphaDst;
      OPERATION AlphaOp;
      unt08 WriteMask;

      TARGET()
        :Enable(false)
        ,ColorSrc (BLEND_ONE)
        ,ColorDst (BLEND_ZERO)
        ,ColorOp  (OPERATION_ADD)
        ,AlphaSrc (BLEND_ONE)
        ,AlphaDst (BLEND_ZERO)
        ,AlphaOp  (OPERATION_ADD)
        ,WriteMask(0xFF)
      {}
    };

    bool  AlphaToCoverageEnable;  //  さっぱり不明
    bool  IndependentBlendEnable; //  さっぱり不明

    TARGET  Target[8];

    BLENDSTATEPARAM()
      :AlphaToCoverageEnable(false)
      ,IndependentBlendEnable(false)
    {
    }
  };

  struct INPUT_ELEMENT
  {
    enum TYPE
    {
      TYPE_FLOAT1,
      TYPE_FLOAT2,
      TYPE_FLOAT3,
      TYPE_FLOAT4,
      TYPE_COLOR,
    };
    enum METHOD {
        METHOD_DEFAULT = 0,
        METHOD_PARTIALU = 1,
        METHOD_PARTIALV = 2,
        METHOD_CROSSUV = 3,
        METHOD_UV = 4,
        METHOD_LOOKUP,
        METHOD_LOOKUPPRESAMPLED
    };

    char*  SemanticName;
    int    SemanticIndex;
    TYPE   Type;
    int    SlotNo;
    int    Offset;
    METHOD Method;
  };

  struct VIEWPORT
  {
    RECT2DI Screen; //!<  表示範囲
    float   MinZ;   //!<  カリングＺ
    float   MaxZ;   //!<  カリングＺ
  };

}}

#endif