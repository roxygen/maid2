#include"graphicsrender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"



namespace Maid
{
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した色で塗りつぶす
/*!
    @param	Pos    [i ]	塗り開始座標
    @param	Size   [i ]	塗りつぶしサイズ
    @param	Center [i ]	中心座標
    @param	Color  [ o]	塗りつぶす色
 */
void GraphicsRender::Fill( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, Size, Center, m );
  Fill( m.Model, m.Translate, Color );
}
	
void GraphicsRender::FillS ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, Size, Center, m );
  Fill( m.Model, MATRIX4DF().SetScale(Scale.w,Scale.h,1)*m.Translate, Color );
}
void GraphicsRender::FillR ( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, Size, Center, m );
  Fill( m.Model, MATRIX4DF().SetRotationXYZ(Rotate,vec)*m.Translate, Color );
}

void GraphicsRender::FillSR( const POINT2DI& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  MODELINFO model;
  CreateVirtualScreenModel( Pos, Size, Center, model );

  const MATRIX4DF m = MATRIX4DF().SetScale(Scale.w,Scale.h,1)
                    * MATRIX4DF().SetRotationXYZ(Rotate,vec)
                    * model.Translate;

  Fill( model.Model, m, Color );
}


void GraphicsRender::Fill( const VECTOR4DF* Model, const MATRIX4DF& mat, const COLOR_R32G32B32A32F& Color )
{
  CUSTOMVERTEX_COLOR v[] =
  {
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), Color ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), Color ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), Color ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), Color ),
  };

  SetupVertex( Model, mat, v, v[0].GetStructSize() );
  SetCommonVertex( v, sizeof(v), v[0].GetStructSize() );

  const IInputLayout&  layout = m_FillLayout;
  const IVertexShader& vs = m_FillVertexShader;
  const IPixelShader&  ps = m_FillPixelShader;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  pCommand->SetInputLayout( layout.Get() );

  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );
  pCommand->Draw( 4, 0 );
}

}