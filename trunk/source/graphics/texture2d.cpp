#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"
#include"../storage/filewriter.h"

#include"surface.h"
#include"transiter.h"

inline int ExtendSize( int src )
{
  int now = 1;
  for( int i=0; i<20; ++i ) //  2^20
  {
    if( src<=now ) { return now; }
    now *= 2;
  }

  return now;
}

namespace Maid
{
  namespace KEEPOUT
  {
    void tex2dFunction::Execute( const IJobInput& Input, IJobOutput& Output )
    {
      const tex2dInput&  in  = static_cast<const tex2dInput&>(Input);
      tex2dOutput& out = static_cast<tex2dOutput&>(Output);

      std::vector<Surface> ImageSurface;
      {
        const FUCTIONRESULT ret = LoadImage( in.FileName, ImageSurface );
        if( FUCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("失敗") << in.FileName ) return ; }
      }
      //  これで ImageSurface に読み込んだデータが入った。
      //  次に各種設定から作成するテクスチャフォーマットを求める

      const PIXELFORMAT DstFormat = in.Core->FindFormatTexture2D( ImageSurface[0].GetPixelFormat(), false );
      if( DstFormat==PIXELFORMAT_NONE )
      {
        MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << ImageSurface[0].GetPixelFormat() );
        return ; 
      }

      const SIZE2DI ImgSize = ImageSurface[0].GetSize();
      SIZE2DI TextureSize;
      SIZE2DI RealSize;

      in.Core->CalcTextureSize( ImgSize, RealSize, TextureSize );


      //  求まったのでテクスチャを実際に作る
      Graphics::SPTEXTURE2D pTexture;
      {
        Graphics::CREATERETEXTURE2DPARAM param;

        param.Size = TextureSize;
        param.MipLevels = 0;  //  mipmapは自動
        param.ArraySize = 1;
        param.Format = DstFormat;
        param.Usage = Graphics::RESOURCEUSAGE_DEFAULT;
        param.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;
        param.CPUAccessFlags = Graphics::CPUACCESSFLAG_WRITE;

        pTexture = in.Core->GetDevice()->CreateTexture2D( param, NULL );
        if( pTexture.get()==NULL ) {
          MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") << in.FileName );
          return ; 
        }
      }

      //  元データとテクスチャがそろったので、流し込みまくる
      const int     LevelMax = pTexture->GetParam().MipLevels + 1;
      SIZE2DI NowRealSize  = RealSize;
      SIZE2DI NowTextureSize  = TextureSize;

      for( int level=0; level<LevelMax; ++level )
      {
        const std::vector<Surface>& surf = ImageSurface;

        const SIZE2DI& SrcSize = level<(int)surf.size()? surf[level].GetSize() : surf.back().GetSize();
        const Surface& src = level<(int)surf.size()? surf[level] : surf.back();

        const SIZE2DI& DstSize = NowRealSize;

        Surface dst;
        dst.Create( NowTextureSize, DstFormat );
//        Transiter::Fill( COLOR_R32G32B32A32F(0,0,0,0), dst );

        if( SrcSize.w <= DstSize.w && SrcSize.h <= DstSize.h )
        { //  画像より作成するサイズの方が大きいなら普通に転送
          const RECT2DI src_rc( POINT2DI(0,0), SrcSize );
          const RECT2DI dst_rc( POINT2DI(0,0), SrcSize );
          Transiter::Copy(src, src_rc, dst, dst_rc);
        }else
        { //  小さいなら縮小して転送
          #pragma  COMPILERMSG( "TODO:バイキュービックとかフィルタ選べるとカッコイイなぁ。" )

          const RECT2DI src_rc( POINT2DI(0,0), SrcSize );
          const RECT2DI dst_rc( POINT2DI(0,0), DstSize );

          Transiter::Average( src, src_rc, dst, dst_rc );
        }

        {
          //  塗りつぶしてない部分を隅っこの色で塗る
          for( int y=0; y<DstSize.h; ++y )
          {
            for( int x=DstSize.w; x<NowTextureSize.w; ++x ) 
            {
              const POINT2DI s(DstSize.w-1,y);
              const POINT2DI d(x,y);
              dst.SetPixel( d, dst.GetPixel(s) );
            }
          }
          for( int y=DstSize.h; y<NowTextureSize.h; ++y )
          {
            const int w = dst.GetWidthByte();

            ::memcpy( dst.GetLinePTR(y), dst.GetLinePTR(DstSize.h-1), w );
          }
        }

        {
          Graphics::SPDRAWCOMMANDEXECUTE pCommand = in.Core->GetDevice()->GetDrawCommandExecute();
          Graphics::MAPPEDRESOURCE target;
          pCommand->ResourceMap( pTexture, level, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, target );

          unt08* p = (unt08*)target.pData;
          const int pitch = dst.GetSize().w * GetPixelBPP(dst.GetPixelFormat())/8;
          for( int y=0; y<NowTextureSize.h; ++y )
          {
            ::memcpy( p, dst.GetLinePTR(y), pitch );
            p += target.Pitch;
          }

          pCommand->ResourceUnmap( pTexture, level );
        }

        NowRealSize.w /= 2;
        NowRealSize.h /= 2;
        NowTextureSize.w /= 2;
        NowTextureSize.h /= 2;

        if( NowRealSize.w < 1 ) { NowRealSize.w = 1; }
        if( NowRealSize.h < 1 ) { NowRealSize.h = 1; }
        if( NowTextureSize.w < 1 ) { NowTextureSize.w = 1; }
        if( NowTextureSize.h < 1 ) { NowTextureSize.h = 1; }
      }

