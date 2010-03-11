﻿#include"graphics3drender.h"

#include"customvertextemplate.h"
#include"graphics3drenderlocal.h"

namespace Maid
{


//  MQO に座標と色があって、ライトがない
static const char* VSCODE_COLOR_LIGHT0 = 
"cbuffer cbPerObject"
"{"
"  matrix s_mWVP  : packoffset( c0 );"
"  float4 s_MaterialColor : packoffset( c4 );"
"  float1 s_Alpha: packoffset( c5 );" // プログラム側で調節する透明度
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION;"    //頂点座標
"  float4 Diffuse     : COLOR0;"      //デフューズ色
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position    : SV_Position;"   //頂点座標
"  float4 Diffuse     : COLOR0;"      //デフューズ色
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
"  Out.Position = mul( Input.Position, s_mWVP );"
"  Out.Diffuse = Input.Diffuse * s_MaterialColor;"
"  Out.Diffuse.a *= s_Alpha;"
""
"  return Out;"
"}"
;

//  MQO に座標と色があって、平行光源のみ
static const char* VSCODE_COLOR_DIRECTIONALLIGHT =
"cbuffer cbPerObject"
"{"
"  matrix s_mWVP  : packoffset( c0 );"
"  float4 s_MaterialColor : packoffset( c4 );" // 素材そのものの色
"  float4 s_MaterialLight : packoffset( c5 );" // 素材の光源反射率
"  float4 s_MaterialEmissive : packoffset( c6 );" // 素材の自己発光量
"  float4 s_LightDir     : packoffset( c7 );"  // 平行光源の向き
"  float4 s_LightColor   : packoffset( c8 );"  // 平行光源の色
"  float4 s_Ambient      : packoffset( c9 );"  // ワールド全体の明るさ
"  float1 s_Alpha        : packoffset( c10 );" // プログラム側で調節する透明度
"  float4 s_Eye          : packoffset( c11 );" // 視線ベクトル
"  float2 s_Speculer     : packoffset( c12 );" // スペキュラの強さ
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION; "  //頂点座標
"  float4 Diffuse     : COLOR0;"     //頂点色影響度
"  float4 Normal      : NORMAL;"     //法線
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position    : SV_Position;" //頂点座標
"  float4 Diffuse     : COLOR0;"      //デフューズ色
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
""
"  float  lightpow = dot(Input.Normal,-s_LightDir);"
"  float4 lightcol = s_LightColor * lightpow;"
""
"  float4 matcolor = max(0.0,  s_MaterialColor * s_MaterialLight * lightcol );"
"  float4 emi = s_MaterialColor * s_MaterialEmissive;"
""
"  float3 eye  = normalize(s_Eye-Input.Position.xyz);"
"  float3 half = normalize(-s_LightDir+eye);"
"  float  spc  = pow(max(0.0,dot(Input.Normal,half)),s_Speculer.y) * s_Speculer.x;"
""
"  float4 worldcolor = emi + matcolor + s_Ambient + spc;"
"  worldcolor.a = s_MaterialColor.a;"
""
"  Out.Position = mul( Input.Position, s_mWVP );"
"  Out.Diffuse = Input.Diffuse * worldcolor;"
"  Out.Diffuse.a *= s_Alpha;"
""
"  return Out;"
"}"
;






//  カラーを送るだけ
static const char* PSCODE_COLOR = 
"struct PS_INPUT"
"{"
"  float4 Position : SV_Position;"
"  float4 Color    : COLOR0;"
"};"
""
"float4 main( PS_INPUT input) : SV_Target"
"{"
"  return input.Color;"
"}"
;





















//  MQO に座標と色とuvがあって、ライトがない
static const char* VSCODE_TEXTURE_LIGHT0 = 
"cbuffer cbPerObject"
"{"
"  matrix s_mWVP  : packoffset( c0 );"
"  float2 s_TextureScale : packoffset( c4 );"   //テクスチャのＵＶとデータ上のＵＶを調節するための値
"  float  s_Alpha : packoffset( c5 );"   //プログラム側で調節する透明度
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION;"    //頂点座標
"  float4 Diffuse     : COLOR0; "     //デフューズ色
"  float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position    : SV_Position;" //頂点座標
"  float4 Diffuse     : COLOR0;"      //デフューズ色
"  float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
"  Out.Position = mul( Input.Position, s_mWVP );"
"  Out.Diffuse  = Input.Diffuse;"
"  Out.Diffuse.a*= s_Alpha;"
"  Out.TexCoords = Input.TexCoords * s_TextureScale;"
""
"  return Out;"
"}"
;


//  MQO に座標と色とuvがあって、平行光源がある
static const char* VSCODE_TEXTURE_DIRECTIONALLIGHT = 
"cbuffer cbPerObject"
"{"
"  matrix s_mWVP  : packoffset( c0 );"
"  float4 s_MaterialLight : packoffset( c4 );" // 素材の光反射率
"  float4 s_MaterialEmissive : packoffset( c5 );" // 素材の光反射率
"  float4 s_LightDir     : packoffset( c6 );"  // 平行光源の向き
"  float4 s_LightColor   : packoffset( c7 );"  // 平行光源の色
"  float4 s_Ambient      : packoffset( c8 );"  // ワールド全体の明るさ
"  float2 s_TextureScale : packoffset( c9 );"   //テクスチャのＵＶとデータ上のＵＶを調節するための値
"  float  s_Alpha : packoffset( c10 );"   //プログラム側で調節する透明度
"  float4 s_Eye          : packoffset( c11 );" // 視線ベクトル
"  float2 s_Speculer     : packoffset( c12 );" // スペキュラの強さ
"};"
""
"struct VS_INPUT"
"{"
"  float4 Position    : POSITION;"   //頂点座標
"  float4 Diffuse     : COLOR0;"     //頂点色影響度
"  float4 Normal      : NORMAL; "    //法線
"  float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"};"
""
"struct VS_OUTPUT"
"{"
"  float4 Position    : SV_Position;"  //頂点座標
"  float4 Diffuse     : COLOR0;"     //デフューズ色
"  float2 TexCoords   : TEXCOORD0;"   //テクスチャUV
"};"
""
"VS_OUTPUT main(VS_INPUT Input)"
"{"
"  VS_OUTPUT Out = (VS_OUTPUT)0;"
""
"  float  lightpow = dot(Input.Normal,-s_LightDir);"
"  float4 lightcol = s_LightColor * lightpow;"
""
"  float3 eye  = normalize(s_Eye-Input.Position.xyz);"
"  float3 half = normalize(-s_LightDir+eye);"
"  float  spc  = pow(max(0.0,dot(Input.Normal,half)),s_Speculer.y) * s_Speculer.x;"
""
"  float4 matcolor = max(0.0, s_MaterialLight * lightcol );"
"  float4 worldcolor = s_MaterialEmissive + matcolor + s_Ambient + spc;"
"  worldcolor.a = s_Alpha;"
""
"  Out.Position = mul( Input.Position, s_mWVP );"
"  Out.Diffuse = Input.Diffuse * worldcolor;"
"  Out.TexCoords = Input.TexCoords * s_TextureScale;"
""
"  return Out;"
"}"
;




