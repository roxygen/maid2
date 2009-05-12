#include"packfilecreater.h"
#include"packfileinfo.h"
#include"fileio/filewrite.h"
#include"fileio/filereadnormal.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/compress/compresszlib.h"
#include"../auxiliary/buffer.h"


namespace Maid
{

void PackFileCreater::BeginArchive( const String& ArchiveName, unt64 DivSize, const String& RootDirectory, const std::vector<FILEINFO>& FileInfo )
{
  m_ArchiveName = ArchiveName;
  m_RootDirectory = RootDirectory;
  m_FileInfo = FileInfo;

  m_DivSize = DivSize;


  m_Status.State = STATUS::STATE_EXECUTING;
  m_Status.Progress = 0;
  m_Status.Text = MAIDTEXT("処理の準備中");

  m_Thread.SetFunc( MakeThreadObject(&PackFileCreater::ThreadFunction,this) );
  m_Thread.Execute();

}

PackFileCreater::STATUS PackFileCreater::GetStatus()
{
  PackFileCreater::STATUS ret;

  {
    ThreadMutexLocker lock(m_StatusMutex);

    ret = m_Status;
  }

  return ret;
}

void   PackFileCreater::OnCancel()
{

}

void PackFileCreater::SetErrorText( const String& text )
{
  ThreadMutexLocker lock(m_StatusMutex);

  m_Status.Text  = text;
  m_Status.State = STATUS::STATE_ERROR;
}



unt PackFileCreater::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  const String tmpfile = MAIDTEXT("_tmp1.dat");
  const String elementfile = MAIDTEXT("_tmp2.dat");

  //  作業フォルダにコピー＆圧縮＆統合する
  Buffer  ElementInfo;

