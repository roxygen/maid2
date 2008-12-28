/*!
    @file
    @brief  平面
 */

#ifndef maid2_graphics_surface_h
#define maid2_graphics_surface_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/mathematics/size.h"
#include"../auxiliary/mathematics/point.h"
#include"pixelformat.h"
#include"color.h"

#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Surface
  {
  public:
    Surface();
		void Create( const SIZE2DI& size, PIXELFORMAT fmt );
		void Create( const SIZE2DI& size, PIXELFORMAT fmt, int pitch );
		void Destroy();

		const SIZE2DI&    GetSize() const;
		PIXELFORMAT GetPixelFormat() const;
    int   GetPitch() const;
    bool  IsEmpty() const;

    //  普通に Surface XX; とか定義したとき用
		void* GetPlanePTR() { return _GetPlanePTR(); }
		void* GetCLUTPTR() { return _GetCLUTPTR(); }
		void* GetLinePTR( int y ) { return _GetLinePTR(y); }
		void* GetPixelPTR( const POINT2DI& pos ) { return _GetPixelPTR(pos); }
		void* GetColorPTR( int index ) { return _GetColorPTR(index); }

    //  const Surface XX; とか定義したとき用
    const void* GetPlanePTR()const { return const_cast<Surface*>(this)->_GetPlanePTR(); }
		const void* GetCLUTPTR() const { return const_cast<Surface*>(this)->_GetCLUTPTR(); }
		const void* GetLinePTR( int y )const { return const_cast<Surface*>(this)->_GetLinePTR(y); }
		const void* GetPixelPTR( const POINT2DI& pos )const { return const_cast<Surface*>(this)->_GetPixelPTR(pos); }
		const void* GetColorPTR( int index )const { return const_cast<Surface*>(this)->_GetColorPTR(index); }

    COLOR_R32G32B32A32F GetPixel( const POINT2DI& pos )const;
    void SetPixel( const POINT2DI& pos, const COLOR_R32G32B32A32F& col );

  private:
		void* _GetPlanePTR();
		void* _GetCLUTPTR();
		void* _GetLinePTR( int y );
		void* _GetPixelPTR( const POINT2DI& pos );
		void* _GetColorPTR( int index );

  private:
    std::vector<unt08>	m_Plane;
    std::vector<unt08>	m_CLUT;

    SIZE2DI		  m_Size;
    PIXELFORMAT m_PixelFormat;
    int  m_Pitch;
    bool m_IsCreated;

	};

  typedef	boost::shared_ptr<Surface>	SPSURFACE;

}

#endif

