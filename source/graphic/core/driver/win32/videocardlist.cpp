#include"videocardlist.h"

namespace Maid
{
VideoCardList::VideoCardList( Window& window )
  :m_Window(window)
{

}

void VideoCardList::Initialize()
{
	m_D3D9DLL.Load( MAIDTEXT("D3D9.dll") );

	{
		typedef IDirect3D9* (WINAPI *FUNCTIONPTR)(UINT);
		FUNCTIONPTR pd3dCreate9 = (FUNCTIONPTR)m_D3D9DLL.GetProcAddress(MAIDTEXT("Direct3DCreate9"));

		IDirect3D9* pD3D = pd3dCreate9( D3D_SDK_VERSION );

		if( pD3D==NULL ) { MAID_THROWEXCEPTION( MAIDTEXT("Direct3DCreate9 の失敗") ); }

		m_pDirect3D09.reset( pD3D );
	}

}

int VideoCardList::GetVideoCardCount()
{
  const int D3D09Count = m_pDirect3D09->GetAdapterCount();
//  const int D3D11Count = m_pDirect3D11->GetAdapterCount();
//  const int OpenGLCount= 1;

  return d3d9count;
}


VideoCardListBase::VIDEOCARDINFO VideoCardList::GetVideoCardInfo( int no )
{
  VIDEOCARDINFO info;
  info.Name = MAIDTEXT("Direct3D9");

  return info;
}

SPVIDEOCARD VideoCardList::CreateVideoCard( int No, const VideoCardListBase::CREATEPARAM& Info )
{
  //  Full16bitで作れるテクスチャを調べる
  //  Full16bitで作れる深度バッファを調べる
  //  Full16bitで作れるレンダーターゲットを調べる

  //  Full32bitで作れるテクスチャを調べる
  //  Full43bitEXで作れるテクスチャを調べる
  //  WindowMode で作れるテクスチャを調べる

  //  Full32bitで作れる深度バッファを調べる
  //  Full43bitEXで作れる深度バッファを調べる
  //  WindowMode で作れる深度バッファを調べる

  //  Full32bitで作れるレンダーターゲットを調べる
  //  Full43bitEXで作れるレンダーターゲットを調べる
  //  WindowMode で作れるレンダーターゲットを調べる


  //m_Window
  return SPVIDEOCARD();
}

}



