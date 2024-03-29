﻿#include"devicedirectsound8.h"
#include"bufferdirectsound8.h"

#include"../../../../auxiliary/debug/warning.h"


namespace Maid { namespace Sound {

DeviceDirectSound8::DeviceDirectSound8( const boost::shared_ptr<GUID>& guid, const DllWrapper& dsounddll, const Window& window )
 :m_pGUID(guid),m_dsounddll(dsounddll),m_Window(window)
{

}


void DeviceDirectSound8::Initialize()
{
  {
		typedef  HRESULT (WINAPI *FUNCTIONPTR)( LPCGUID, LPDIRECTSOUND8*, LPUNKNOWN );
		FUNCTIONPTR pdsoundCreate = (FUNCTIONPTR)m_dsounddll.GetProcAddress(MAIDTEXT("DirectSoundCreate8"));

		IDirectSound8* pDS8=NULL;
		const HRESULT ret = pdsoundCreate( m_pGUID.get(), &pDS8, NULL );
		if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("DirectSoundCreate8()") ); return; }

    m_pDevice.reset(pDS8);
  }

	HRESULT ret;

	ret = m_pDevice->SetCooperativeLevel( m_Window.GetHWND(), DSSCL_PRIORITY );
	if( FAILED(ret) ) { goto NORMAL_MODE; }

  {
    IDirectSoundBuffer* pBuf;
    DSBUFFERDESC dsbd = {0};

    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER | DSBCAPS_GLOBALFOCUS;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
    dsbd.guid3DAlgorithm = GUID_NULL ;

    ret = m_pDevice->CreateSoundBuffer( &dsbd, &pBuf, NULL );
    if( FAILED(ret) ) { goto NORMAL_MODE; }

    m_pPrimary.reset( pBuf );
  }

  PlayDummySound();
  return ;


NORMAL_MODE:
	ret = m_pDevice->SetCooperativeLevel( m_Window.GetHWND(), DSSCL_NORMAL );
  if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("IDirectSound8::SetCooperativeLevel()") ); }

  PlayDummySound();
  return ;
}

void DeviceDirectSound8::Finalize()
{
  m_pDummy.reset();
  m_pPrimary.reset();
  m_pDevice.reset();
}

void DeviceDirectSound8::SetFormat( const PCMFORMAT& fmt )
{
  //  優先モードでなかったら変更できない
  if( !IsPriorityMode() ) { return ; }

  WAVEFORMATEX wfx;
  ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
  wfx.wFormatTag      = WAVE_FORMAT_PCM; 
  wfx.nChannels       = (WORD) fmt.Channels; 
  wfx.nSamplesPerSec  = fmt.SamplesPerSecond; 
  wfx.wBitsPerSample  = (WORD) fmt.BitPerSamples;
  wfx.nBlockAlign     = fmt.CalcBlockSize();
  wfx.nAvgBytesPerSec = fmt.CalcBytePerLength();

  const HRESULT ret = m_pPrimary->SetFormat(&wfx);
  if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("IDirectSound8::SetFormat()") ); }
}


SPBUFFER DeviceDirectSound8::CreateBuffer( const CREATEBUFFERPARAM& param )
{
  const PCMFORMAT& fmt = param.Format;

  WAVEFORMATEX wfx={0};
  wfx.wFormatTag      = WAVE_FORMAT_PCM;
  wfx.nChannels       = fmt.Channels;
  wfx.nSamplesPerSec  = fmt.SamplesPerSecond;
  wfx.wBitsPerSample  = fmt.BitPerSamples;
  wfx.nBlockAlign     = fmt.CalcBlockSize();
  wfx.nAvgBytesPerSec = fmt.CalcBytePerLength();
  wfx.cbSize          = 0;

  const DWORD flag = DSBCAPS_GLOBALFOCUS
                   | DSBCAPS_LOCSOFTWARE
                   | DSBCAPS_CTRLVOLUME
                   | DSBCAPS_GETCURRENTPOSITION2
                   ;


  DSBUFFERDESC dsbd = {0};
  dsbd.dwSize        = sizeof(dsbd);
  dsbd.dwFlags       = flag;
  dsbd.dwBufferBytes = param.Length;
  dsbd.lpwfxFormat   = &wfx;
  dsbd.guid3DAlgorithm = GUID_NULL ;

  IDirectSoundBuffer* pBuf=NULL;
  const HRESULT ret = m_pDevice->CreateSoundBuffer( &dsbd, &pBuf, NULL );
  if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("IDirectSound8::CreateSoundBuffer()") ); }
  SPDIRECTSOUNDBUFFER p(pBuf);

  return SPBUFFER( new BufferDirectSound8(param,p) );
}

SPBUFFER DeviceDirectSound8::DuplicateBuffer( const SPBUFFER& pSrc )
{
  BufferDirectSound8* p = static_cast<BufferDirectSound8*>(pSrc.get());

  IDirectSoundBuffer* pBuf=NULL;
  const HRESULT ret = m_pDevice->DuplicateSoundBuffer( p->GetBuffer().get(), &pBuf );
  if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("IDirectSound8::DuplicateSoundBuffer()") ); }
  SPDIRECTSOUNDBUFFER pds(pBuf);

  return SPBUFFER( new BufferDirectSound8(p->GetParam(),pds) );
}

bool  DeviceDirectSound8::IsPriorityMode()
{
  return m_pPrimary.get()!=NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WDMドライバのバグ対策関数
/*!	
 *	プライマリバッファが止まった後に再度再生させるとき
 *	前回再生したバッファが少し再生されてしまいノイズが聞こえる
 *	なのでプライマリバッファを停止させないように無音を再生しつづける
 *
 */
void DeviceDirectSound8::PlayDummySound()
{
  const DWORD chan = 2;
  const DWORD freq = 44100;
  const DWORD bps  = 16;

  WAVEFORMATEX wfx={0};
  wfx.wFormatTag      = WAVE_FORMAT_PCM;
  wfx.nChannels       = chan;
  wfx.nSamplesPerSec  = freq;
  wfx.wBitsPerSample  = bps;
  wfx.nBlockAlign     = chan* (bps/8);
  wfx.nAvgBytesPerSec = freq*wfx.nBlockAlign;
  wfx.cbSize          = 0;

  {
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(dsbd);
    dsbd.dwFlags       = DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes = wfx.nAvgBytesPerSec;
    dsbd.lpwfxFormat   = &wfx;
    dsbd.guid3DAlgorithm = GUID_NULL ;

    IDirectSoundBuffer* pBuf;
    m_pDevice->CreateSoundBuffer( &dsbd, &pBuf, NULL );
    m_pDummy.reset(pBuf);
  }

  //	そんでもって流し込む
  LPVOID	pPos1,	pPos2;
  DWORD	dwLen1,	dwLen2;

  m_pDummy->Lock( 0, wfx.nAvgBytesPerSec, &pPos1, &dwLen1, &pPos2, &dwLen2, 0 );
  {
    ZeroMemory( pPos1, wfx.nAvgBytesPerSec );
  }
  m_pDummy->Unlock( pPos1, dwLen1, pPos2, dwLen2 );

  m_pDummy->Play( 0, 0, DSBPLAY_LOOPING );
}

}}


