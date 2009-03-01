#include"devicelist.h"
#include"../../../auxiliary/macro.h"
#include"../../../framework/win32/shell.h"

#include"direct3d09/deviced3d09.h"
#include"fontdevice.h"

namespace Maid { namespace Graphics {

//  DeviceID の割り振りルール
//  D3DXX が３ケタ　adapter番号が３ケタとする
//  たとえば Dx9   で adapter:1 なら  090001
//  たとえば Dx10  で adapter:5 なら  100005
//  たとえば Dx10.1で adapter:5 なら  101005

//  それぞれのデバイスで adapter:0はデフォルトデバイスとする
inline int MakeID( int dx, int sub, int no )
{
  return dx*10000 + sub*1000 + no;
}

inline int MakeID( int dx, int no )
{
  return MakeID( dx, 0, no );
}


inline int GetDxVersion( int id ) { return id / 10000; }
inline int GetDxSubVersion( int id ) { return (id / 1000) %10; }
inline int GetDeviceID ( int id ) { return id % 1000; }


DeviceList::DeviceList( Window& window )
  :m_Window(window)
  ,m_IsD3D09(false)
  ,m_IsDXGI(false)
  ,m_IsD3D10(false)
  ,m_IsD3D10_1(false)
  ,m_IsD3D10_WARP(false)
  ,m_IsD3D11(false)
{

}

void DeviceList::Initialize()
{
  InitializeD3D09();
  InitializeDXGI();
  InitializeD3D10();
//  InitializeD3D11();
}


void DeviceList::GetList( std::vector<INFO>& ret )
{
  FindAdapterD3D09( ret );
  FindAdapterD3D10( ret );
}

SPDEVICE DeviceList::Create( int DeviceID )
{
  SPDEVICE pDevice;

  const int dx = GetDxVersion(DeviceID);
  const int sub = GetDxSubVersion(DeviceID);
  const int id = GetDeviceID(DeviceID);

  switch( dx )
  {
  case 9:
    {
      UINT adapter = D3DADAPTER_DEFAULT;
      bool IsSoftwareVP = true;

      if( 2 < id ) { adapter = (id-2)/2; }
      IsSoftwareVP =  id%2 == 0;

      pDevice.reset( new DeviceD3D09( m_pDirect3D09, adapter, IsSoftwareVP, m_Window ) );

    }break;

  case 10:
    {
      if( sub==0 )
      {

      }
      ef( sub==1 )
      {

      }
    }break;

  default:{ MAID_ASSERT( true, "デバイスが選択されていません " << DeviceID ); }break;
  }

  return pDevice;
}

SPFONTDEVICE DeviceList::CreateFontDevice()
{
  return SPFONTDEVICE( new FontDevice );
}


void DeviceList::InitializeD3D09()
{
  const String dllname = MAIDTEXT("d3d9.dll");

  if( m_D3D9DLL.Load( dllname ) == DllWrapper::LOADRETURN_ERROR ) { return ; }

	{
		typedef IDirect3D9* (WINAPI *FUNCTIONPTR)(UINT);
		FUNCTIONPTR pd3dCreate9 = (FUNCTIONPTR)m_D3D9DLL.GetProcAddress(MAIDTEXT("Direct3DCreate9"));

		IDirect3D9* pD3D = pd3dCreate9( D3D_SDK_VERSION );

		if( pD3D==NULL ) { MAID_WARNING( MAIDTEXT("Direct3DCreate9 の失敗") ); return ; }

		m_pDirect3D09.reset( pD3D );
	}

  m_IsD3D09 = true;
}

void DeviceList::FindAdapterD3D09( std::vector<INFO>& info )
{
  if( !m_IsD3D09 ) { return ; }

  //  dx9 DeviceID 生成ルール
  //  偶数は MixVertexProcessing
  //  奇数は SoftwareVertexProcessing

  //  最初はデフォルト D3DADAPTER_DEFAULT
  info.push_back( INFO(MakeID(9,0),MAIDTEXT("Direct3D9 :Default(SoftwareVertexProcessing)")) );
  info.push_back( INFO(MakeID(9,1),MAIDTEXT("Direct3D9 :Default(MixedVertexProcessing)"))    );


  const SPD3D09& pD3D = m_pDirect3D09;

  const UINT count = pD3D->GetAdapterCount();

  for( UINT i=0; i<count; ++i )
  {
    D3DADAPTER_IDENTIFIER9 dat;
    const HRESULT ret = pD3D->GetAdapterIdentifier( i, 0, &dat );

    if( FAILED(ret) ) { continue; }

    const int id_base = 2+i*2;
    const String tex = MAIDTEXT("Direct3D9 :") + String::ConvertSJIStoMAID(dat.Description);

    info.push_back( INFO(MakeID(9,id_base+0), tex + MAIDTEXT("(SoftwareVertexProcessing)")) );
    info.push_back( INFO(MakeID(9,id_base+1), tex + MAIDTEXT("(MixedVertexProcessing)")) );
  }
}



void DeviceList::InitializeDXGI()
{
  const String dllname = MAIDTEXT("dxgi.dll");

  if( m_DXGIDLL.Load( dllname ) == DllWrapper::LOADRETURN_ERROR ) { return ; }

  {
		typedef HRESULT (WINAPI *FUNCTIONPTR)(REFIID, void **);
		FUNCTIONPTR create = (FUNCTIONPTR)m_DXGIDLL.GetProcAddress(MAIDTEXT("CreateDXGIFactory"));

    IDXGIFactory * pFactory = NULL;
    const HRESULT ret = create(__uuidof(IDXGIFactory), (void**)(&pFactory) );

    if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("CreateDXGIFactory の失敗") ); return ; }

