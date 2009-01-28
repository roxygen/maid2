#ifndef maid2_graphics_graphics3drender_h
#define maid2_graphics_graphics3drender_h

/*!
    @file
    @brief  描画を受け持つクラス 3D ver.
 */


#include"../config/define.h"
#include"../auxiliary/mathematics/size.h"
#include"../auxiliary/globalpointer.h"

#include"graphicscore.h"
#include"vertex.h"
#include"texture2dbase.h"
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"
#include"rendertargetbase.h"
#include"samplerstate.h"
#include"modelmqo.h"
#include"camera.h"
#include"shaderconstant.h"

namespace Maid
{
  class Graphics3DRender
    : public GlobalPointer<GraphicsCore>
  {
  public:
    Graphics3DRender();
    virtual ~Graphics3DRender();

    void Initialize();
    void Initialize( const Graphics::SPDRAWCOMMAND& pCommand );

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );
    void ClearDepth( float Depth );
    void ClearStencil( unt08 Stencil );
    void ClearDepthStencil( float Depth, unt08 Stencil );

    void Begin();
    void End();

    void SetDefaultSetting();

    void SetCamera( const Camera& cam );
    void SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth );

    void Fill ();

    void Blt  ( const POINT3DF& Pos, const ModelMQO& model );
 
    bool IsInitializing() const;

  protected:
    bool IsMemberLoading() const;

    enum STATE
    {
      STATE_EMPTY,    //  まだ初期化されてない
      STATE_LOADING,  //  初期設定中
      STATE_WORKING,  //  稼動できる
    };

    STATE m_State;

  protected:
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    Vertex  m_TestVertex;
    InputLayout  m_TestInputLayout;
    Index  m_TestIndex;


    InputLayout  m_MQOLayout;
    VertexShader m_MQOVertexShader;
    PixelShader  m_MQOPixelShader;

    ShaderConstant  m_ShaderConstant;
    RasterizerState m_Rasterizer;

    SamplerState  m_Sampler;

    DepthStencilState m_DepthOn;

    BlendState m_BlendState;

		MATRIX4DF	m_ViewMatrix;
		MATRIX4DF	m_ProjectionMatrix;
  };	
	
}

#endif