//  テクスチャとカラーを合成
static const char* PSCODE_TEXTURECOLOR = 
"Texture2D<float4> texture_slot0;"
"sampler sampler_slot0 = sampler_state"
"{"
"    Texture   = <texture_slot0>;"
"};"
""
"struct PS_INPUT"
"{"
"    float4 Position : SV_Position;"
"    float4 Color : COLOR0;"
"    float2 TexUV : TEXCOORD0;"
"};"
""
"float4 main( PS_INPUT input) : SV_Target"
"{"
"    return texture_slot0.Sample(sampler_slot0,input.TexUV) * input.Color;"
"}"
;

void Graphics3DRender::MQOShaderCreate()
{
  //A
  //0  MQOMATERIAL.Texture無 MQOMATERIAL.Bump無 MQOMATERIAL.Color * MQOOBJECT.Color
  //1  MQOMATERIAL.Texture有 MQOMATERIAL.Bump無 MQOMATERIAL.Color * MQOOBJECT.Color
  //2  MQOMATERIAL.Texture有 MQOMATERIAL.Bump有 MQOMATERIAL.Color * MQOOBJECT.Color

  //B
  //0 光源無し
  //1 平行光源

  //  A*10+B でidを割り振る

  {
    {
      const int id = 0*10+0;
      m_MQOVertexShader[id].Create( MAIDTEXT(VSCODE_COLOR_LIGHT0) );
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };
      m_MQOLayout[id].Create( element, NUMELEMENTS(element), MAIDTEXT(VSCODE_COLOR_LIGHT0) );

      m_MQOPixelShader[id].Create( MAIDTEXT(PSCODE_COLOR) );
    }
    {
      const int id = 0*10+1;

      m_MQOVertexShader[id].Create( MAIDTEXT(VSCODE_COLOR_DIRECTIONALLIGHT) );
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"NORMAL",   0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 2, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_MQOLayout[id].Create( element, NUMELEMENTS(element), MAIDTEXT(VSCODE_COLOR_DIRECTIONALLIGHT) );
      m_MQOPixelShader[id].Create( MAIDTEXT(PSCODE_COLOR) );
    }
  }
  {
    {
      const int id = 1*10+0;
      const String vs_code = MAIDTEXT(VSCODE_TEXTURE_LIGHT0);
      const String ps_code = MAIDTEXT(PSCODE_TEXTURECOLOR);
      m_MQOVertexShader[id].Create( vs_code );
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 3, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };
      m_MQOLayout[id].Create( element, NUMELEMENTS(element), vs_code );

      m_MQOPixelShader[id].Create( ps_code );
    }
    {
      const int id = 1*10+1;
      const String vs_code = MAIDTEXT(VSCODE_TEXTURE_DIRECTIONALLIGHT);
      const String ps_code = MAIDTEXT(PSCODE_TEXTURECOLOR);
      m_MQOVertexShader[id].Create( vs_code );
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"NORMAL",   0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 2, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 3, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };
      m_MQOLayout[id].Create( element, NUMELEMENTS(element), vs_code );

      m_MQOPixelShader[id].Create( ps_code );
    }
  }

  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_BACK;
    param.MultiSample = false;

    m_MQORasterizer.Create( param );
  }
  {
    m_MQOBlendState.Create( BlendState::TEMPLATE_ALPHA );
  }

}

