#ifndef maid2_graphics_core_idrawcommand_h
#define maid2_graphics_core_idrawcommand_h

#include"../../config/define.h"

#include"iobject.h"
#include"../color.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	IDrawCommand idrawcommand.h
      @brief	描画コンテキスト
  */

  class IDrawCommand : public IObject
  {
  public:
    IDrawCommand(TYPE t):IObject(t){}

    virtual ~IDrawCommand(){}

    virtual void Begin()=0;
    virtual void End()=0;

    enum CLEARFLAG
    {
      CLEARFLAG_DEPTH,
      CLEARFLAG_STENCIL,
      CLEARFLAG_DEPTHSTENCIL,
    };

    virtual void ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )=0;
    virtual void ClearRenderTarget( const COLOR_A32B32G32R32F& Col )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファのセット
    /*!
      @param	pBuffer	[i ]	設定するインデックス
     */
    virtual void SetIndex( const IDevice::RESOURCE& Buffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファのセット
    /*!
      @param	Slot  	[i ]	設定する場所
      @param	pBuffer	[i ]	設定するバッファ
     */
    virtual void SetVertex( int Slot, const IDevice::RESOURCE& Buffer )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義のセット
    /*!
      @param	pDecl	[i ]	設定する定義
     */
    virtual void SetInputLayout( const IDevice::INPUTLAYOUT& Layout )=0;


    virtual void VSSetMaterial( int stage, const IDevice::SHADERMATERIAL& Buffer )=0;
    virtual void VSSetShader( const IDevice::VERTEXSHADER& Shader )=0;
    virtual void VSSetSamplerState( const IDevice::SAMPLERSTATE& Sampler )=0;


    virtual void PSSetMaterial( int stage, const IDevice::SHADERMATERIAL& Buffer )=0;
    virtual void PSSetShader( const IDevice::PIXELSHADER& Shader )=0;
    virtual void PSSetSamplerState( const IDevice::SAMPLERSTATE& Sampler )=0;

    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ ) = 0;
    virtual void SetRasterizerState( const IDevice::RASTERIZERSTATE& State ) = 0;
    virtual void SetBlendState( const IDevice::BLENDSTATE& State ) = 0;
    virtual void SetRenderTarget( int no, const IDevice::RENDERTARGET& Color, const IDevice::DEPTHSTENCIL Depth )=0;
    virtual void ResetRenderTarget( int no )=0;
  };

  typedef	boost::shared_ptr<IDrawCommand>	SPDRAWCOMMAND;

}}

#endif