  unt64 ArchiveSize = 0;
  unt64 HeaderSize = 0;
  {
    //  FileWrite を使う方法があるんだけど、アーカイブファイルが２Ｇ越えると動作しなそうなので
    //  地味に行う
    FILE* hDstFile = NULL;

    hDstFile = ::fopen( String::ConvertMAIDtoSJIS(tmpfile).c_str(), "wb" );
    if( hDstFile==NULL ) { SetErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

    for( int i=0; i<(int)m_FileInfo.size(); ++i )
    {
      const FILEINFO& srcinfo = m_FileInfo[i];

      std::vector<unt08>  SrcFileImage; //  ファイルをメモリ上に置いておく場所
      std::vector<unt08>  DstFileImage; //  書き込むイメージ
      FileReadNormal hSrcFile( m_RootDirectory + srcinfo.SourceFile );

      const IFileRead::OPENRESULT ret =  hSrcFile.Open();
      if( ret!=IFileRead::OPENRESULT_OK ) { SetErrorText(MAIDTEXT("ファイルオープンに失敗")); return 0; }

      SrcFileImage.resize( hSrcFile.GetSize() );
      hSrcFile.Read( &(SrcFileImage[0]), hSrcFile.GetSize() );

      DstFileImage.resize( hSrcFile.GetSize() );

      PACKFILE::COMPRESSTYPE CompType = PACKFILE::COMPRESSTYPE_NONE;

      if( srcinfo.IsCompress )
      {
        CompressZLIB comp;
        const size_t size = comp.Encode( &(SrcFileImage[0]), SrcFileImage.size(), &(DstFileImage[0]), DstFileImage.size() );

        //  圧縮したのに膨らんでしまったら、キャンセル
        if( SrcFileImage.size() <= size )
        {
          DstFileImage = SrcFileImage;
          CompType = PACKFILE::COMPRESSTYPE_NONE;
        }else
        {
          DstFileImage.resize( size );
          CompType = PACKFILE::COMPRESSTYPE_ZLIB;
        }
      }else
      {
        DstFileImage = SrcFileImage;
        CompType = PACKFILE::COMPRESSTYPE_NONE;
      }

      //  ファイル情報を記録
      {
        std::wstring str = String::ConvertMAIDtoUNICODE(srcinfo.SourceFile);

        const unt32 Elementsize = sizeof(PACKFILE::ELEMENTINFO)-1 + str.length()*sizeof(wchar_t);
        const unt32 OldSize = ElementInfo.GetSize();

        ElementInfo.Resize( OldSize+Elementsize );

        PACKFILE::ELEMENTINFO* pInfo = (PACKFILE::ELEMENTINFO*)ElementInfo.GetPointer(OldSize);

        ZERO( pInfo, Elementsize );


        pInfo->CompressSize = DstFileImage.size();
        pInfo->CompressType = CompType;
        pInfo->Position = ArchiveSize;
        pInfo->FileSize = SrcFileImage.size();

        pInfo->StructSize = Elementsize;
        ::memcpy( pInfo->FileName, str.c_str(), str.length()*sizeof(wchar_t) );
      }

      //  書きんで次へ
      fwrite( &(DstFileImage[0]), DstFileImage.size(), 1, hDstFile );

      //  4byte境界にそろえる
      const int tmp = 4-(DstFileImage.size()&3);
      const unt08 tmpdat[] = { 0x00,0x00,0x00,0x00 };
      if( tmp!=0 )
      {
        fwrite( tmpdat, tmp, 1, hDstFile );
      }


      ArchiveSize += DstFileImage.size() + tmp;
    }

    fclose(hDstFile);
  }


  {
    //  ヘッダの圧縮&ファイルに書き込む
    const unt DivCount = (sizeof(PACKFILE::HEADER) + ElementInfo.GetSize() + ArchiveSize) / m_DivSize +1;
    const Buffer& SrcFileImage = ElementInfo;
    Buffer  DstFileImage; //  書き込むイメージ

    DstFileImage.Resize( SrcFileImage.GetSize() );

    PACKFILE::COMPRESSTYPE CompType = PACKFILE::COMPRESSTYPE_NONE;
    {
      CompressZLIB comp;
      const size_t size = comp.Encode( SrcFileImage.GetPointer(0), SrcFileImage.GetSize(), DstFileImage.GetPointer(0), DstFileImage.GetSize() );

      //  圧縮したのに膨らんでしまったら、キャンセル
      if( SrcFileImage.GetSize() <= size )
      {
        ::memcpy( DstFileImage.GetPointer(0), SrcFileImage.GetPointer(0), DstFileImage.GetSize() );
        CompType = PACKFILE::COMPRESSTYPE_NONE;
      }else
      {
        DstFileImage.Resize( size );
        CompType = PACKFILE::COMPRESSTYPE_ZLIB;
      }
    }

    {
      FILE* hFile = NULL;

      hFile = fopen( String::ConvertMAIDtoSJIS(elementfile).c_str(), "wb" );
      if( hFile==NULL ) { SetErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

      PACKFILE::HEADER Header;

      const unt64 HeaderAndIndexSize = sizeof(Header) + DstFileImage.GetSize();
      const unt64 arcoff = 4-(HeaderAndIndexSize&3);

      ZERO( &Header, sizeof(Header) );
      Header.FileCode = StrToBinary32('M','P','F','2');
      Header.Version  = PACKFILE::HEADER::VERSION_0x0100;
      Header.InfoSizeComp = DstFileImage.GetSize();
      Header.InfoSize     = SrcFileImage.GetSize();
      Header.CompressType = CompType;
      Header.ArchiveOffset= HeaderAndIndexSize+arcoff;
      Header.DiveCount = DivCount;

      fwrite( &Header, sizeof(Header), 1, hFile );
      fwrite( DstFileImage.GetPointer(0), DstFileImage.GetSize(), 1, hFile );
      if( arcoff!=0 )
      {
        const unt08 tmpdat[] = { 0xDE,0xAD,0xBE,0xEF };
        fwrite( tmpdat, arcoff, 1, hFile );
      }
      fclose( hFile );

      HeaderSize = Header.ArchiveOffset;
    }
  }



  //  ファイルの分割
  unt64 CopySize = ArchiveSize+HeaderSize;

  FILE* hHeader = fopen( String::ConvertMAIDtoSJIS(elementfile).c_str(), "rb" );
  FILE* hArchive = fopen( String::ConvertMAIDtoSJIS(tmpfile).c_str(), "rb" );

  FILE* hFileList[] = { hHeader, hArchive };
  int pos = 0;

  const unt64 READMAX = 10*1024*1024; //  一回に読み書きする最大量は１０Ｍ
  unt64 WriteSize = m_DivSize;  //  あと何byte書き込むか

  Buffer  ReadWork;

  ReadWork.Resize(READMAX);

  int FileCount = 0;  //  作ったファイル数

  FILE* hTargetFile = CreateArchiveFile(FileCount);
  if( hTargetFile==NULL ) { SetErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

  while( true )
  {
    if( CopySize==0 ) { break; }

    ReadWork.Resize(READMAX);

    FILE* hFile = hFileList[pos];

    const size_t readsize = fread( ReadWork.GetPointer(0), 1, ReadWork.GetSize(), hFile );

    if( readsize<READMAX )
    {
      ++pos;

      ReadWork.Resize(readsize);
    }

    if( ReadWork.GetSize() < WriteSize )
    {
      fwrite( ReadWork.GetPointer(0), 1, ReadWork.GetSize(), hTargetFile );
      WriteSize -= ReadWork.GetSize();
    }else
    {
      fwrite( ReadWork.GetPointer(0), 1, (unt32)WriteSize, hTargetFile );
      fclose(hTargetFile);
      ++FileCount;
      FILE* hTargetFile = CreateArchiveFile(FileCount);
      if( hTargetFile==NULL ) { SetErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

      fwrite( ReadWork.GetPointer(WriteSize), 1, ReadWork.GetSize()-(unt32)WriteSize, hTargetFile );

      WriteSize = m_DivSize - WriteSize;
    }


    if( feof(hArchive)!=0 ) { break; }
  }

  fclose(hHeader);
  fclose(hArchive);


  {
    ThreadMutexLocker lock(m_StatusMutex);
    m_Status.Text  = MAIDTEXT("終了しました");
    m_Status.State = STATUS::STATE_SUCCESS;
  }
  return 0;
}

FILE* PackFileCreater::CreateArchiveFile( int count )
{
  FILE* hTargetFile = fopen( String::ConvertMAIDtoSJIS(m_ArchiveName+String::PrintFormat("%02d",count)).c_str(), "wb" );

  return hTargetFile;
}

}