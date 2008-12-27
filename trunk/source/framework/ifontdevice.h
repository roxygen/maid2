/*!
	  @file
	  @brief フォント管理のベースクラス
 */

#ifndef framework_ifontdevice_h
#define framework_ifontdevice_h

#include"../config/define.h"

#include"../auxiliary/string.h"
#include"../auxiliary/mathematics/size.h"

#include"../graphics/imagefile/surfacesystemmemory.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IFont
  {
  public:
    IFont( const String& name, const SIZE2DI& size )
      :m_Name(name),m_Size(size){}
    virtual ~IFont(){}

    const String&  GetName() const { return m_Name; }
    const SIZE2DI& GetSize() const { return m_Size; }

  private:
    const String  m_Name; //  フォント名
    const SIZE2DI m_Size; //  半角の大きさ
  };

  typedef	boost::shared_ptr<IFont>	SPFONT;


  class IFontDevice
  {
  public:
    virtual ~IFontDevice(){}

    struct FONTINFO
    {
      String  Name; //!<  フォント名
    };

    virtual void GetFontList( std::vector<FONTINFO>& List )=0;

    virtual SPFONT CreateFont( const String& name, const SIZE2DI& size )=0;
    virtual SPFONT CreateDefaultFont( const SIZE2DI& size )=0;
    
    virtual void Rasterize( const SPFONT& pFont, unt32 FontCode, SurfaceSystemMemory& surf )=0;
  };

  typedef	boost::shared_ptr<IFontDevice>	SPFONTDEVICE;

}


#endif