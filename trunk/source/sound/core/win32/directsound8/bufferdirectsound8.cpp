#include"bufferdirectsound8.h"

#include"../../../../auxiliary/debug/warning.h"

namespace Maid { namespace Sound {

BufferDirectSound8::BufferDirectSound8( const CREATEBUFFERPARAM& param, const SPDIRECTSOUNDBUFFER& pBuffer )
 :IBuffer(param),m_pBuffer(pBuffer)
{

}

void BufferDirectSound8::Lock( size_t begin, size_t Length, LOCKDATA& data )
{
  void* pBuf1 = NULL;
  void* pBuf2 = NULL;
  DWORD len1 = 0;
  DWORD len2 = 0;

  const HRESULT ret = m_pBuffer->Lock( begin, Length, &pBuf1, &len1, &pBuf2, &len2, 0 );
  if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::Lock" ); }

  data.pData1      = pBuf1;
  data.Data1Length = len1;
  data.pData2      = pBuf2;
  data.Data2Length = len2;
}

void BufferDirectSound8::Unlock( LOCKDATA& data )
{
  void* pBuf1 = data.pData1;
  void* pBuf2 = data.pData2;
  DWORD len1 = data.Data1Length;
  DWORD len2 = data.Data2Length;

  const HRESULT ret = m_pBuffer->Unlock( pBuf1, len1, pBuf2, len2 );
  if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::Unlock" ); }
}

void BufferDirectSound8::Play( bool IsLoop )
{
  DWORD flag = 0;
	if( IsLoop ) { flag |= DSBPLAY_LOOPING; }

	const HRESULT ret = m_pBuffer->Play( 0, 0, flag );
	if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::Play()" ); }

}

void BufferDirectSound8::SetPosition( size_t pos )
{
  const HRESULT ret = m_pBuffer->SetCurrentPosition( pos );
  if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::SetCurrentPosition()" ); }
}

void BufferDirectSound8::SetVolume( float vol )
{
  if( vol<=0 )
  {
		const HRESULT ret = m_pBuffer->SetVolume( DSBVOLUME_MIN );
		if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::SetVolume(DSBVOLUME_MIN)" ); }
  }else
  {
    const float dsv = float(DSBVOLUME_MIN-DSBVOLUME_MAX) * vol;
    const int v = int(dsv+0.5f);

    const HRESULT ret = m_pBuffer->SetVolume( std::min(DSBVOLUME_MAX,v) );
		if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::SetVolume()" ); }
  }

}

void BufferDirectSound8::Stop()
{
	const HRESULT ret = m_pBuffer->Stop();
	if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::Stop()" ); }
}

bool BufferDirectSound8::IsPlay() const
{
  DWORD status=0;
	const HRESULT ret = m_pBuffer->GetStatus(&status);
	if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::GetStatus()" ); }

  return IsFlag(status,DSBSTATUS_PLAYING);
}

void BufferDirectSound8::GetPosition( size_t& play, size_t& write )const
{
  DWORD p=0;
  DWORD w=0;
	const HRESULT ret = m_pBuffer->GetCurrentPosition(&p,&w);
	if( FAILED(ret) ) { MAID_WARNING( "IDirectSoundBuffer::GetCurrentPosition()" ); }

  play  = p;
  write = w;
}

}}


