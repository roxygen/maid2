#include"graphicsrender.h"
#include"customvertextemplate.h"




namespace Maid
{

  /*!
      @class  GraphicsRender graphicsrender.h
      @brief  標準的な描画ルーチンを集めたクラス
  \n          ここに書いてある感じで各種ゲームに追加実装するのが吉
  */

GraphicsRender::GraphicsRender()
  :m_State(STATE_EMPTY)
  ,m_BltState(BLENDSTATE_NONE)
{

}

GraphicsRender::~GraphicsRender()
{

}


//! このクラスを使えるように初期化
/*!
    この関数を呼んだあと、IsInitializing() がfalseを返すまで待機すること
 */
void GraphicsRender::Initialize()
{
  const GraphicsCore& Core = *(GlobalPointer<GraphicsCore>::Get());

  {
    //  1頂点あたり128バイトも使わないでしょう
    m_CommonVertex.Create( 128*4 );
  }


  {
    {
      const String vs = MAIDTEXT("0100");
      m_FillVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_FillLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_FillPixelShader.Create( MAIDTEXT("0200") );
    }
  }

  {
    {
      const String vs = MAIDTEXT("0101");

      m_BltVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_BltLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_BltPixelShader210.Create( MAIDTEXT("0210") );
      m_BltPixelShader211.Create( MAIDTEXT("0211") );
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
    param.DepthTest = false;
    m_DepthOff.Create( param );
  }

  {
    m_BlendState.resize( 8 );

    m_BlendState[0].Create( BlendState::TEMPLATE_NORMAL );
    m_BlendState[1].Create( BlendState::TEMPLATE_ALPHA );
    m_BlendState[2].Create( BlendState::TEMPLATE_ADD );
    m_BlendState[3].Create( BlendState::TEMPLATE_SUB );
    m_BlendState[4].Create( BlendState::TEMPLATE_MUL );
    m_BlendState[5].Create( BlendState::TEMPLATE_SCREEN );
    m_BlendState[6].Create( BlendState::TEMPLATE_DARKEN );
    m_BlendState[7].Create( BlendState::TEMPLATE_LIGHTTEN );
  }

  {
    Graphics::SAMPLERSTATEPARAM state;
    state.MinFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;
    state.MagFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;
    state.MipFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;

    m_SamplerLinar.Create( state );

    state.MinFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;
    state.MagFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;
    state.MipFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;

    m_SamplerPoint.Create( state );
  }

  {
    m_VirtualScreenSize = Core.GetScreenMode().Format.Size;
  }
  {
    m_FontManager.Initialize();
    m_FontManager.SetCacheSize(1024);
  }

  m_BeginCount = 0;
  m_pDrawCommand = Core.GetDevice()->GetDrawCommandExecute();
  m_State = STATE_LOADING;
}

//! 描画開始の合図
/*!
    この呼び出し以降、graphicsrender2,3,4,.cppの関数が使用できます。
    呼び出し前は ClearRenderTarget(), ClearDepth() のみ使用できます
 */
void GraphicsRender::Begin()
{
  if( m_BeginCount!=0 ) { return ; }
  ++m_BeginCount;
  m_pDrawCommand->Begin();
  SetDefaultSetting();
}

//! 描画終了の合図
/*!
    この呼び出し以降、graphicsrender2,3,4,.cppの関数が使用できません
 */
void GraphicsRender::End()
{
  if( m_BeginCount==0 ) { return ; }
  m_pDrawCommand->End();
  m_FontManager.Garbage();
  --m_BeginCount;
}

//! 設定されている RenderTarget を塗りつぶす
/*!
    SLI機能(Scalable Link Interface)を使うには、この呼び出しが必須らしいです。
    原理を考えれば当たり前ですね。

    @param  Col [i ]  塗りつぶす色
 */
void GraphicsRender::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}

//! 設定されている DepthStencil のうち、Depthを塗りつぶす
/*!
    普通は 1.0f でしょうね。

    @param  Depth [i ]  設定するdepth値
 */
void GraphicsRender::ClearDepth( float Depth )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTH, Depth, 0 );
}

//! 設定されている DepthStencil のうち、Stencilを塗りつぶす
/*!
    普通は 0xFF でしょうね。

    @param  Stencil [i ]  設定する stencil値
 */
void GraphicsRender::ClearStencil( unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_STENCIL, 0, Stencil );
}


//! 設定されている DepthStencil を塗りつぶす
/*!
    普通は 1.0f、0xFF でしょうね。

    @param  Depth   [i ]  設定する depth値
    @param  Stencil [i ]  設定する stencil値
 */
void GraphicsRender::ClearDepthStencil( float Depth, unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTHSTENCIL, Depth, Stencil );
}

//! 仮想サイズの変更
/*!
    ゲーム中の描画に使っている座標をすべて仮想サイズで決めることで
    解像度の違いをすべて吸収できるようになります。

    @param  Size [i ] 新しいサイズ
 */
void GraphicsRender::SetVirtualScreenSize( const SIZE2DI& Size )
{
  m_VirtualScreenSize = Size;
}

//! RenderTargetの設定
/*!
    引数のとおり、DepthStencilはないものとして設定されます

    @param  Target [i ]  設定する Target
 */
void GraphicsRender::SetRenderTarget( const RenderTargetBase& Target )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Graphics::SPDEPTHSTENCIL() );

  const RECT2DI rc = Target.CalcDrawArea();

