#include"filereadrange.h"

#include"../../auxiliary/debug/assert.h"


namespace Maid
{

/*!
 	@class	FileReadRange FileReadRange.h
 	@brief	ファイルの特定区間を１ファイルとして扱うクラス 
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FileReadRange::FileReadRange() : m_BeginPosition(0), m_Size(0)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileReadRange::~FileReadRange()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く
/*!
 	@param	FileName		[i ]	開くファイル名
 	@param	BeginPosition	[i ]	読み込み開始位置
 	@param	Size			[i ]	読み込み範囲
 
  @return オープンに成功したら OPENRESULT_OK
\n        失敗したら OPENRESULT_ERROR
 */
FileReadRange::OPENRESULT FileReadRange::Open( const String& FileName, size_t BeginPosition, size_t Size )
{
	Close();

  {
    const FileReadNormal::OPENRESULT ret = m_hFile.Open( FileName );
    if( ret!=FileReadNormal::OPENRESULT_OK ) { return OPENRESULT_ERROR; }
  }

	{
		//	開かれたファイルのファイルサイズを調べて　引数が大きすぎるようならエラー
		if( m_hFile.GetSize() < BeginPosition+Size )
		{
			MAID_ASSERT( m_hFile.GetSize() < BeginPosition+Size, 
        "指定されたファイルサイズが不正です" << BeginPosition << "-" << BeginPosition+Size );
		}
	}

	m_BeginPosition = BeginPosition;
	m_Size     = Size;

	m_hFile.Seek( FileRead::POSITION_BEGIN, m_BeginPosition );

  return OPENRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 *	引数などは FileRead::Read() を参照すること
 */
size_t FileReadRange::Read( void* pData, size_t Size )
{
	const size_t ReadSize = std::min(Size,m_Size-GetPosition());
	const size_t ret = m_hFile.Read( pData, ReadSize );

	return ret;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
 *	引数などは FileRead::GetSize() を参照すること
 */
size_t FileReadRange::GetSize() const
{
	return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 *	引数などは FileRead::GetPosition() を参照すること
 */
size_t FileReadRange::GetPosition() const
{
	return m_hFile.GetPosition()-m_BeginPosition;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 *	引数などは FileRead::Seek() を参照すること
 */
void FileReadRange::Seek( FileRead::POSITION Pos, int Size)
{
	size_t NewPos;

	switch( Pos )
	{
	case FileRead::POSITION_BEGIN:
		{
			if( Size<0 )  { NewPos = 0;    }
			else          { NewPos = Size; }
		}break;
	case FileRead::POSITION_END:
		{
			if( 0<m_Size )  { NewPos = m_Size;    }
			else            { NewPos = Size; }
		}break;

	case FileRead::POSITION_CURRENT:
		{
			NewPos = GetPosition() + Size;
			if( NewPos<0		  ) { NewPos = 0;			}
			if( m_Size<NewPos	) { NewPos = m_Size;    }

		}break;
	}

	m_hFile.Seek( FileRead::POSITION_BEGIN, m_BeginPosition+NewPos );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
 *	引数などは FileRead::Close() を参照すること
 */
void FileReadRange::Close()
{
	m_hFile.Close();
	m_BeginPosition = 0;
	m_Size    = 0;
}


}
