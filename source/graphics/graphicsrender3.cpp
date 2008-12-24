#include"graphicsrender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"



namespace Maid
{
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したテクスチャを描画する
/*!
    @param	Pos     [i ]	描画開始座標
    @param	Texture [i ]	描画するもの
    @param	Size    [i ]	切り抜き範囲
    @param	Center  [i ]	中心座標
    @param	alpha   [ o]	透明度
 */
void GraphicsRender::Blt  ( const POINT2DI& Pos, const Texture2D& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  const COLOR_R32G32B32A32F col(1,1,1,alpha);

  MODELINFO m;
  CreateVirtualScreenModel( Pos, rc.GetSize(), Center, m );
  Blt( m.Model, m.Translate, Texture, rc, col );
}

/*
void GraphicsRender::FillS ( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, const COLOR_R32G32B32A32F& Color )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, Size, Center, m );
  Fill( m.Model, MATRIX4DF().SetScale(Scale.w,Scale.h,1)*m.Translate, Color );
}
void GraphicsRender::FillR ( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec, const COLOR_R32G32B32A32F& Color )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  MODELINFO m;
  CreateVirtualScreenModel( Pos, Size, Center, m );
  Fill( m.Model, MATRIX4DF().SetRotationXYZ(Rotate,vec)*m.Translate, Color );
}

void GraphicsRender::FillSR( const POINT2DI& Pos, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const COLOR_R32G32B32A32F& Color )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  MODELINFO model;
  CreateVirtualScreenModel( Pos, Size, Center, model );

  const MATRIX4DF m = MATRIX4DF().SetScale(Scale.w,Scale.h,1)
                    * MATRIX4DF().SetRotationXYZ(Rotate,vec)
                    * model.Translate;

  Fill( model.Model, m, Color );
}
*/

void GraphicsRender::Blt( const VECTOR4DF* Model, const MATRIX4DF& mat, const Texture2D& Texture,  const RECT2DI& rc, const COLOR_R32G32B32A32F& Color )
{
  //  uvの計算
  const SIZE2DI& image = Texture.GetImageSize();
  const SIZE2DI& create= Texture.GetCreateSize();
  const SIZE2DI& texsize=Texture.GetTextureSize();

  const int x = rc.x * image.w / create.w;
  const int y = rc.y * image.h / create.h;
  const int w = rc.w * image.w / create.w;
  const int h = rc.h * image.h / create.h;

  const float ux = float(x) / float(texsize.w);
  const float vy = float(y) / float(texsize.h);
  const float uw = float(w) / float(texsize.w);
  const float vh = float(h) / float(texsize.h);


  CUSTOMVERTEX_SPRITE v[] =
  {
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux   ,vy   ) ),
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux+uw,vy   ) ),
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux   ,vy+vh) ),
    CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux+uw,vy+vh) ),
  };

  SetupVertex( Model, mat, v, v[0].GetStructSize() );

  const IVertex&       vertex = m_CommonVertex;
  const IInputLayout&  layout = m_BltLayout;
  const IVertexShader& vs = m_BltVertexShader;
  const IPixelShader&  ps = m_BltPixelShader;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  {
    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    pCommand->ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, map );
    memcpy( map.pData, v, map.Pitch );
    pCommand->ResourceUnmap( vertex.Get(), sub );
  }

  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  pCommand->SetVertex( 0, vertex.Get(), 0, v[0].GetStructSize() );
  pCommand->SetInputLayout( layout.Get() );

  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );
  pCommand->Draw( 4, 0 );
}

}