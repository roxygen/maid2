#include"surfacebufferd3d09.h"


namespace Maid
{
SurfaceBufferD3D09::SurfaceBufferD3D09()
{
  //TextureBufferD3D09::Surface の初期化のためだけにある

}

SurfaceBufferD3D09::SurfaceBufferD3D09( IDirect3DSurface9* pCom )
{
	pSurface.reset(pCom);
	pSurface->GetDesc( &Desc );
}

SIZE2DI SurfaceBufferD3D09::GetSize() const
{
	MAID_ASSERT( pSurface.get()==NULL, "初期化されていません" );
	return SIZE2DI( Desc.Width, Desc.Height );
}

PIXELFORMAT SurfaceBufferD3D09::GetPixelFormat() const
{
	MAID_ASSERT( pSurface.get()==NULL, "初期化されていません" );
	return D3DFORMATtoPIXELFORMAT( Desc.Format );
}

void SurfaceBufferD3D09::Lock( SurfaceBufferLockInfo& info )
{
	MAID_ASSERT( pSurface.get()==NULL, "初期化されていません" );

	SIZE2DI size = GetSize();
	PIXELFORMAT fmt = GetPixelFormat();

	D3DLOCKED_RECT rc;

	const HRESULT ret =	pSurface->LockRect( &rc, NULL, 0 );
	if( FAILED(ret) ) { MAID_THROWEXCEPTION( MAIDTEXT("IDirect3DSurface9::LockRect()") ); }

  info.SetInfo( rc.pBits, NULL, size, fmt, rc.Pitch, false );
}

void SurfaceBufferD3D09::Unlock()
{
	MAID_ASSERT( pSurface.get()==NULL, "初期化されていません" );
	const HRESULT ret =	pSurface->UnlockRect();
	if( FAILED(ret) ) { MAID_THROWEXCEPTION( MAIDTEXT("IDirect3DSurface9::UnlockRect()") ); }
}

}