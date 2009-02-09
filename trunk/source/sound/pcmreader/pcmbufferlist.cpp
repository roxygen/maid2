#include"pcmbufferlist.h"

#include"../../auxiliary/macro.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"



namespace Maid
{

PCMBufferList::PCMBufferList( const PCMFORMAT& fmt )
  :m_Format(fmt)
  ,m_Position(0)
  ,m_BufferPosition(0)
{

}

PCMBufferList::INITIALIZERESULT PCMBufferList::Initialize()
{
  Finalize();

  return INITIALIZERESULT_OK;
}

void PCMBufferList::Finalize()
{
  ThreadMutexLocker lock(m_Mutex);

  m_BufferList.clear();
  m_Position = 0;
  m_BufferPosition = 0;
}

void PCMBufferList::Clear()
{
  ThreadMutexLocker lock(m_Mutex);

  m_BufferList.clear();
  m_Position = 0;
  m_BufferPosition = 0;
}


size_t  PCMBufferList::Read( void* pDst, size_t size )
{
  ThreadMutexLocker lock(m_Mutex);

  size_t len = size;
  unt08* pTarget = (unt08*)pDst;

  std::list<SPBUFFER>::iterator ite=m_BufferList.begin();

  while( true )
  {
    if( len==0 ) { break; }
    if( ite==m_BufferList.end() ) { break; }

    const SPBUFFER& pSrc = (*ite);

    const size_t lim = pSrc->GetSize() - m_BufferPosition;  //  このバッファでまだ読んでない量

    if( len <= lim )
    {
      ::memcpy( pTarget, pSrc->GetPointer(m_BufferPosition), len );

      m_BufferPosition += len;
      len = 0;
      break;
    }else
    {
      ::memcpy( pTarget, pSrc->GetPointer(m_BufferPosition), lim );

      pTarget += lim;

      len -= lim;
      m_BufferPosition = 0;
      ite = m_BufferList.erase(ite);
    }
  }

  if( 0<len )
  {
    //  足りない分は無音でも突っ込んでおく
    ZERO( pTarget, len );
  }

  m_Position += size;
  return size;
}

void    PCMBufferList::SetPosition( size_t Offset )
{
  m_Position = Offset;
}

size_t  PCMBufferList::GetPosition()		const
{
  return m_Position;
}

size_t  PCMBufferList::GetLength()			const
{
  ThreadMutexLocker lock(const_cast<PCMBufferList*>(this)->m_Mutex);

  size_t ret = m_Position;

  for( std::list<SPBUFFER>::const_iterator ite=m_BufferList.begin();
            ite!=m_BufferList.end(); ++ite )
  {
    ret += (*ite)->GetSize();
  }

  return ret;
}

PCMFORMAT PCMBufferList::GetFormat() const
{
  return m_Format;
}

void PCMBufferList::Create( const void* pData, size_t Size )
{
  SPBUFFER pBuf( new Buffer );
  pBuf->Resize(Size);
  ::memcpy( pBuf->GetPointer(0), pData, Size );


  ThreadMutexLocker lock(m_Mutex);
  m_BufferList.push_back( pBuf );
}


}

