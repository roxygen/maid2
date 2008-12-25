#include"surfacesystemmemory.h"

#include"../../auxiliary/string.h"
#include"../../auxiliary/debug/warning.h"


namespace Maid
{

SurfaceSystemMemory::SurfaceSystemMemory()
:m_Size(0,0),m_PixelFormat(PIXELFORMAT_NONE),m_Pitch(0),m_IsCreated(false)
{

}

void SurfaceSystemMemory::Create( const SIZE2DI& size, PIXELFORMAT fmt )
{
  const int pitch = size.w * GetPixelBPP(fmt) / 8;

  Create( size, fmt, pitch );
}

void SurfaceSystemMemory::Create( const SIZE2DI& size, PIXELFORMAT fmt, int Pitch )
{
  Destroy();

  {
    const int bpp = GetPixelBPP(fmt);
    m_Plane.resize( Pitch * size.h );
  }
  {
    const int bpp = GetCLUTBPP(fmt);
    const int len = GetCLUTIndexCount(fmt);
    m_CLUT.resize( bpp * len /8 );
  }

  m_Size = size;
  m_PixelFormat = fmt;
  m_Pitch = Pitch;
  m_IsCreated = true;
}

void SurfaceSystemMemory::Destroy()
{
  if( IsEmpty() ) { return ; }

  m_Plane.clear();
  m_CLUT.clear();
  m_Size = SIZE2DI(0,0);
  m_PixelFormat = PIXELFORMAT_NONE;
  m_Pitch = 0;
}

bool SurfaceSystemMemory::IsEmpty() const
{
  return !m_IsCreated;
}

SIZE2DI     SurfaceSystemMemory::GetSize() const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );

  return m_Size;
}

PIXELFORMAT SurfaceSystemMemory::GetPixelFormat() const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return m_PixelFormat;
}


int   SurfaceSystemMemory::GetPitch() const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return m_Pitch;
}

void* SurfaceSystemMemory::GetPlanePTR()
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return &(m_Plane[0]); 
}

void* SurfaceSystemMemory::GetCLUTPTR()
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return &(m_CLUT[0]); 
}


}


