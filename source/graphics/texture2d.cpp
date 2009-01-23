#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"

#include"surface.h"
#include"transiter.h"

namespace Maid
{
  namespace KEEPOUT
  {
  void tex2dFunction::Execute( const IJobInput& Input, IJobOutput& Output )
  {
    const tex2dInput&  in  = static_cast<const tex2dInput&>(Input);
    tex2dOutput& out = static_cast<tex2dOutput&>(Output);

    Surface src;

    {
      FileReaderSync  hFile;

      {
        const bool ret = hFile.Open( in.FileName );
        if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") << in.FileName ); return ; }
      }

      std::vector<unt08> image( hFile.GetSize() );

      {
        const bool ret = hFile.Read( &(image[0]), image.size() );
        if( ret ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") << in.FileName ); return ; }
      }

      { //  ファイルフォーマットを調べて読み込み開始
        if( Bitmap::Check( image ) ) { Bitmap::Load( image, src ); }
        ef( PNG::Check( image ) ) { PNG::Load( image, src ); }

         #pragma  COMPILERMSG( "TODO:のこり jpeg,psd,gif,tga...ぐらいか？" )
      }

      if( src.IsEmpty() ) { MAID_WARNING( MAIDTEXT("展開に失敗") << in.FileName ); return ; 
      }
    }

    //  これで src に読み込んだデータが入った。
    //  次はハードウェアで使えるフォーマットに変換する

    const PIXELFORMAT DstFormat = in.Core->FindFormatTexture2D( src.GetPixelFormat(), false );
    if( DstFormat==PIXELFORMAT_NONE )
    {
      MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << src.GetPixelFormat() << in.FileName );
      return ; 
    }

    const SIZE2DI ImgSize = src.GetSize();
    SIZE2DI RealSize;
    SIZE2DI TextureSize;

    in.Core->CalcTextureSize( ImgSize, RealSize, TextureSize );

    Surface dst;
    dst.Create( TextureSize, DstFormat );
    Transiter::Fill( COLOR_R32G32B32A32F(0,0,0,0), dst );

    {
      if( ImgSize.w <= RealSize.w && ImgSize.h <= RealSize.h )
      { //  画像より作成するサイズの方が大きいなら普通に転送or拡大
        //  現状は等倍転送のみ
        #pragma  COMPILERMSG( "TODO:超解像度で拡大できるとカッコイイかなぁ。" )
        Transiter::Copy(src, dst);
        RealSize = ImgSize;
      }else
      { //  小さいなら縮小して転送
        #pragma  COMPILERMSG( "TODO:バイキュービックとかフィルタ選べるとカッコイイなぁ。" )

        const RECT2DI src_rc( POINT2DI(0,0), src.GetSize() );
        const RECT2DI dst_rc( POINT2DI(0,0), RealSize );

        Transiter::Average( src, src_rc, dst, dst_rc );
      }
    }

    //  これで作るテクスチャが完成
    //  あとはデバイスにやってもらう
    Graphics::SPTEXTURE2D pTexture;

    {
      Graphics::CREATERETEXTURE2DPARAM param;

      param.Size = TextureSize;
      param.MipLevels = 0;
      param.ArraySize = 0;
      param.Format = DstFormat;
      param.Usage = Graphics::RESOURCEUSAGE_IMMUTABLE;
      param.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;
      param.MiscFlags = Graphics::RESOURCEMISCFLAG_GENERATE_MIPS;

      Graphics::SUBRESOURCE Data;
      Data.pData = dst.GetLinePTR(0);
      Data.Pitch = dst.GetPitch();
      pTexture = in.Core->GetDevice()->CreateTexture2D( param, &Data );
      if( pTexture.get()==NULL )
      {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") << in.FileName );
        return ; 
      }
    }


    Graphics::SPMATERIAL pMaterial;
    {
      Graphics::CREATEMATERIALPARAM param;
      param.Format = DstFormat;
      param.Dimension =  Graphics::CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;

      pMaterial = in.Core->GetDevice()->CreateMaterial( pTexture, param );
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
  }

}


Texture2D::CACHE::INFOMAP  Texture2D::CACHE::s_InfoMap;

void Texture2D::LoadFile( const String& FileName )
{
  Texture2DBase::Clear();
  m_Cache.Start( KEEPOUT::tex2dInput(FileName, GlobalPointer<GraphicsCore>::Get() ) );
}


bool Texture2D::IsLoading()const
{
  //  ポインタが存在してるならすでに作ってる
  if( IsSetupped() ) { return false; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  const KEEPOUT::tex2dOutput& out = m_Cache.GetOutput();

  const_cast<Texture2D*>(this)->Setup( out.pTexture, out.pMaterial, out.ImageSize, out.CreateSize );

  return false;

}

const SIZE2DI& Texture2D::GetImageSize()const
{
  return m_Cache.GetOutput().ImageSize;
}


}