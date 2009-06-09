#ifndef maid2_graphics_graphicscommandcontrol_h
#define maid2_graphics_graphicscommandcontrol_h

/*!
    @file
    @brief  描画ではなく初期設定を行うクラス
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicscore.h"

namespace Maid
{
  class GraphicsCommandControl
    : public GlobalPointer<GraphicsCore>
  {
  public:
    GraphicsCommandControl();
    virtual ~GraphicsCommandControl();

    void Initialize();
    void Initialize( const Graphics::SPDRAWCOMMAND& pCommand );

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );
    void ClearDepth( float Depth );
    void ClearStencil( unt08 Stencil );
    void ClearDepthStencil( float Depth, unt08 Stencil );

    void Begin();
    void End();

    void SetRenderTarget( const RenderTargetBase& Target );
    void SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth );

    Graphics::IDrawCommand& GetCommand();

    float GetTexelMapValue() const;


    enum MAPTYPE
    {
      MAPTYPE_READ = Graphics::IDrawCommand::MAPTYPE_READ,
      MAPTYPE_WRITE = Graphics::IDrawCommand::MAPTYPE_WRITE,
      MAPTYPE_READ_WRITE = Graphics::IDrawCommand::MAPTYPE_READ_WRITE,
      MAPTYPE_WRITE_DISCARD = Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD,
      MAPTYPE_WRITE_NO_OVERWRITE = Graphics::IDrawCommand::MAPTYPE_WRITE_NO_OVERWRITE,
    };

    enum MAPFLAG
    {
      MAPFLAG_DO_NOT_WAIT = Graphics::IDrawCommand::MAPFLAG_DO_NOT_WAIT,
    };
    struct MAPPEDRESOURCE
    {
      void *pData;
      int Pitch;
      int Slice;

      MAPPEDRESOURCE()
        :pData(NULL)
        ,Pitch(0)
        ,Slice(0)
      {}
    };

    void ResourceMap( const GraphicsObject<Graphics::SPBUFFER>& res, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped );
    void ResourceMap( const ITexture2D& res, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped );
    void ResourceUnmap( const GraphicsObject<Graphics::SPBUFFER>& res, int SubLevel );
    void ResourceUnmap( const ITexture2D& res, int SubLevel );

    void SetIndex( const IIndex& Buffer, size_t Offset );
    void SetVertex( int Slot, const IVertex& Buffer, int Offset, int Stride );
    void SetInputLayout( const IInputLayout& Layout );

    enum PRIMITIVE_TOPOLOGY
    {
      PRIMITIVE_TOPOLOGY_POINTLIST = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_POINTLIST,
      PRIMITIVE_TOPOLOGY_LINELIST = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_LINELIST,
      PRIMITIVE_TOPOLOGY_LINESTRIP = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_LINESTRIP,
      PRIMITIVE_TOPOLOGY_TRIANGLELIST = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    };
    void SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology );

/*

    void VSSetConstant( int slot, const SPBUFFER& pBuffer );
    void VSSetMaterial( int stage, const SPMATERIAL& Material );
    void VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler );
    void VSSetShader( const SPVERTEXSHADER& Shader );

    void PSSetConstant( int slot, const SPBUFFER& pBuffer );
    void PSSetMaterial( int stage, const SPMATERIAL& Material );
    void PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler );
    void PSSetShader( const SPPIXELSHADER& Shader );

    void SetRasterizerState( const SPRASTERIZERSTATE& State );
    void SetDepthStencilState( const SPDEPTHSTENCILSTATE& State );
    void SetBlendState( const SPBLENDSTATE& State );
    void SetViewPort( const VIEWPORT& port );
    void SetRenderTarget( int no, const SPRENDERTARGET& Color, const SPDEPTHSTENCIL& Depth );
    VIEWPORT GetViewport();
    void GetRenderTarget( int no, SPRENDERTARGET& Color, SPDEPTHSTENCIL& Depth );
    void Draw( size_t UseVertexCount, size_t StartVertex );
    void DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex );


*/




  protected:
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    float m_TexelMapValue;
  };
}

#endif