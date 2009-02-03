/*
  ムービーの再生(コーデックは ogg theora&vorbis)
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/texture2dmemory.h"

#include"decoder.h"


using namespace Maid;



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
    switch( m_State )
    {
    case STATE_INITIALIZING:
      {
        if( m_Dec.IsInitialized() )
        {
          Decoder::FILEINFO& info = m_Info;

          m_Dec.GetFileInfo( info );

          m_Texture.Create( info.Image.EncodedSize, PIXELFORMAT_R08G08B08I );

          m_State = STATE_PLAING;
          m_Frame = 0;
        }

      }break;

    case STATE_PLAING:
      {
        //  画像のエンコードは重いので、描画の時に行う
        //  サウンドは毎回やる

        ++m_Frame;
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

      //  MAID_WARNING( "gettime:" << float(time) );
        m_Dec.FlushImage( time, pImage );

        if( pImage.get()!=NULL )
        {
          UpdateTexture( pImage );
        }
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

    //  yuv の変換はここみてね
    //  http://vision.kuee.kyoto-u.ac.jp/~hiroaki/firewire/yuv.html
    //  http://d.hatena.ne.jp/Ko-Ta/20090124

    const SIZE2DI& yplane = pImage->SizeY;
    const int ypitch = pImage->PitchY;
    const unt08* PixelY = &(pImage->BitsY[0]);

    const SIZE2DI& uvplane = pImage->SizeUV;
    const int uvpitch = pImage->PitchUV;
    const unt08* PixelU = &(pImage->BitsU[0]);
    const unt08* PixelV = &(pImage->BitsV[0]);

    const int bpp = GetPixelBPP(m_Texture.GetRealFormat());
    const int PixelPitch = bpp / 8;


    //  いろいろパフォーマンスがいりそうなので、直接メモリに書き込む
    for( int y_y=0; y_y< yplane.h; ++y_y )
    {
      const int uv_y = y_y * uvplane.h / yplane.h ;

      const unt08* pSrcPixelY = PixelY + ypitch  * y_y;
      const unt08* pSrcPixelU = PixelU + uvpitch * uv_y;
      const unt08* pSrcPixelV = PixelV + uvpitch * uv_y;
      unt08* pDstPixel = (unt08*)(dst.pData) + (y_y*dst.Pitch);

      const unt32 delta_x = (uvplane.w << 8) / yplane.w;
      unt32 fix_uv_x = 0;

      for( int y_x=0; y_x<yplane.w; ++y_x )
      {
        const unt32 uv_x = fix_uv_x >> 8;

        int y = pSrcPixelY[y_x ];
        int u = pSrcPixelU[uv_x];
        int v = pSrcPixelV[uv_x];

        y=(y-16)*298 >> 8;    //Y成分を16～235→0～256に伸縮
        u=(u-128)*291 >>8;    //U成分をCbに変換
        v=(v-128)*291 >>8;    //V成分をCrに変換
        int r = (y*256        +v*358) >> 8; //R = Y            + 1.40200V
        int g = (y*256 -u*88  -v*183) >> 8; //G = Y - 0.34414U - 0.71414V
        int b = (y*256 +u*454       ) >> 8; //B = Y + 1.77200U
        //0～255の閾値処理
        if( r < 0 ) { r=0; }ef( 255<r ) { r = 255; }
        if( g < 0 ) { g=0; }ef( 255<g ) { g = 255; }
        if( b < 0 ) { b=0; }ef( 255<b ) { b = 255; }

        unt08* dst = pDstPixel + (y_x*PixelPitch);

        dst[0] = b;
        dst[1] = g;
        dst[2] = r;
        fix_uv_x += delta_x;
      }
    }
    m_Texture.Unmap();
  }


private:
  Graphics2DRender  m_Render;


  int m_Frame;

  enum STATE
  {
    STATE_INITIALIZING, //  初期化中
    STATE_PLAING,       //  再生中
  };

  STATE m_State;

  Decoder::FILEINFO m_Info;

  Texture2DMemory m_Texture;
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

