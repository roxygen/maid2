#ifndef maid2_graphics_core_win32_direct3d09_drawcommandexecuted3d09_h
#define maid2_graphics_core_win32_direct3d09_drawcommandexecuted3d09_h

#include"../../../../config/define.h"

#include"../../idrawcommandexecute.h"
#include"d3d09include.h"
#include"common.h"


namespace Maid { namespace Graphics {

  /*!	
      @class	DrawCommandExecuteD3D09 drawcommandplayerd3d09.h
      @brief	描画コンテキスト 即時実行タイプ
  */

  class DrawCommandExecuteD3D09 : public IDrawCommandExecute
  {
  public:
    DrawCommandExecuteD3D09( const SPD3D09DEVICE& pDev, const OBJECTMAP& ObjectMap );

    virtual void Begin();
    virtual void End();

    virtual void ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil );
    virtual void ClearRenderTarget( const COLOR_A32B32G32R32F& Col );

    virtual void SetIndex( const IIndex& Buffer );
    virtual void SetVertex( int Slot, const IVertex& Buffer );
    virtual void SetInputLayout( const IInputLayout& Layout );
    virtual void VSSetMaterial( int stage, const IMaterial& Buffer );
    virtual void VSSetShader( const IVertexShader& Shader );
    virtual void VSSetSamplerState( const ISamplerState& Sampler );
    virtual void PSSetMaterial( int stage, const IMaterial& Buffer );
    virtual void PSSetShader( const IPixelShader& Shader );
    virtual void PSSetSamplerState( const ISamplerState& Sampler );
    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ );
    virtual void SetRasterizerState( const IRasterizerState& State );
    virtual void SetBlendState( const IBlendState& State );
    virtual void SetRenderTarget( int no, const IRenderTarget& Color, const IDepthStencil& Depth );
    virtual void ResetRenderTarget( int no );

  private:
    const SPD3D09DEVICE&  m_pDevice;
    const OBJECTMAP&      m_ObjectMap;
  };

}}

#endif