      Graphics::SPMATERIAL pMaterial;
      {
        pMaterial = in.Core->GetDevice()->CreateMaterial( pTexture, NULL );
        if( pMaterial.get()==NULL )
        {
          MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") << in.FileName );
          return ; 
        }
      }
   
      out.pTexture  = pTexture;
      out.pMaterial = pMaterial;
      out.ImageSize   = ImgSize;
      out.CreateSize  = RealSize;
      out.TextureSize = TextureSize;
      out.ImageFormat = ImageSurface[0].GetPixelFormat();
    }

    FUCTIONRESULT tex2dFunction::LoadImage( const String& filename, std::vector<Surface>& dst )
    {
      FileReaderSync  hFile;

      {
        const bool ret = hFile.Open( filename );
        if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") ); return FUCTIONRESULT_ERROR; }
      }

      std::vector<unt08> image( hFile.GetSize() );

      {
        const size_t ret = hFile.Read( &(image[0]), image.size() );
        if( ret!=image.size() ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") ); return FUCTIONRESULT_ERROR; }
      }


      dst.reserve( 13 );  //  4096x4096 - 1x1 までの配列が 13 なのでそれぐらいあればいいでしょう
      { //  ファイルフォーマットを調べて読み込み開始

        if( Bitmap::Check( image ) ) 
        {
          dst.resize(1);
          const FUCTIONRESULT ret = Bitmap::Load( image, dst[0] ); 
          if( FUCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("bitmap失敗") ); return FUCTIONRESULT_ERROR; }
          GenerateSublevel( dst );
        }
        ef( PNG::Check( image ) ) 
        {
          dst.resize(1);
          const FUCTIONRESULT ret = PNG::Load( image, dst[0] ); 
          if( FUCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("png失敗") ); return FUCTIONRESULT_ERROR; }
          GenerateSublevel( dst );
        }
        ef( Jpeg::Check( image ) ) 
        {
          dst.resize(1);
          const FUCTIONRESULT ret = Jpeg::Load( image, dst[0] ); 
          if( FUCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("jpeg失敗") ); return FUCTIONRESULT_ERROR; }
          GenerateSublevel( dst );
        }

        #pragma  COMPILERMSG( "TODO:のこり jpeg,psd,gif,tga...ぐらいか？" )
      }

      return FUCTIONRESULT_OK;
    }


    void tex2dFunction::GenerateSublevel( std::vector<Surface>& target )
    {
      MAID_ASSERT( target[0].IsEmpty(), "最初のレベルは作ってください" );

      int sublevel = 0;

      {
        const SIZE2DI& size = target[0].GetSize();
        int p = std::max( size.w, size.h );
        while( true )
        {
          p >>= 1;
          if( p==0 ) { break; }
          ++sublevel;
        }
      }

      target.resize( sublevel + 1 );

      for( int i=0; i<sublevel; ++i )
      {
        const Surface& src = target[i];
        Surface& dst = target[i+1];

        const SIZE2DI src_size = src.GetSize();
        const SIZE2DI dst_size = SIZE2DI(std::max(src_size.w/2,1), std::max(src_size.h/2,1) );

        dst.Create( dst_size, src.GetPixelFormat() );

        for( int dst_y=0; dst_y<dst_size.h; ++dst_y ) {
          for( int dst_x=0; dst_x<dst_size.w; ++dst_x ) { 

            const int x = dst_x*2;
            const int y = dst_y*2;

            float r=0;
            float g=0;
            float b=0;
            float a=0;
            int count = 0;

            if( x<src_size.w && y<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x  ,y  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }

            if( x+1<src_size.w && y<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x+1  ,y  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }
            if( x <src_size.w && y+1<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x   ,y+1  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }
            if( x+1 <src_size.w && y+1<src_size.h )
            {
              const COLOR_R32G32B32A32F c = src.GetPixel( POINT2DI(x+1   ,y+1  ) );
              r += c.GetR(); g += c.GetG(); b += c.GetB(); a += c.GetA();
              ++count;
            }

            const float inv_div = 1.0f / float(count);
            r *= inv_div; g *= inv_div; b *= inv_div; a *= inv_div;
            dst.SetPixel( POINT2DI(dst_x,dst_y), COLOR_R32G32B32A32F(r,g,b,a) );
          }
        }
      }
    }
  }


Texture2D::CACHE::INFOMAP  Texture2D::CACHE::s_InfoMap;
ThreadMutex  Texture2D::CACHE::s_Mutex;

void Texture2D::LoadFile( const String& FileName )
{
  Texture2DBase::Clear();
  if( FileName.empty() ) { return ; }
  m_Cache.Start( KEEPOUT::tex2dInput(FileName, GlobalPointer<GraphicsCore>::Get() ) );
}


bool Texture2D::IsLoading()const
{
  //  ポインタが存在してるならすでに作ってる
  if( IsSetupped() ) { return false; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const KEEPOUT::tex2dOutput& out = m_Cache.GetOutput();

  const_cast<Texture2D*>(this)->Setup( out.pTexture, out.pMaterial, out.ImageSize, out.CreateSize, out.ImageFormat );

  return false;

}

const SIZE2DI& Texture2D::GetImageSize()const
{
  return m_Cache.GetOutput().ImageSize;
}

bool Texture2D::IsEmpty() const
{
  return m_Cache.IsEmpty();
}


}