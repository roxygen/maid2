#include"oggcontainer.h"

#include"../../auxiliary/macro.h"
#include"../../auxiliary/debug/warning.h"


namespace Maid { namespace Xiph {


const long READSIZE = 1024*1024;


OggContainer::OggContainer()
  :m_IsEnd(false)
{
  ZERO( &m_State, sizeof(m_State) );
}

OggContainer::~OggContainer()
{

}


void OggContainer::Initialize( const String& FileName )
{
  Finalize();

  m_hFile.Open( FileName );
  ogg_sync_init(&m_State);

  NextPage();
}

void OggContainer::Finalize()
{
  ogg_sync_clear(&m_State);
  m_hFile.Close();
  m_IsEnd = false;
}

const OggPage& OggContainer::GetCurrentPage() const
{
  return m_CurrentPage;

}

void OggContainer::NextPage()
{
  while( true )
  {
    /*
       return values:
       -1) recapture (hole in data)
        0) need more data
        1) page returned
    */
    const int ret = ogg_sync_pageout(&m_State,&(m_CurrentPage.m_Page));

//    MAID_WARNING( "error" << ret );
    if( ret==1 ) { break; }
    ef( ret==0 )
    {
      //  すでに終端にいたらダメポ
      if( IsEnd() ) { return ; }
/*
      {
        const size_t size = m_hFile.GetSize();
        const size_t pos  = m_hFile.GetPosition();

        if( size <= pos ) { m_IsEnd = true; return ; }
      }
*/
      char* buffer = ogg_sync_buffer(&m_State,READSIZE);
      const size_t size = m_hFile.Read( buffer, READSIZE );
      {
        if( size==0 ) { m_IsEnd = true; return ; }
      }

      ogg_sync_wrote(&m_State, size);
    }
  }
}

bool OggContainer::IsEnd() const
{
  const size_t size = m_hFile.GetSize();

//  return size <= (size_t)m_State.returned ;

//    MAID_WARNING( "IsEnd" << m_IsEnd );
  return m_IsEnd;
}

void OggContainer::UpdateBuffer()
{
  char* buffer = ogg_sync_buffer(&m_State,READSIZE);
  const size_t size = m_hFile.Read( buffer, READSIZE );

  ogg_sync_wrote(&m_State, size);

}


}}
