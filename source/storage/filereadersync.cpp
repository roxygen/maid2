#include"filereadersync.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{

  bool FileReaderSync::Open( const String& filename )
  {
    m_Reader.Open( filename );
    return m_Reader.Sync();
  }

  bool FileReaderSync::Read( void* pData, size_t size )
  {
    m_Reader.Read( pData, size );
    return m_Reader.Sync();
  }

  bool FileReaderSync::Seek( POSITION Pos, int Size )
  {
    FileReader::POSITION p;
    switch( Pos )
    {
    case POSITION_BEGIN:    { p = FileReader::POSITION_BEGIN;   }break;
    case POSITION_END:      { p = FileReader::POSITION_END;     }break;
    case POSITION_CURRENT:  { p = FileReader::POSITION_CURRENT; }break;
    }

    m_Reader.Seek( p, Size );
    return m_Reader.Sync();
  }

  bool FileReaderSync::Close()
  {
    m_Reader.Close();
    return m_Reader.Sync();
  }

  size_t  FileReaderSync::GetSize() const
  {
    return m_Reader.GetSize();
  }

  size_t  FileReaderSync::GetPosition() const
  {
    return m_Reader.GetPosition();
  }



}