    m_pDXGIFactory.reset(pFactory);
  }

  m_IsDXGI = true;
}

void DeviceList::InitializeD3D10()
{
  {
    const String dllname = MAIDTEXT("D3D10.dll");
    if( m_D3D10DLL.Load( dllname ) == DllWrapper::LOADRETURN_ERROR ) { return ; }

    m_IsD3D10 = true;
  }

  {
    const String dllname = MAIDTEXT("D3D10_1.dll");
    if( m_D3D10_1DLL.Load( dllname ) == DllWrapper::LOADRETURN_ERROR ) { return ; }

    m_IsD3D10_1 = true;
  }

  {
    if( m_D3D10_WARPDLL.Load( MAIDTEXT("D3D10WARP.dll") ) == DllWrapper::LOADRETURN_ERROR )
    {
      //  ベータがあるかも知れない
      if( m_D3D10_WARPDLL.Load( MAIDTEXT("D3D10WARP_beta.dll") ) == DllWrapper::LOADRETURN_ERROR )
      {
        return ; 
      }
    }

    m_IsD3D10_WARP = true;
  }
}

void DeviceList::FindAdapterD3D10( std::vector<INFO>& info )
{
  if( !m_IsDXGI ) { return ; }

  //  D3D10 は 生vista, D3D10.1 は vista SP1

  //  並び順を 10.0 10.1 にする

  if( m_IsD3D10 )
  {
    for( int no=0; ; ++no )
    {
      SPDXGIADAPTER pAdapter;
      {
        IDXGIAdapter* p = NULL;
        const HRESULT ret = m_pDXGIFactory->EnumAdapters(no, &p);
        if( ret==DXGI_ERROR_NOT_FOUND) { break; }
        pAdapter.reset(p);
      }

      DXGI_ADAPTER_DESC desc;
      {
        const HRESULT ret = pAdapter->GetDesc( &desc );
        if( FAILED(ret) ) { continue; }
      }

      const HRESULT ret = pAdapter->CheckInterfaceSupport(__uuidof(ID3D10Device), NULL);
      if( ret==S_OK )
      {
        const String text = MAIDTEXT("Direct3D10:") + String::ConvertUNICODEtoMAID(desc.Description);
        info.push_back( INFO(MakeID(10,0,no),text ) );
      }
    }
  }


  if( m_IsD3D10_1 )
  {
    if( m_IsD3D10_WARP )
    {
      const String text = MAIDTEXT("Direct3D10.1:Windows Advanced Rasterization Platform");
      info.push_back( INFO(MakeID(10,1,0),text ) );
    }

    for( int no=0; ; ++no )
    {
      SPDXGIADAPTER pAdapter;
      {
        IDXGIAdapter* p = NULL;
        const HRESULT ret = m_pDXGIFactory->EnumAdapters(no, &p);
        if( ret==DXGI_ERROR_NOT_FOUND) { break; }
        pAdapter.reset(p);
      }

      DXGI_ADAPTER_DESC desc;
      {
        const HRESULT ret = pAdapter->GetDesc( &desc );
        if( FAILED(ret) ) { continue; }
      }

      const HRESULT ret = pAdapter->CheckInterfaceSupport(__uuidof(ID3D10Device1), NULL);
      if( ret==S_OK )
      {
        const String text = MAIDTEXT("Direct3D10.1:") + String::ConvertUNICODEtoMAID(desc.Description);
        info.push_back( INFO(MakeID(10,1,no+1),text ) );
      }
    }
  }




}


}}