bool Graphics3DRender::MQOShaderIsLoading() const
{
  for( std::map<int,InputLayout>::const_iterator ite=m_MQOLayout.begin();
                            ite!=m_MQOLayout.end(); ++ite )
  {
    if( ite->second.IsCompiling() ) { return true; }
  }

  for( std::map<int,VertexShader>::const_iterator ite=m_MQOVertexShader.begin();
                            ite!=m_MQOVertexShader.end(); ++ite )
  {
    if( ite->second.IsCompiling() ) { return true; }
  }

  for( std::map<int,PixelShader>::const_iterator ite=m_MQOPixelShader.begin();
                            ite!=m_MQOPixelShader.end(); ++ite )
  {
    if( ite->second.IsCompiling() ) { return true; }
  }

  return false;
}

void Graphics3DRender::MQOShaderSetup( const MATRIX4DF& world, const MATRIX4DF& wvp, const MQOMATERIAL& mat, float alpha )
{
  //  マテリアルの状況からシェーダ、パラメータを設定する
  int MaterialType = 0;
  if( mat.Texture.IsEmpty() )
  {
    MaterialType = 0;
  }else
  {
    if( mat.Bump.IsEmpty() ){ MaterialType = 1; }
    else
    {
      MaterialType = 2; 
    }
  }
  int LightingType = 0;
  {
    if( m_Light.empty() ) { LightingType = 0; }
    else                  { LightingType = 1; }
  }
  Graphics::IDrawCommand& Command = GetCommand();
  const int ShaderID = MaterialType*10 + LightingType;

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    Command.ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
    switch( ShaderID )
    {
    case 0:
      {
        CONSTANT0100& dst = *((CONSTANT0100*)map.pData);
        dst.s_mWVP  = wvp.GetTranspose();
        dst.s_MaterialColor = mat.Color;
        dst.s_Alpha = alpha;
      }break;

    case 1:
      {
        const LIGHT& light = m_Light[0];
        MAID_ASSERT( light.Type!=LIGHT::DIRECTIONAL, "平行光源のみサポート" );

        //  ライトをまわしてシェーダ演算量を減らす
        const VECTOR4DF v = 
          (VECTOR4DF(light.Direction.x, light.Direction.y, light.Direction.z, 0 )
          * world.GetInverse()).Normalize()
          ;

        CONSTANT_COLOR_DIRECTIONALLIGHT& dst = *((CONSTANT_COLOR_DIRECTIONALLIGHT*)map.pData);
        dst.s_mWVP  = wvp.GetTranspose();
        dst.s_MaterialColor = mat.Color;
        dst.s_MaterialLight = COLOR_R32G32B32A32F(mat.Diffuse,mat.Diffuse,mat.Diffuse,1);
        dst.s_MaterialEmissive = COLOR_R32G32B32A32F(mat.Emissive,mat.Emissive,mat.Emissive,1);
        dst.s_LightDir = v;
        dst.s_LightColor = light.Diffuse;
        dst.s_Ambient = m_Ambient;
        dst.s_Alpha = alpha;

        dst.s_Eye = VECTOR4DF(0,0,0,1) * (world*m_Camera.GetViewMatrix()).GetInverse();
        dst.s_Speculer = mat.Specular;
        dst.s_SpeculerPow = mat.Power;
      }break;

    case 10:
      {
        CONSTANT_TEXTURE_COLOR_LIGHT0& dst = *((CONSTANT_TEXTURE_COLOR_LIGHT0*)map.pData);
        dst.s_mWVP  = wvp.GetTranspose();
        dst.s_TextureScale = SIZE2DF(1.0f,1.0f);
        dst.s_Alpha = alpha;

        if( mat.Texture.IsSetupped() )
        {
          const SIZE2DF real = mat.Texture.GetRealSize();
          const SIZE2DF tex  = mat.Texture.GetTextureSize();
          dst.s_TextureScale = SIZE2DF(real.w/tex.w,real.h/tex.h);

          const IMaterial&  m = mat.Texture;
          Command.PSSetMaterial(0, m.Get() );
        }
      }break;

    case 11:
      {
        const LIGHT& light = m_Light[0];
        MAID_ASSERT( light.Type!=LIGHT::DIRECTIONAL, "平行光源のみサポート" );

        //  ライトをまわしてシェーダ演算量を減らす
        const VECTOR4DF v = 
          (VECTOR4DF(light.Direction.x, light.Direction.y, light.Direction.z, 0 )
          * world.GetInverse()).Normalize()
          ;

        CONSTANT_TEXTURE_DIRECTIONALLIGHT& dst = *((CONSTANT_TEXTURE_DIRECTIONALLIGHT*)map.pData);
        dst.s_mWVP  = wvp.GetTranspose();
        dst.s_MaterialLight = COLOR_R32G32B32A32F(mat.Diffuse,mat.Diffuse,mat.Diffuse,1);
        dst.s_MaterialEmissive = COLOR_R32G32B32A32F(mat.Emissive,mat.Emissive,mat.Emissive,1);
        dst.s_LightDir = v;
        dst.s_LightColor = light.Diffuse;
        dst.s_Ambient = m_Ambient;
        dst.s_TextureScale = SIZE2DF(1.0f,1.0f);
        dst.s_Alpha = alpha;

        dst.s_Eye = VECTOR4DF(0,0,0,1) * (world*m_Camera.GetViewMatrix()).GetInverse();
        dst.s_Speculer = mat.Specular;
        dst.s_SpeculerPow = mat.Power;

        if( mat.Texture.IsSetupped() )
        {
          const SIZE2DF real = mat.Texture.GetRealSize();
          const SIZE2DF tex  = mat.Texture.GetTextureSize();
          dst.s_TextureScale = SIZE2DF(real.w/tex.w,real.h/tex.h);

          const IMaterial&  m = mat.Texture;
          Command.PSSetMaterial(0, m.Get() );
        }
      }break;
    }
    Command.ResourceUnmap( con.Get(), sub );
    Command.VSSetConstant( 0, con.Get() );
  }
  {
    const IInputLayout&  layout = m_MQOLayout[ShaderID];
    const IVertexShader& vs = m_MQOVertexShader[ShaderID];
    const IPixelShader&  ps = m_MQOPixelShader[ShaderID];

    Command.SetInputLayout( layout.Get() );
    Command.VSSetShader( vs.Get() );
    Command.PSSetShader( ps.Get() );
  }
}

