/*
  ムービーの再生(コーデックは ogg theora&vorbis)
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/texture2dmemory.h"
#include"../../source/sound/pcmwriter.h"

#include"decoder.h"


using namespace Maid;

struct YUVtoRGBINFO
{
  SIZE2DI SrcSizeY;
  int SrcPitchY;
  const unt08* pSrcY;

  SIZE2DI SrcSizeUV;
  int SrcPitchUV;
  const unt08* pSrcU;
  const unt08* pSrcV;

  SIZE2DI DstSize;
  int DstPitch;
  PIXELFORMAT DstFormat;
  void* pDst;
};



template<class CONVERTER>
void YUVtoRGB( const YUVtoRGBINFO& info, const CONVERTER& conv )
{
  //  yuv の変換はここみてね
  //  http://vision.kuee.kyoto-u.ac.jp/~hiroaki/firewire/yuv.html
  //  http://d.hatena.ne.jp/Ko-Ta/20090124

  const SIZE2DI& yplane = info.SrcSizeY;
  const int ypitch = info.SrcPitchY;
  const unt08* PixelY = info.pSrcY;

  const SIZE2DI& uvplane = info.SrcSizeUV;
  const int uvpitch   = info.SrcPitchUV;
  const unt08* PixelU = info.pSrcU;
  const unt08* PixelV = info.pSrcV;

  const int bpp = GetPixelBPP(info.DstFormat);
  const int PixelPitch = bpp / 8;

  const SIZE2DI& DestSize = info.DstSize;

  //  theoraのframeサイズとテクスチャの実サイズが（ライブラリの設定で）違うことがあるので
  //  縮小する必要がある
  //  本当はフィルタかけたいけど、手間なのでしない

  for( int dst_y=0; dst_y<DestSize.h; ++dst_y )
  {
    const int y_y  = dst_y * yplane.h  / DestSize.h;
    const int uv_y = dst_y * uvplane.h / DestSize.h;

    const unt08* pSrcLineY = PixelY + ypitch  * y_y;
    const unt08* pSrcLineU = PixelU + uvpitch * uv_y;
    const unt08* pSrcLineV = PixelV + uvpitch * uv_y;
    unt08* pDstLine = (unt08*)(info.pDst) + (dst_y*info.DstPitch);

    //  X軸の移動は固定小数点で行う。8bitの精度で大丈夫...?
    const unt FIXBIT = 8;
    const unt32 delta_y_x  = ( yplane.w << FIXBIT) / DestSize.w;
    const unt32 delta_uv_x = (uvplane.w << FIXBIT) / DestSize.w;
    unt32 fix_y_x = 0;
    unt32 fix_uv_x = 0;

    for( int dst_x=0; dst_x<DestSize.w; ++dst_x )
    {
      const unt32 y_x  = fix_y_x >> FIXBIT;
      const unt32 uv_x = fix_uv_x>> FIXBIT;

      int y = pSrcLineY[y_x ];
      int u = pSrcLineU[uv_x];
      int v = pSrcLineV[uv_x];

      //  32bitギリギリなので注意
      y=(y-16 )*76608 >>8;    //Y成分を16～235→0～65535に伸縮
      u=(u-128)*74896 >>8;    //U成分をCbに変換16-240 -> -32767-32767
      v=(v-128)*74896 >>8;    //V成分をCrに変換

      const int FLOATBIT = 13;

      int r = (y*8192          +v*11485) >> FLOATBIT; //R = Y            + 1.40200V
      int g = (y*8192 -u* 2819 -v* 5850) >> FLOATBIT; //G = Y - 0.34414U - 0.71414V
      int b = (y*8192 +u*14516         ) >> FLOATBIT; //B = Y + 1.77200U
      //0～65535の閾値処理
      if( r < 0 ) { r=0; }ef( 65535<r ) { r = 65535; }
      if( g < 0 ) { g=0; }ef( 65535<g ) { g = 65535; }
      if( b < 0 ) { b=0; }ef( 65535<b ) { b = 65535; }

      CONVERTER* dst = (CONVERTER*)(pDstLine + (dst_x*PixelPitch));

      dst->SetA(0xFFFF);
      dst->SetR(r);
      dst->SetG(g);
      dst->SetB(b);

      fix_y_x += delta_y_x;
      fix_uv_x+= delta_uv_x;
    }
  }
}


void YUVtoRGB( const YUVtoRGBINFO& info )
{
  switch( info.DstFormat )
  {
  case PIXELFORMAT_R08G08B08I:    { YUVtoRGB(info,COLOR_R08G08B08I()); }break;
  case PIXELFORMAT_A08R08G08B08I: { YUVtoRGB(info,COLOR_A08R08G08B08I()); }break;
  case PIXELFORMAT_X08R08G08B08I: { YUVtoRGB(info,COLOR_X08R08G08B08I()); }break;

  case PIXELFORMAT_A02R10G10B10I: { YUVtoRGB(info,COLOR_A02R10G10B10I()); }break;
  case PIXELFORMAT_A02B10G10R10I: { YUVtoRGB(info,COLOR_A02B10G10R10I()); }break;

  case PIXELFORMAT_R05G06B05I:    { YUVtoRGB(info,COLOR_R05G06B05I()); }break;
  case PIXELFORMAT_X01R05G05B05I: { YUVtoRGB(info,COLOR_X01R05G05B05I()); }break;
  case PIXELFORMAT_A01R05G05B05I: { YUVtoRGB(info,COLOR_A01R05G05B05I()); }break;
  case PIXELFORMAT_A01B05G05R05I: { YUVtoRGB(info,COLOR_A01B05G05R05I()); }break;
  case PIXELFORMAT_A04R04G04B04I: { YUVtoRGB(info,COLOR_A04R04G04B04I()); }break;

  default:
    {
      MAID_ASSERT( true, "対応してません" );
    }break;
  }
}



class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
     return true;
  }

  void Initialize()
  {
    m_Render.Initialize();
    m_Dec.Initialize();

    m_Frame = 0;
    m_State = STATE_INITIALIZING;
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();


    switch( m_State )
    {
    case STATE_INITIALIZING:
      {
        if( m_Dec.IsInitialized() )
        {
          Decoder::FILEINFO& info = m_Info;

          m_Dec.GetFileInfo( info );

          m_Texture.Create( info.Image.EncodedSize, PIXELFORMAT_R08G08B08I );

          {
            Buffer wave;
            m_Dec.FlushPCM( 0, wave );
            m_Sound.Initialize( info.Pcm.Format, 2.0f, wave.GetPointer(0), wave.GetSize() );
          }

          m_Sound.Play();
          m_State = STATE_PLAING;
          m_Frame = 0;
          //  画像のエンコードは重いので、描画の時に行う
          //  サウンドは毎回やる
        }

      }break;

    case STATE_PLAING:
      {
        //  画像のエンコードは重いので、描画の時に行う
        //  サウンドは毎回やる
/*
        if( m_Dec.IsEnd() )
        {
          //  末端まできたら終了
      //    m_Sound.Stop();
      //    m_State = STATE_END;
        }
*/
        if( k.IsIn( Keybord::BUTTON_SPACE) )
        {
          //  スペースを押したら1秒にシーク
          m_Sound.Stop();
          const int frame = 60;
          m_Dec.Seek( double(frame) / 60.0 );
          m_Frame = frame;
          m_State = STATE_SEEKING;
        }else
        {
          Buffer wave;
          m_Dec.FlushPCM( double(m_Frame) / 60.0, wave );
          if( wave.IsCreated() )
          {
            m_Sound.Write( wave.GetPointer(0), wave.GetSize() );
          }
          ++m_Frame;
        }
      }break;

    case STATE_SEEKING:
      {
        if( !m_Dec.IsSeeking() )
        {
          //  シーク中は何もしない。
        }else
        {
          //  画像のエンコードは重いので、描画の時に行う
          //  サウンドは毎回やる
          Buffer wave;
          m_Dec.FlushPCM( double(m_Frame) / 60.0, wave );

          if( wave.IsCreated() )
          {
            m_Sound.Write( wave.GetPointer(0), wave.GetSize() );
          }
          m_Sound.Play();
          m_State = STATE_PLAING;
        }
      }break;

    case STATE_END:
      {

      }break;
    }
  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0.5f,0,0) );

    if(  m_Render.IsInitializing() ) { return ; }

    m_Render.Begin();

    switch( m_State )
    {
    case STATE_INITIALIZING:
      {

      }break;

    case STATE_PLAING:
      {
        //  時間を求める
        const double time = double(m_Frame) / 60.0;
        SPOGGDECODEDBUFFERIMAGE pImage;

        m_Dec.FlushImage( time, pImage );

        if( pImage.get()!=NULL )
        {
          UpdateTexture( pImage );
        }
        const RECT2DI rc( POINT2DI(0,0), m_Texture.GetSize() );

        m_Render.Blt( POINT2DI(0,0), m_Texture, rc, POINT2DI(0,0), 1 );
      }break;

    case STATE_SEEKING:
      {
        const RECT2DI rc( POINT2DI(0,0), m_Texture.GetSize() );

        m_Render.Blt( POINT2DI(0,0), m_Texture, rc, POINT2DI(0,0), 1 );
      }break;

    case STATE_END:
      {
        const RECT2DI rc( POINT2DI(0,0), m_Texture.GetSize() );

        m_Render.Blt( POINT2DI(0,0), m_Texture, rc, POINT2DI(0,0), 1 );

      }break;
    }


    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  void UpdateTexture( const SPOGGDECODEDBUFFERIMAGE& pImage )
  {
    Maid::Texture2DMemory::SURFACEINFO dst;
    m_Texture.Map( dst );

    YUVtoRGBINFO info;

    info.SrcSizeY = pImage->SizeY;
    info.SrcPitchY = pImage->PitchY;
    info.pSrcY = &(pImage->BitsY[0]);
    info.SrcSizeUV = pImage->SizeUV;
    info.SrcPitchUV = pImage->PitchUV;
    info.pSrcU = &(pImage->BitsU[0]);
    info.pSrcV = &(pImage->BitsV[0]);

    info.DstSize = m_Texture.GetRealSize();
    info.DstPitch = dst.Pitch;
    info.DstFormat = m_Texture.GetRealFormat();
    info.pDst = dst.pData;

    YUVtoRGB( info );

    m_Texture.Unmap();
  }


private:
  Graphics2DRender  m_Render;


  int m_Frame;

  enum STATE
  {
    STATE_INITIALIZING, //  初期化中
    STATE_PLAING,       //  再生中
    STATE_SEEKING,      //  シーク中
    STATE_END,         //  再生終了
  };

  STATE m_State;

  Decoder::FILEINFO m_Info;

  Texture2DMemory m_Texture;
  PCMWriter   m_Sound;
  Decoder m_Dec;
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new MyApp) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

