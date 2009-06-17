#ifndef maid2_graphics_font_h
#define maid2_graphics_font_h

#include"../config/define.h"

#include"../auxiliary/globalpointer.h"
#include"core/ifontdevice.h"

#include"surface.h"
#include"graphicscore.h"

#include<map>

namespace Maid
{
	/*!
	 	  @class	IFontRasterizeFilter font.h
	 	  @brief	フォントのラスタライズ後に加工するフィルタ
	 */ 
  class IFontRasterizeFilter
  {
  public:
    virtual ~IFontRasterizeFilter(){}

  //! フォントデータにフィルタ処理を施す
  /*!
      この中は自由にやっていいので
      内部で pData を交換してしまってもＯＫ

      @param pData  [io]  フィルタ処理するサーフェス

      @return 関数の成功で０
              失敗で０以外
   */
    virtual int Convert( SPSURFACEINSTANCE& pData )=0;
  };

  typedef	boost::shared_ptr<IFontRasterizeFilter>	SPFONTRASTERIZEFILTER;
  typedef	std::vector<SPFONTRASTERIZEFILTER>	FONTRASTERIZEFILTER;

  class Font 
    :public GlobalPointer<GraphicsCore>
  {
  public:
    enum
    {
      FILTER_NONE=~0,
    };

    void Create( const SIZE2DI& size, bool IsAntiAlias );
    void Create( const SIZE2DI& size, bool IsAntiAlias, unt FilterNo );
    void Create( const String& Name, const SIZE2DI& size, bool IsAntiAlias, unt FilterNo );
    void Destroy();

    const String&  GetName()   const;
    const SIZE2DI& GetSize()   const;
    bool IsAntiAlias()const;
    unt GetFilterNo() const;

    void Rasterize( unt32 code, SurfaceInstance& Dst )const;

    SIZE2DI CalcTextSize( const String& text ) const;

    static void SetFilter( unt No, const FONTRASTERIZEFILTER& Filter );

  private:
    typedef	std::map<unt,FONTRASTERIZEFILTER>	FILTERMAP;
    static FILTERMAP  s_FilterMap;

    Graphics::SPFONT m_pFont;
    String  m_FontName;
    SIZE2DI m_FontSize;
    bool    m_IsAntiAlias;
    unt     m_FilterNo;
  };


}
#endif
