﻿#include"devicelist.h"
#include"directsound8/devicedirectsound8.h"

#include"../../../auxiliary/debug/warning.h"


namespace Maid { namespace Sound {

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


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! DirectSoundドライバの能力を調べる関数
/*!	
 *	詳しくはDXヘルプを参照
 *
 */
BOOL CALLBACK DeviceList::DirectSoundEnumCallback( LPGUID pGUID, LPCWSTR strDesc, LPCWSTR strDrvName, LPVOID pContext )
{
  std::vector<DIRECTSOUND8DEVICE>* pList = (std::vector<DIRECTSOUND8DEVICE>*)pContext;

  DIRECTSOUND8DEVICE  dev;

  if( pGUID!=NULL )
  {
    dev.pGUID.reset( new GUID );
    ::memcpy( dev.pGUID.get(), pGUID, sizeof(GUID) );
  }

  dev.Desc = String::ConvertUNICODEtoMAID(strDesc);
  dev.Driver = String::ConvertUNICODEtoMAID(strDrvName);

  pList->push_back( dev );

  return TRUE;
}


DeviceList::DeviceList( Window& window )
  :m_Window(window)
{

}

void DeviceList::Initialize()
{

  { //  directsound8 を調べる
	  m_dsoundDLL.Load( MAIDTEXT("dsound.dll") );
		typedef  HRESULT (WINAPI *SoundEnum)( LPDSENUMCALLBACK, LPVOID );
		SoundEnum pDirectSoundEnumerate = (SoundEnum)m_dsoundDLL.GetProcAddress(MAIDTEXT("DirectSoundEnumerateW"));
		pDirectSoundEnumerate( &DeviceList::DirectSoundEnumCallback,  &m_DirectSound8Device );
  }

}

void DeviceList::GetList( std::vector<INFO>& ret )
{
  {
    const std::vector<DIRECTSOUND8DEVICE>& info = m_DirectSound8Device;

    for( int i=0; i<(int)info.size(); ++i )
    {
      const int id = MakeID( 8, i );
      String name = MAIDTEXT("DirectSound8: ");

      if( info[i].Driver.empty() ) { name += MAIDTEXT("default"); }
      else { name += info[i].Desc; }

      ret.push_back( INFO( id, name ) );
    }
  }
}

SPDEVICE DeviceList::CreateDevice( int DeviceID )
{
  SPDEVICE pDevice;

  const int dx = GetDxVersion(DeviceID);
  const int sub = GetDxSubVersion(DeviceID);
  const int id = GetDeviceID(DeviceID);

  switch( dx )
  {
  case 8:
    {
      pDevice.reset( new DeviceDirectSound8( m_DirectSound8Device[id].pGUID,m_dsoundDLL,m_Window) );
    }break;

  default:{ MAID_ASSERT( true, "デバイスが選択されていません " << DeviceID ); }break;
  }

  return pDevice;
}

}}


