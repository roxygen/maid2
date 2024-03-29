﻿#include"texture2d.h"
#include"imagefile.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/thread.h"
#include"../auxiliary/debug/warning.h"
#include"../storage/filewriter.h"

#include"surface.h"
#include"transiter.h"
#include"texture2dlocal.h"


namespace Maid
{
  namespace KEEPOUT
  {
    FUNCTIONRESULT tex2dFunction::LoadImageFile( const String& filename, std::vector<SurfaceInstance>& dst )
    {
      FileReaderSync  hFile;

      {
        const bool ret = hFile.Open( filename );
        if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") ); return FUNCTIONRESULT_ERROR; }
      }

      std::vector<unt08> image( hFile.GetSize() );

      {
        const size_t ret = hFile.Read( &(image[0]), image.size() );
        if( ret!=image.size() ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") ); return FUNCTIONRESULT_ERROR; }
      }

      { //  ファイルフォーマットを調べて読み込み開始

        if( Bitmap::Check( image ) ) 
        {
          dst.resize(1);
          const FUNCTIONRESULT ret = Bitmap::Load( image, dst[0] ); 
          if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("bitmap失敗") ); return FUNCTIONRESULT_ERROR; }
        }
        else if( PNG::Check( image ) ) 
        {
          dst.resize(1);
          const FUNCTIONRESULT ret = PNG::Load( image, dst[0] ); 
          if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("png失敗") ); return FUNCTIONRESULT_ERROR; }
        }
        else if( Jpeg::Check( image ) ) 
        {
          dst.resize(1);
          const FUNCTIONRESULT ret = Jpeg::Load( image, dst[0] ); 
          if( FUNCTIONRESULT_FAILE(ret) ) { MAID_WARNING( MAIDTEXT("jpeg失敗") ); return FUNCTIONRESULT_ERROR; }
        }

        #pragma  COMPILERMSG( "TODO:のこり psd,gif,tga...ぐらいか？" )
      }

      return FUNCTIONRESULT_OK;
    }






    FUNCTIONRESULT tex2dFunction::LoadImage( const tex2dInput::CREATECONFIG& Element, std::vector<SurfaceInstance>& dst )
    {
      //  まずはファイル名を分解するところから
      //  一文字目が< で始まってるなら、合成ファイルと判断する

      const std::map<String,String>::const_iterator IteColor = Element.find(ELEMENT_COLOR);
      const std::map<String,String>::const_iterator IteAlpha = Element.find(ELEMENT_ALPHA);

      //  カラーとアルファが両方ある場合は
      //  カラーのビット数に合わせてアルファを合成する
      if( IteColor!=Element.end() && IteAlpha!=Element.end() )
      {
        std::vector<SurfaceInstance> col;
        std::vector<SurfaceInstance> alp;

        LoadImageFile( IteColor->second, col );
        LoadImageFile( IteAlpha->second, alp );

        MAID_ASSERT( col.size()!=alp.size(), "レイヤの数があいません" );

        const size_t count = col.size();
        dst.resize( count );

        for( size_t i=0; i<count; ++i )
        {
          SurfaceInstance& src_c = col[i];
          SurfaceInstance& src_a = alp[i];
          SurfaceInstance& d = dst[i];
          const SIZE2DI     size = src_c.GetSize();
          const PIXELFORMAT fmt  = ConvertAlphaFormat(src_c.GetPixelFormat());

          MAID_ASSERT( src_a.GetPixelFormat()!=PIXELFORMAT_P08X08R08G08B08I && src_a.GetPixelFormat()!=PIXELFORMAT_P08A08R08G08B08I,
            "アルファ画像はインデックス付でお願いします" );

          d.Create( size, fmt );

          Transiter::CopyAndSetAlpha( src_c, src_a, d );
        }
      }
      else if( IteColor!=Element.end() )
      {
        //  カラーだけの場合
        LoadImageFile( IteColor->second, dst );
      }

      return FUNCTIONRESULT_OK;
    }

  }
}