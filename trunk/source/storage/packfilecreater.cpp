#include"packfilecreater.h"
#include"packfileinfo.h"
#include"fileio/filewrite.h"
#include"fileio/filereadnormal.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/compress/compresszlib.h"
#include"../auxiliary/buffer.h"

#include<boost/filesystem/path.hpp>
#include<boost/filesystem/fstream.hpp>

namespace Maid
{

void PackFileCreater::BeginArchive( const String& ArchiveName, unt64 DivSize, const String& RootDirectory, const std::vector<FILEINFO>& FileInfo )
{
  m_ArchiveName = ArchiveName;
  m_RootDirectory = RootDirectory;
  m_FileInfo = FileInfo;

  m_DivSize = DivSize;

  m_Thread.SetFunc( MakeThreadObject(&PackFileCreater::ThreadFunction,this) );
  m_Thread.Execute();

}

PackFileCreater::STATUS PackFileCreater::GetStatus()
{

  return m_Status;
}

void   PackFileCreater::OnCancel()
{

}


unt PackFileCreater::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  const std::string tmpdir = "maid_tmp";
  const String tmpfile = String::ConvertSJIStoMAID(tmpdir) + MAIDTEXT("tmp1.dat");
  const String elementfile = String::ConvertSJIStoMAID(tmpdir) + MAIDTEXT("tmp2.dat");

  //  作業フォルダの作成
  boost::filesystem::create_directory( boost::filesystem::path(tmpdir) );

  //  作業フォルダにコピー＆圧縮＆統合する
  Buffer  ElementInfo;

  unt64 ArchiveSize = 0;
  {
    FileWrite hDstFile;

    const FileWrite::OPENRESULT ret = hDstFile.Open( tmpfile, FileWrite::OPENOPTION_NEW );
    if( ret!=FileWrite::OPENRESULT_OK ) { MAID_WARNING("ファイル作成に失敗"); return 0; }

    for( int i=0; i<(int)m_FileInfo.size(); ++i )
    {
      const FILEINFO& srcinfo = m_FileInfo[i];

      std::vector<unt08>  SrcFileImage; //  ファイルをメモリ上に置いておく場所
      std::vector<unt08>  DstFileImage; //  書き込むイメージ
      FileReadNormal hSrcFile( m_RootDirectory + MAIDTEXT("\\") + srcinfo.SourceFile );

      const IFileRead::OPENRESULT ret =  hSrcFile.Open();
      if( ret!=IFileRead::OPENRESULT_OK ) { MAID_WARNING("ファイルオープンに失敗"); return 0; }

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


      //  書き込む
      const unt64 FilePos = hDstFile.GetSize();
      hDstFile.Write( &(DstFileImage[0]), DstFileImage.size() );
      ArchiveSize += DstFileImage.size();

      //  ファイル情報を記録して次へ

      {
        std::wstring str = String::ConvertMAIDtoUNICODE(srcinfo.SourceFile);

        const unt32 Elementsize = sizeof(PACKFILE::ELEMENTINFO)-1 + str.length()*sizeof(wchar_t);
        const unt32 OldSize = ElementInfo.GetSize();

        ElementInfo.Resize( OldSize+Elementsize );

        PACKFILE::ELEMENTINFO* pInfo = (PACKFILE::ELEMENTINFO*)ElementInfo.GetPointer(OldSize);

        pInfo->CompressSize = DstFileImage.size();
        pInfo->CompressType = CompType;
        pInfo->Position = FilePos;
        pInfo->FileSize = SrcFileImage.size();

        pInfo->StructSize = Elementsize;
        ::memcpy( pInfo->FileName, str.c_str(), str.length()*sizeof(wchar_t) );
      }
    }
  }

  //  ヘッダの圧縮&ファイルに書き込む

  {
    const unt DivCount = (sizeof(PACKFILE::HEADER) + ElementInfo.GetSize() + ArchiveSize) / m_DivSize;
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
      FileWrite hFile;

      const FileWrite::OPENRESULT ret =  hFile.Open( elementfile, FileWrite::OPENOPTION_NEW );
      if( ret!=FileWrite::OPENRESULT_OK ) { MAID_WARNING("ファイルオープンに失敗"); return 0; }

      PACKFILE::HEADER Header;

      Header.InfoSizeComp = DstFileImage.GetSize();
      Header.InfoSize     = SrcFileImage.GetSize();
      Header.CompressType = CompType;
      Header.ArchiveOffset= sizeof(Header) + ElementInfo.GetSize();
      Header.DiveCount = DivCount;

      hFile.Write( &Header, sizeof(Header) );
      hFile.Write( DstFileImage.GetPointer(0), DstFileImage.GetSize() );

    }
  }


  //  分割数を求めて、分割する

  return 0;
}

}