void Graphics3DRender::Blt( const POINT3DF& Pos, const ModelMQO& model, float alpha )
{
  const MATRIX4DF world = MATRIX4DF().SetTranslate(Pos.x,Pos.y,Pos.z);
  Blt( world, model, alpha );
}

void Graphics3DRender::BltS ( const POINT3DF& Pos, const ModelMQO& model, float alpha, const SIZE3DF& Scale )
{
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x, Pos.y, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, Scale.d );

  const MATRIX4DF world = s*t;
  Blt( world, model, alpha );
}

void Graphics3DRender::BltR ( const POINT3DF& Pos, const ModelMQO& model, float alpha, float Rotate, const VECTOR3DF& vec )
{
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x, Pos.y, Pos.z );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = r*t;
  Blt( world, model, alpha );
}

void Graphics3DRender::BltSR( const POINT3DF& Pos, const ModelMQO& model, float alpha, const SIZE3DF& Scale, float Rotate, const VECTOR3DF& vec )
{
  const MATRIX4DF t = MATRIX4DF().SetTranslate( Pos.x, Pos.y, Pos.z );
  const MATRIX4DF s = MATRIX4DF().SetScale( Scale.w, Scale.h, Scale.d );
  const MATRIX4DF r = MATRIX4DF().SetRotationXYZ( Rotate, vec );

  const MATRIX4DF world = s*r*t;
  Blt( world, model, alpha );
}


