#include"surfacebuffermemory.h"

#include"../../../auxiliary/debug/assert.h"


namespace Maid
{

SurfaceBufferMemory::SurfaceBufferMemory()
{
	m_Size = SIZE2DI(0,0);
	m_PixelFormat = PIXELFORMAT_NONE;
}


SurfaceBufferMemory::~SurfaceBufferMemory()
{
	Destroy();
}

SIZE2DI SurfaceBufferMemory::GetSize() const
{
	MAID_ASSERT( m_PixelFormat==PIXELFORMAT_NONE, "まだ作成されていません" );
	return m_Size;
}

PIXELFORMAT SurfaceBufferMemory::GetPixelFormat() const
{
	MAID_ASSERT( m_PixelFormat==PIXELFORMAT_NONE, "まだ作成されていません" );
	return m_PixelFormat;
}

void SurfaceBufferMemory::Lock( SurfaceBufferLockInfo& info )
{
	MAID_ASSERT( m_PixelFormat==PIXELFORMAT_NONE, "まだ作成されていません" );

	void* pCLUT = NULL;
	void* pPlane = &m_Plane[0];
  const int pitch = m_Size.w*GetPixelBPP(m_PixelFormat)/8;
	
	if( GetCLUTBPP(m_PixelFormat)!=0 )
	{
		pCLUT = &(m_CLUT[0]);
	}

  info.SetInfo( pPlane, pCLUT, m_Size, m_PixelFormat, pitch, m_IsReverse );
}

void SurfaceBufferMemory::Unlock()
{
	//	やることなし
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの作成
/*!
 	  @param	size	[i ]	サーフェスの大きさ
 	  @param	fmt		[i ]	ピクセルフォーマット
 */
void SurfaceBufferMemory::Create( const SIZE2DI& size, PIXELFORMAT fmt )
{
	Destroy();

	const int bpp  = GetPixelBPP(fmt);
	const int clut = GetCLUTBPP(fmt);
	const int count = GetCLUTLength(fmt);

	m_Plane.resize( size.w * bpp/8 * size.h +16 );

	if( clut!=0 )
	{
		m_CLUT.resize( clut/8 * count +16);
	}

	m_Size = size;
	m_PixelFormat = fmt;
	m_IsReverse = false;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの破棄
/*!
 */
void SurfaceBufferMemory::Destroy()
{
	m_Plane.clear();
	m_PixelFormat = PIXELFORMAT_NONE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このサーフェスは反転してる設定を行う
/*!
	@param	IsReverse	[i ]	反転してるなら true
 */
void SurfaceBufferMemory::SetReverseSurface( bool IsReverse )
{
	m_IsReverse = IsReverse;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 裏技で取得
/*!
 */
void* SurfaceBufferMemory::GetPlanePTR()
{
	return &(m_Plane[0]);
}

}


