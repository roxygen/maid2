#include"graphicsrender.h"
#include"customvertextemplate.h"




namespace Maid
{
void GraphicsRender::Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand )
{
  {
    //  1頂点あたり128バイトも使わないでしょう
    m_CommonVertex.Create( 128*4 );
  }


  {

    {
      const char* vs =
        "vs.1.1\n"
        "dcl_position v0\n"
        "dcl_color v1\n"
        "mov oPos, v0\n"
        "mov oD0, v1\n"
        ;

      m_FillVertexShader.Create( String::ConvertSJIStoMAID(vs) );
    }

    {
      const char* ps =
        "0"
        ;

      m_FillPixelShader.Create( String::ConvertSJIStoMAID(ps) );
    }

    {
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_FillLayout.Create( element, NUMELEMENTS(element), NULL, 0 );
    }
  }

  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_NONE;
    param.MultiSample = false;

    m_Rasterizer.Create( param );
  }
  {
    Graphics::DEPTHSTENCILSTATEPARAM param;
    param.DepthTest = true;
    m_DepthOn.Create( param );
    param.DepthTest = false;
    m_DepthOff.Create( param );
  }

  {
    m_BlendState.resize( 5 );
    m_BlendState[0].Create( BlendState::TEMPLATE_NORMAL );
    m_BlendState[1].Create( BlendState::TEMPLATE_ALPHA );
    m_BlendState[2].Create( BlendState::TEMPLATE_ADD );
    m_BlendState[3].Create( BlendState::TEMPLATE_SUB );
    m_BlendState[4].Create( BlendState::TEMPLATE_MUL );
  }

  {
    const Graphics::VIEWPORT view = pDrawCommand->GetViewport();
    m_VirtualScreenSize = SIZE2DI( view.Screen.w, view.Screen.h );
  }

  m_pDrawCommand = pDrawCommand;
}

void GraphicsRender::Begin()
{
  m_pDrawCommand->Begin();
}

void GraphicsRender::End()
{
  m_pDrawCommand->End();
}

void GraphicsRender::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 仮想サイズの変更
/*!
    @param  Size [i ] 新しいサイズ
 */
void GraphicsRender::SetVirtualScreenSize( const SIZE2DI& Size )
{
  m_VirtualScreenSize = Size;
}


bool GraphicsRender::IsLoading() const
{
  return m_FillVertexShader.IsCompiling()
    || m_FillPixelShader.IsCompiling()
    ;
}

void GraphicsRender::SetDefaultSetting()
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  SetDepthTest( false );
  SetBlendState( BLENDSTATE_ALPHA );
  IRasterizerState& raster = m_Rasterizer;
  m_pDrawCommand->SetRasterizerState( raster.Get() );
}



void GraphicsRender::SetDepthTest( bool Enable )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );
  IDepthStencilState& state = Enable? m_DepthOn : m_DepthOff;
  m_pDrawCommand->SetDepthStencilState( state.Get() );
}

void GraphicsRender::SetBlendState( BLENDSTATE State )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );
  IBlendState& state = m_BlendState[State];

  m_pDrawCommand->SetBlendState( state.Get() );
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 与えられたパラメーターからスクリーンサイズを加味した平面モデルを作成する
/*!
    @param	Pos    [i ]	配置する座標
    @param	Size   [i ]	平面の大きさ
    @param	Center [i ]	平面の中心点
    @param	pos    [ o]	出来上がったモデル(4頂点であること)
 */
void GraphicsRender::CreateVirtualScreenModel( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, MODELINFO& m )
{
  const SIZE2DI& screen = m_VirtualScreenSize;

  const float w = float(Size.w) / float(screen.w/2);
  const float h = float(Size.h) / float(screen.w/2);
  const float x = float(Center.x) / float(screen.w/2);
  const float y = float(Center.y) / float(screen.w/2);

  m.Model[0] = VECTOR4DF(-x,  -y,  0,1);  //  左上
  m.Model[1] = VECTOR4DF(-x+w,-y,  0,1);  //  右上
  m.Model[2] = VECTOR4DF(-x,  -y+h,0,1);  //  左下
  m.Model[3] = VECTOR4DF(-x+w,-y+h,0,1);  //  右下

  const float aspect = float(screen.w) / float(screen.h);
  const float TranceX = float(Pos.x-screen.w/2) / float(screen.w/2);
  const float TranceY = float(Pos.y-screen.h/2) / float(screen.h/2);

  m.Translate = MATRIX4DF().SetScale(1,aspect,1)
              * MATRIX4DF().SetTranslate(TranceX,TranceY,0);

}

void GraphicsRender::SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride )
{
  unt08* p = (unt08*)pVertex;
  for( int i=0; i<4; ++i )
  {
    const VECTOR4DF r   = Model[i] * Mat;
    POINT3DF* pos = (POINT3DF*)p;
    pos->x = r.x;
    pos->y = -r.y;
    pos->z = r.z;

    p += Stride;
  }
}

}