//  const Graphics::VIEWPORT view( RECT2DI(POINT2DI(0,0),Target.GetSize()), 0, 1 );
  const Graphics::VIEWPORT view( rc, 0, 1 );


  m_pDrawCommand->SetViewPort( view );
}

//! RenderTargetの設定
/*!
    @param  Target [i ]  設定する Target
    @param  Depth  [i ]  設定する DepthStencil
 */
void GraphicsRender::SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Depth.Get() );

  const RECT2DI rc = Target.CalcDrawArea();
//  const Graphics::VIEWPORT view( RECT2DI(POINT2DI(0,0),Target.GetSize()), 0, 1 );
  const Graphics::VIEWPORT view( rc, 0, 1 );

  m_pDrawCommand->SetViewPort( view );
}


//! 初期化中か？
/*!
    @return   初期化中なら true
              まだなら false
 */
bool GraphicsRender::IsInitializing() const
{
  if( m_State!=STATE_LOADING ) { return false; }


  //  メンバが読み込み中なら当然 true
  if( IsMemberLoading() ) { return true; }

  //  読み込み終わったので、残りの処理を行う

  const_cast<GraphicsRender*>(this)->m_State = STATE_WORKING;
  const_cast<GraphicsRender*>(this)->SetDefaultSetting();
  return false;
}


//! 最低限の設定を行う
/*!
    このクラスの描画周りで困ったらこれを呼んでリセットする感じ
 */
void GraphicsRender::SetDefaultSetting()
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );

  {
    IDepthStencilState& state = m_DepthOff;
    m_pDrawCommand->SetDepthStencilState( state.Get() );
  }
  SetBlendState( BLENDSTATE_ALPHA );
  SetFilterState( TEXTUREFILTER_LINEAR );
  IRasterizerState& raster = m_Rasterizer;
  m_pDrawCommand->SetRasterizerState( raster.Get() );
}


//! ２Ｄの描画モードを変更する
/*!
    @param  State [i ]  描画モード
 */
void GraphicsRender::SetBlendState( BLENDSTATE State )
{
  MAID_ASSERT( IsInitializing(), "初期化中は呼ばないこと" );
  IBlendState& state = m_BlendState[State];

  m_pDrawCommand->SetBlendState( state.Get() );
  m_BltState = State;
}

void GraphicsRender::SetFilterState( TEXTUREFILTER filter )
{
  ISamplerState& state = m_SamplerPoint;

  switch( filter )
  {
  case TEXTUREFILTER_POINT:  { state = m_SamplerPoint; }break;
  case TEXTUREFILTER_LINEAR: { state = m_SamplerLinar; }break;
  }

  m_pDrawCommand->PSSetSamplerState( 0, state.Get() );
  m_pDrawCommand->PSSetSamplerState( 1, state.Get() );

}


bool GraphicsRender::IsMemberLoading() const
{
  //  読み込みするメンバーがいるか？
  return m_FillVertexShader.IsCompiling()
    || m_FillLayout.IsCompiling()
    || m_FillPixelShader.IsCompiling()
    || m_BltLayout.IsCompiling()
    || m_BltVertexShader.IsCompiling()
    || m_BltPixelShader210.IsCompiling()
    || m_BltPixelShader211.IsCompiling()
    ;
}



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
  const float TranceX = (float(Pos.x-screen.w/2)-0.5f) / float(screen.w/2);
  const float TranceY = (float(Pos.y-screen.h/2)-0.5f) / float(screen.h/2);

  m.Translate = MATRIX4DF().SetScale(1,aspect,1)
              * MATRIX4DF().SetTranslate(TranceX,TranceY,0);

}

//! モデルデータから頂点を作成する
/*!
    @param	Model   [i ]	基本モデルデータ
    @param	Mat     [i ]	作用させる行列
    @param	pVertex [ o]	出力先
    @param	Stride  [i ]	pVertex の１頂点あたりの大きさ(バイト単位)
 */
void GraphicsRender::SetupVertex( const VECTOR4DF* Model, const MATRIX4DF& Mat, void* pVertex, int Stride )
{
  unt08* p = (unt08*)pVertex;
  for( int i=0; i<4; ++i )
  {
    const VECTOR4DF r   = Model[i] * Mat;
    POINT3DF* pos = (POINT3DF*)p;
    pos->x = r.x;
    pos->y = -r.y;    //  ←値を反転させてるの、なんでだっけな？　３Ｄと２Ｄの座標系が反転してるから？
//    pos->z = r.z;
    pos->z = 0; //  2Dだし r.z の値はいらない（クリップされてしまうというのもある）

    p += Stride;
  }
}

//! このクラスで使われている共通の VertexBuffer に流し込む
/*!
    @param	pVertex [i ]	流し込むデータ
    @param	size    [i ]	データの大きさ（バイト単位）
    @param	Stride  [i ]	１頂点あたりの大きさ（バイト単位）
 */
void GraphicsRender::SetCommonVertex( void* pVertex, int size, int Stride )
{
  const IVertex& vertex = m_CommonVertex;
  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;
  pCommand->ResourceMap( vertex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, map );
  memcpy( map.pData, pVertex, size );
  pCommand->ResourceUnmap( vertex.Get(), sub );

  pCommand->SetVertex( 0, vertex.Get(), 0, Stride );

}

}