void Graphics3DRender::Blt( const MATRIX4DF& world, const ModelMQO& model, float alpha )
{
  const MATRIX4DF wvp = world * m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix();
  Graphics::IDrawCommand& Command = GetCommand();

  //  このステートは固定
  {
    Command.SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    {
      IBlendState& state = m_MQOBlendState;
      Command.SetBlendState( state.Get() );
    }

    {
      IDepthStencilState& state = m_DepthOn;
      Command.SetDepthStencilState( state.Get() );
    }

    {
      IRasterizerState& raster = m_MQORasterizer;
      Command.SetRasterizerState( raster.Get() );
    }
  }


  const MQOSCENE& Scene = model.GetScene();
  const std::vector<MQOMATERIAL>& matlist = model.GetMaterialList();
  const std::vector<boost::shared_ptr<MQOOBJECT> >& objlist = model.GetObjectList();

  for( int i=0; i<(int)objlist.size(); ++i )
  {
    const MQOOBJECT& Obj = *(objlist[i]);
    const IVertex& point = Obj.Point;
    const IVertex& color = Obj.Color;
    const IVertex& normal = Obj.Normal;
    const IVertex& uv = Obj.UV;

    Command.SetVertex( 0, point.Get(), 0, sizeof(POINT3DF) );
    Command.SetVertex( 1, color.Get(), 0, sizeof(COLOR_R32G32B32A32F) );
    Command.SetVertex( 2, normal.Get(), 0, sizeof(VECTOR3DF) );
    Command.SetVertex( 3, uv.Get(), 0, sizeof(POINT2DF) );

    for( int j=0; j<(int)Obj.Primitive.size(); ++j )
    {
      const MQOOBJECT::PRIMITIVE& prim = Obj.Primitive[j];
      const IIndex& index = prim.Index;
      const size_t size = prim.Index.GetSize();

      if( prim.MaterialNo!=MQOOBJECT::PRIMITIVE::MATERIAL_NONE )
      {
        //  マテリアルの状態から vshader, pshader, m_ShaderConstant を決める
        const MQOMATERIAL& mat = matlist[prim.MaterialNo];
        MQOShaderSetup( world, wvp, mat, alpha );
        Command.SetIndex( index.Get(), 0 );
        Command.DrawIndexed( size/2, 0, 0 );
      }else
      {
        //  マテリアルがない時は描画しない
      }
    }
  }

}



}
