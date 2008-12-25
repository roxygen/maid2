#include"fontdevice.h"
#include"surfacedib.h"

//  フォントの列挙
// http://msdn.microsoft.com/ja-jp/library/cc428521.aspx


namespace Maid
{

static const char* s_DEFAULTFONTNAME = "ＭＳ ゴシック";

static int CALLBACK EnumFontFamExProc(
      CONST LOGFONT* lpelfe ,
			CONST TEXTMETRIC* lpntme ,
			DWORD FontType, LPARAM lParam)
{
  std::vector<IFontDevice::FONTINFO>* pInfoList = (std::vector<IFontDevice::FONTINFO>*)lParam;

  //  縦書きはスルー
  if( lpelfe->lfFaceName[0]=='@' ) { return TRUE; }

  IFontDevice::FONTINFO info;
  info.Name = String::ConvertUNICODEtoMAID(lpelfe->lfFaceName);

  //  デフォルトは最初に追加されてるのでパス
  if( info.Name==MAIDTEXT(s_DEFAULTFONTNAME) ) { return TRUE; }

  pInfoList->push_back( info );
  return TRUE;
}

void FontDevice::GetFontList( std::vector<FONTINFO>& List )
{
  MAID_ASSERT( !List.empty(), "empty()ではありません" );

  LOGFONT logfont;
  logfont.lfCharSet = DEFAULT_CHARSET;

  EnumFontFamiliesEx( NULL, &logfont, EnumFontFamExProc, (LPARAM)&List, 0 );
}


class FontWindows : public IFont
{
public:
  FontWindows( const String& name, const SIZE2DI& size, HFONT font )
    :IFont(name,size),m_Font(font){}

  ~FontWindows()
  {
    DeleteObject(m_Font);
  }

  HFONT GetHFONT() const { return m_Font; }

private:
  HFONT m_Font;
};


SPFONT FontDevice::CreateFont( const String& name, const SIZE2DI& size )
{
  const std::wstring unicodename = String::ConvertMAIDtoUNICODE(name);

	const HFONT	  hFont = ::CreateFont( 
			size.h,     // フォントの高さ（論理単位）０ならデフォルト 
			size.w,			// 幅。０なら、高さにあった幅 
			0,          // スクリーンの表示画面とＸ軸の角度の１０倍
			0,
			FW_REGULAR,		// 太さ 
			FALSE,	      //イタリック体
			FALSE,				//アンダーライン
			FALSE,				//打ち消し線
			DEFAULT_CHARSET,			//文字セット
			OUT_DEFAULT_PRECIS,		//出力精度
			CLIP_DEFAULT_PRECIS,	//クリッピング精度
			PROOF_QUALITY,				//出力品質
			FIXED_PITCH | FF_MODERN,	//ピッチとファミリー
			unicodename.c_str()
      );

  return SPFONT( new FontWindows(name,size,hFont) );
}

void FontDevice::Rasterize( const SPFONT& pFont, unt32 FontCode, SurfaceSystemMemory& Dst )
{
  const SIZE2DI FontSize = pFont->GetSize();
  SurfaceDIB Src;

  Src.Create( FontSize, PIXELFORMAT_A08R08G08B08I );
  Src.Fill( COLOR_R32G32B32A32F(0,0,0,0) );

  {
    const HFONT   hFont = static_cast<FontWindows*>(pFont.get())->GetHFONT();
	  const HDC     hDC	       = ::CreateCompatibleDC(NULL);
	  const HFONT   hOldFont   = (HFONT)::SelectObject( hDC, hFont );
	  const HBITMAP hOldBitmap = (HBITMAP)::SelectObject( hDC, Src.GetHBITMAP() );
	  //	↓の設定をすることで、ドットの打たれたピクセルが 0x00000001 になる
	  ::SetTextColor( hDC, 0x00010000 );
	  ::SetBkColor  ( hDC, RGB(0,0,0) );
	  ::SetBkMode   ( hDC, OPAQUE);

	  {
      String tmp;
      tmp += FontCode;
		  const std::wstring str = String::ConvertMAIDtoUNICODE( tmp );

		  ::TextOut( hDC, 0, 0, str.c_str(), (int)str.length() );
	  }
	  ::SelectObject( hDC, hOldBitmap );
	  ::SelectObject( hDC, hOldFont );
	  ::DeleteDC( hDC );
  }

  //  0x00000001 になっているピクセルが打たれたピクセルなので
  //  それを見つけたら 不透明度を 0xFF にする
  Dst.Create( FontSize, PIXELFORMAT_A08R08G08B08I );
  Dst.Fill( COLOR_R32G32B32A32F(0,0,0,0) );

  for( int y=0; y<FontSize.h; ++y )
  {
    for( int x=0; x<FontSize.w; ++x )
    {
      const POINT2DI pos(x,y);
      unt32 v = *(unt32*)Src.GetPixelPTR(pos);

      if( v!=0 ) { Dst.SetPixel(pos,COLOR_R32G32B32A32F(0,0,0,1)); }
    }
  }
}

}


