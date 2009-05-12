#include"../../source/framework/application.h"
#include"../../source/storage/packfilecreater.h"

using namespace Maid;

void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }

  Storage storage;
  storage.Initialize(); //  これもおまじない

  JobPool jobpool;
  jobpool.Initialize(); //  これも(ry

  {
    XMLFileReader hFile;

    hFile.Load( MAIDTEXT("filearchiver_config.xml") );

    while( true )
    {
      if( hFile.IsLoading() ) { ::Sleep(1); continue; }
      break;
    }

  }

/*
  class XMLFileReader
  {
  public:
    void Load( const String& FileName );
    bool IsLoading() const;

    bool IsEmpty() const;


    XMLReader& GetReader();
*/




  PackFileCreater m_Packer;


  PackFileCreater::FILEINFO FileList[] =
  {
    PackFileCreater::FILEINFO(MAIDTEXT("nc1429.png"),true),
    PackFileCreater::FILEINFO(MAIDTEXT("nc1673.bmp"),false),
  };

  m_Packer.BeginArchive( MAIDTEXT("arc.dat"), 1024*1024*600, MAIDTEXT(""),
    std::vector<PackFileCreater::FILEINFO>(FileList,FileList+NUMELEMENTS(FileList)) );

  while( true )
  {
    const PackFileCreater::STATUS stat = m_Packer.GetStatus();

    if( stat.State!=PackFileCreater::STATUS::STATE_EXECUTING ) { break; }

    Sleep(100);

  }
}
