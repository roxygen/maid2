﻿#include"filereadmemory.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

/*!
  @class  FileReadMemory FileReadMemory.h
  @brief  メモリ上においてあるデータをファイルとして扱うクラス
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ(ポインタ先を保証しておくバージョン)
/*!
    @param	pBegin	[i ]	読み込み開始アドレス
    @param	Size    [i ]	読み込み範囲
 */
FileReadMemory::FileReadMemory( const boost::shared_array<unt08>& pBegin, size_t Size )
  : m_pShared(pBegin)
  , m_pBegin(pBegin.get())
  , m_Size(Size)
  , m_Position(0)
  , m_IsOpen(false)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ(ポインタ先を保証しないバージョン)
/*!
 	  このインスタンスがなくなるまで pBegin を解放してはいけません
   
 	  @param	pBegin	[i ]	読み込み開始アドレス
 	  @param	Size    [i ]	読み込み範囲 
 */
FileReadMemory::FileReadMemory( const void* pBegin, size_t Size )
  : m_pBegin((unt08*)pBegin)
  , m_Size(Size)
  , m_Position(0)
  , m_IsOpen(false)
{

}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileReadMemory::~FileReadMemory()
{
  Close();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く
/*!
    引数などは IFileRead::Open() を参照
 */
FileReadMemory::OPENRESULT  FileReadMemory::Open()
{
  m_IsOpen = true;
  m_Position   = 0;

  return OPENRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
    引数などは IFileRead::Close() を参照すること
 */
void FileReadMemory::Close()
{
  m_IsOpen = false;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているか？
/*!
 	  引数などは IFileRead::IsOpen() を参照すること
 */
bool FileReadMemory::IsOpen() const
{
  return m_IsOpen;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 *	引数などは IFileRead::Read() を参照すること
 */
size_t FileReadMemory::Read( void* pData, size_t Size )
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  const size_t ReadSize = std::min( m_Size - m_Position, Size );

  if( ReadSize<=0 ) { return 0; }

  memcpy( pData, &(m_pBegin[m_Position]), ReadSize );

  m_Position += ReadSize;

  return ReadSize;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
 *	引数などは FileRead::GetSize() を参照すること
 */
size_t FileReadMemory::GetSize() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 *	引数などは FileRead::GetPosition() を参照すること
 */
size_t FileReadMemory::GetPosition() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  return m_Position;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 *	引数などは FileRead::Seek() を参照すること
 */
void FileReadMemory::Seek( IFileRead::POSITION ePos, int Size )
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  switch( ePos )
  {
  case IFileRead::POSITION_BEGIN:
    {
      if( Size<0 )  { m_Position = 0; }
      else          { m_Position = Size; }
    }break;
  case IFileRead::POSITION_END:
    {
      if( 0<Size )  { m_Position = m_Size; }
      else          { m_Position = m_Size+Size; }
    }break;

  case IFileRead::POSITION_CURRENT:
    {
      m_Position += Size;
      if( m_Position < 0 )      { m_Position = 0; }
      if( m_Size < m_Position )	{ m_Position = m_Size; }
    }break;
  }
}


}
