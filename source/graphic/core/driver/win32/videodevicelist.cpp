#include"videodevicelist.h"
#include"../../../../auxiliary/macro.h"

#include"direct3d09/videodeviced3d09.h"

namespace Maid
{
VideoDeviceList::VideoDeviceList( Window& window )
  :m_Window(window)
{

}

void VideoDeviceList::Initialize()
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

int VideoDeviceList::GetVideoCardCount()
{
  const int D3D09Count = m_pDirect3D09->GetAdapterCount();
//  const int D3D11Count = m_pDirect3D11->GetAdapterCount();
//  const int OpenGLCount= 1;

  return D3D09Count;
}


VideoDeviceListBase::VIDEOCARDINFO VideoDeviceList::GetVideoCardInfo( int no )
{
  VIDEOCARDINFO info;
  info.Name = MAIDTEXT("Direct3D9");

  return info;
}

SPVIDEODEVICE VideoDeviceList::CreateVideoDevice( int No, const VideoDeviceListBase::CREATEPARAM& Info )
{
#pragma  COMPILERMSG( "当面は引数に関係なくD3D9 HALです" )

  //  ここで作るデバイスはかならずウィンドウモードにすること！

  SPVIDEODEVICE pDevice;

  {
    const UINT adapter = D3DADAPTER_DEFAULT;
    const D3DDEVTYPE type = D3DDEVTYPE_HAL;
    const HWND hWnd = m_Window.GetHWND();
    const DWORD Flags = Info.IsSoftwareVertexProcessing? D3DCREATE_SOFTWARE_VERTEXPROCESSING : D3DCREATE_MIXED_VERTEXPROCESSING; 
    D3DPRESENT_PARAMETERS param={0};
    param.BackBufferWidth = 0;
    param.BackBufferHeight = 0;
    param.BackBufferFormat = D3DFMT_UNKNOWN;
    param.BackBufferCount = 1;
    param.MultiSampleType = D3DMULTISAMPLE_NONE;  //  ウィンドウモードではマルチサンプルできない
    param.MultiSampleQuality = 0;
    param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    param.hDeviceWindow = hWnd;
    param.Windowed = TRUE;
    param.EnableAutoDepthStencil = FALSE;
    param.Flags = 0;
    param.FullScreen_RefreshRateInHz = 0;
    param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    IDirect3DDevice9* p=NULL;
    HRESULT ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,Flags, &param, &p );
    if( FAILED(ret) ) 
    {
      //  失敗すると、param に適切な値が入っているので、もう一回作る
      ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,Flags, &param, &p );

      if( FAILED(ret) ) 
      {
        //  それでもダメな場合は頂点処理をソフトウェアでやってみる
        ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, &param, &p );
        if( FAILED(ret) ) 
        {
          //  これでもダメならアウト
          MAID_THROWEXCEPTION(MAIDTEXT("IDirect3D::CreateDevice()"));
        }
      }
    }

    pDevice.reset( new VideoDeviceD3D09(p,m_Window) );
  }

  pDevice->Initialize();

  return pDevice;
}

}



