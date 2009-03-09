#include"texture2dmemory.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{

void Texture2DMemory::Create( const SIZE2DI& Size, PIXELFORMAT fmt )
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  SIZE2DI RealSize;
  SIZE2DI TextureSize;

  pCore->CalcTextureSize( Size, RealSize, TextureSize );

  if( Size.w <= RealSize.w && Size.h <= RealSize.h )
  { //  画像より作成するサイズの方が大きいなら引数どおりの大きさにする
    RealSize = Size;
  }

  const PIXELFORMAT DstFormat = pCore->FindFormatTexture2D( fmt, false );
  if( DstFormat==PIXELFORMAT_NONE )
  {
    MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << fmt );
    return ; 
  }


  Graphics::SPTEXTURE2D pTexture;

  {
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = TextureSize;
    param.MipLevels = 0;
    param.ArraySize = 0;
    param.Format = DstFormat;
    param.Usage = Graphics::RESOURCEUSAGE_IMMUTABLE;
    param.BindFlags = Graphics::RESOURCEUSAGE_DYNAMIC;
    param.MiscFlags = Graphics::RESOURCEMISCFLAG_GENERATE_MIPS;

    pTexture = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pTexture.get()==NULL )
    {
      MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
      return ; 
    }
  }


  Graphics::SPMATERIAL pMaterial;
  {
    Graphics::CREATEMATERIALPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;

//    pMaterial = pCore->GetDevice()->CreateMaterial( pTexture, param );
    pMaterial = pCore->GetDevice()->CreateMaterial( pTexture, NULL );
    if( pMaterial.get()==NULL )
    {
      MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
      return ; 
    }
  }

  Setup( pTexture, pMaterial, Size, RealSize, fmt );

}

void Texture2DMemory::Map( SURFACEINFO& out )
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  Graphics::SPDRAWCOMMANDEXECUTE pCommand = pCore->GetDevice()->GetDrawCommandExecute();

  ITexture2D& tex = (*this);

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;
  pCommand->ResourceMap( tex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );

  out.pData = map.pData;
  out.Pitch = map.Pitch;
}

void Texture2DMemory::Unmap()
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  Graphics::SPDRAWCOMMANDEXECUTE pCommand = pCore->GetDevice()->GetDrawCommandExecute();

  ITexture2D& tex = (*this);

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;
  pCommand->ResourceUnmap( tex.Get(), sub );
}


}