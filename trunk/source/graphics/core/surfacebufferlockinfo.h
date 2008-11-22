#ifndef maid2_graphic_core_surfacebufferlockinfo_h
#define maid2_graphic_core_surfacebufferlockinfo_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

#include"../../../auxiliary/mathematics.h"
#include"../../pixelformat.h"
#include"../../color.h"


namespace Maid
{
  /*!
    @class	SurfaceBufferLockInfo surfacebuffer.h
    @brief	サーフェスの情報を表す構造体
  \n			SurfaceBuffer::Lock() でこのクラスを取得します
   
   */
  class SurfaceBufferLockInfo
  {
  public:
    void*           GetAddress() const;
    int		          GetPitch() const;
    const SIZE2DI&	GetSize() const;
    PIXELFORMAT     GetPixelFormat() const;

    void* GetCLUT() const;

    void* GetLinePtr( int PosY )const;
    void* GetPixelPtr( const POINT2DI& pos ) const;

    COLOR_A16B16G16R16I GetPixelI( const POINT2DI& pos );
    void SetPixelI( const POINT2DI& pos, const COLOR_A16B16G16R16I& col );
    COLOR_A32B32G32R32F GetPixelF( const POINT2DI& pos );
    void SetPixelF( const POINT2DI& pos, const COLOR_A32B32G32R32F& col );

    void SetInfo( void* pBits, void* pCLUT, const SIZE2DI& Size, PIXELFORMAT fmt, int Pitch, bool IsReverse );
  private:
    void*		m_pBits;
    void*		m_pCLUT;
    SIZE2DI	m_Size;
    PIXELFORMAT	m_Format;
    int			m_Pitch;
    bool    m_IsReverse;
  };
}


#endif