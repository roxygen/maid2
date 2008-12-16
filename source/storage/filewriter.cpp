#include"filewriter.h"
#include"storagemessage1.h"


namespace Maid
{
  using namespace StorageMessage;

  FileWriter::FileWriter()
  {
  }

  FileWriter::~FileWriter()
  {
    Close();
  }

  void FileWriter::Create( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileWCreate> pMess( new FileWCreate );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileWriter::Open( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileWOpen> pMess( new FileWOpen );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileWriter::OpenAdd( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileWAdd> pMess( new FileWAdd );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  void FileWriter::Write( const void* pData, size_t size )
  {
    boost::shared_ptr<FileWWrite> pMess( new FileWWrite );
    pMess->Buffer.resize( size );
    
    memcpy( &(pMess->Buffer[0]), pData, size );

    SendMessage( pMess );
  }

  void FileWriter::Seek( POSITION Pos, int Size )
  {
    boost::shared_ptr<FileWSeek> pMess( new FileWSeek );
    pMess->Size = Size;

    switch( Pos )
    {
    case POSITION_BEGIN:    { pMess->Pos = FileWSeek::POSITION_BEGIN;   }break;
    case POSITION_END:      { pMess->Pos = FileWSeek::POSITION_END;     }break;
    case POSITION_CURRENT:  { pMess->Pos = FileWSeek::POSITION_CURRENT; }break;
    }

    SendMessage( pMess );
  }

  void FileWriter::Close()
  {
    if( m_pObject.get()==NULL ) { return ; }

    boost::shared_ptr<FileWClose> pMess( new FileWClose );
    SendMessage( pMess );
    m_pObject.reset();
  }

  const FileWriter::INFO& FileWriter::GetStatus()const
  {
    INFO& info = const_cast<FileWriter*>(this)->m_Info;

    if( m_pObject.get()==NULL )
    {
      info.Status = INFO::STATUS_SLEEPING;
    }else
    {
      if( m_pObject->GetMessageCount()==0 )  { info.Status = INFO::STATUS_SLEEPING;  }
      else                                   { info.Status = INFO::STATUS_WRITING;   }
    }

    return info;
  }

  void FileWriter::CreateObject()
  {
    Close();
    m_Info.Status = INFO::STATUS_SLEEPING;
    Storage* p = GlobalPointer<Storage>::Get();
    m_pObject = p->CreateFileWriter();
  }

  void FileWriter::SendMessage( const SPSTORAGEMESSAGE& pMess )
  {
    pMess->pObject = m_pObject;
    GlobalPointer<Storage>::Get()->SendMessage( pMess );
  }

}

