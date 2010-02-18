﻿#include"filereader.h"
#include"storagemessage2.h"
#include"storagelocal.h"
#include"../auxiliary/debug/warning.h"

#include"../auxiliary/thread.h"

namespace Maid
{
  using namespace StorageMessage;

  /*!
      @class  FileReader filereader.h
      @brief  非同期ファイル読み込みクラス
              同期したい場合は FileReaderSync を使います
  */

  FileReader::FileReader()
  {
  }

  FileReader::~FileReader()
  {
    Close();
  }

  //! ファイルを開く
  /*!
 	    @param	filename      [i ]	開きたいファイル
   */
  void FileReader::Open( const String& filename )
  {
    CreateObject();

    boost::shared_ptr<FileROpen> pMess( new FileROpen );
    pMess->FileName = filename;

    SendMessage( pMess );
  }

  //! データを読み込む
  /*!
 	    @param	pData  [ o]	書き込み先
 	    @param	size   [i ]	読む量（バイト単位）

      @return 読み込む予定サイズ
   */
  size_t FileReader::Read( void* pData, size_t size )
  {
    const size_t file_pos  = GetPosition();
    const size_t file_size = GetSize();

    const size_t read_size = std::min(size, file_size-file_pos);

    if( read_size!=0 )
    {
      boost::shared_ptr<FileRRead> pMess( new FileRRead );
      pMess->pDst = pData;
      pMess->Size = std::min(size, read_size);

      SendMessage( pMess );
    }

    return read_size;
  }

  //! シークする
  /*!
 	    @param	Pos  [ o]	シーク基準
 	    @param	Size [i ]	移動量
   */
  void FileReader::Seek( POSITION Pos, int Size )
  {
    boost::shared_ptr<FileRSeek> pMess( new FileRSeek );
    pMess->Size = Size;

    switch( Pos )
    {
    case POSITION_BEGIN:    { pMess->Pos = FileRSeek::POSITION_BEGIN;   }break;
    case POSITION_END:      { pMess->Pos = FileRSeek::POSITION_END;     }break;
    case POSITION_CURRENT:  { pMess->Pos = FileRSeek::POSITION_CURRENT; }break;
    }

    SendMessage( pMess );
  }

  //! 閉じる
  /*!
   */
  void FileReader::Close()
  {
    Close( false );
  }

  //! 閉じる
  /*!
      @param  IsSync [i ] 閉じるのを待つか？
   */
  void FileReader::Close( bool IsSync )
  {
    if( m_pObject.get()==NULL ) { return ; }

    boost::shared_ptr<FileRClose> pMess( new FileRClose );
    SendMessage( pMess );

    if( IsSync )
    {
      if( Sync() )
      {
        MAID_WARNING( MAIDTEXT("FileReader::Close()") ); 
      }
    }

    m_pObject.reset();
  }

  //! 実行している処理が終わるまで待つ
  /*!
   */
  bool FileReader::Sync()
  {
    while( true )
    {
      if( IsError() ) {  return true; }
      if( !IsExecuting() ) { break; }
      ThreadController::Sleep(1);
    }
    return false;
  }

  //! まだ開いていないか？
  /*!
   */
  bool FileReader::IsEmpty() const
  {
    return m_pObject.get()==NULL;
  }

  //! 処理が実行中か？
  /*!
   */
  bool FileReader::IsExecuting() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->GetMessageCount()!=0;
  }

  //! エラーが起こったか？
  /*!
   */
  bool FileReader::IsError() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return false; }
    return m_pObject->IsError();
  }

  //! ファイルの全長の取得
  /*!
   */
  size_t  FileReader::GetSize() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetSize();
  }

  //! ファイルカーソル位置の取得
  /*!
   */
  size_t  FileReader::GetPosition() const
  {
    MAID_ASSERT( IsEmpty(), "まだOpenしてません" );
    if( IsEmpty() ) { return 0; }
    return m_pObject->GetPosition();
  }

  bool FileReader::IsExist( const String& FileName )
  {
    const Storage* p = GlobalPointer<Storage>::Get();
    return p->IsFileExist( FileName );
  }


  void FileReader::CreateObject()
  {
    Close();
    Storage* p = GlobalPointer<Storage>::Get();
    m_pObject = p->CreateFileReader();
  }

  void FileReader::SendMessage( const SPSTORAGEMESSAGE& pMess )
  {
    pMess->pObject = m_pObject;
    GlobalPointer<Storage>::Get()->SendMessage( pMess );
  }

}

