﻿#include"memorybuffer.h"

#include"debug/assert.h"

namespace Maid
{



static char TEMPORARY[] = "DEADBEEF"; //  初期化してないときorSize==0のときにGetPointer() した場合に返すバッファ。安全策


//! バッファの長さを変更する
/*!
    すでに書き込まれているデータの内容は保障されます
 
    @param	size	[i ]	新しい大きさ(バイト単位)
 */
void MemoryBuffer::Resize( size_t size )
{
  m_Image.resize( size );
}

//! データの取得
/*!
    @param	size	[i ]	取得する位置（バイト単位）

    @return 指定した場所のアドレス
 */
void* MemoryBuffer::GetPointer( size_t size )
{
  return _GetPointer(size);
}

//! データの取得
/*!
    @param	size	[i ]	取得する位置（バイト単位）

    @return 指定した場所のアドレス
 */
const void* MemoryBuffer::GetPointer( size_t size )const
{
  return const_cast<MemoryBuffer*>(this)->_GetPointer(size);
}

//! 確保されているバッファの長さ
/*!
    @return 長さ
 */
size_t MemoryBuffer::GetSize() const
{
  return m_Image.size();
}

void MemoryBuffer::Add( const void* pData, size_t length )
{
  if( length==0 ) { return ; }
  const size_t old_size = GetSize();

  Resize( old_size+length );

  memcpy( _GetPointer(old_size), pData, length );
}

void MemoryBuffer::Get( void* pDst, size_t pos, size_t size )const
{
  if( size==0 ) { return ; }
  memcpy( pDst, GetPointer(pos), size );
}



//! データの取得
/*!
    @param	size	[i ]	取得する位置（バイト単位）

    @return 指定した場所のアドレス
 */
void* MemoryBuffer::_GetPointer( size_t size )
{
  //  まだ確保されていないときは安全対策として静的メモリを返す
  if( m_Image.empty() ) { return TEMPORARY; }

  MAID_ASSERT( !(size<m_Image.size()), "指定された長さがありません " << size );
  return &(m_Image[size]);
}

}



