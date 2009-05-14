#include"../../config/define.h"
#include"../../source/framework/win32/shell.h"
#include"../../source/storage/packfilecreater.h"
#include"../../source/storage/xmlfile.h"
#include"../../source/auxiliary/debug/warning.h"


#include <boost/regex.hpp>


using namespace Maid;


struct ARCHIVECONFIG
{
  String  ArchiveName;    //  出力する名前
  unt32   DivideSize;     //  分割サイズ
  String  RootDirectory;  //  アーカイブ検索の基準となるディレクトリ
  std::vector<boost::wregex> ArchiveFile;    //  アーカイブ化するファイル（ワイルドカード）
  std::vector<boost::wregex> NoArchiveFile;  //  アーカイブに含めないファイル(正規表現)
  std::vector<boost::wregex> NoComplessFile; //  圧縮しないファイル(正規表現)
};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したディレクトリ内にあるファイル名を列挙する
/*!
 	ディレクトリ内のディレクトリも検索します
 
 	\param	Dir		[i ]	検索するディレクトリ名
 	\param	List	[ o]	列挙先
 */
void	SerachFile( const String& Dir, std::list<String>& List )
{
	Shell::FINDOBJECTLIST	CandidateFile;

  if( Dir.empty() ) { CandidateFile = Shell::PickupFile( MAIDTEXT("*") ); }
  else { CandidateFile = Shell::PickupFile( Dir+MAIDTEXT("\\*.*") ); }
	

	for( Shell::FINDOBJECTLIST::iterator ite=CandidateFile.begin(); ite!=CandidateFile.end(); ++ite )
	{
		const String str = ite->GetFileName();

		if( str[0]=='.' ) { continue; }

		switch( ite->GetFileType() )
		{
		case Shell::FindObject::TYPE_FILE:
			{
        if( Dir.empty() ) { List.push_back( String::ToLower(str) ); }	//<- ついでに小文字化しておく
        else { List.push_back( String::ToLower(Dir+MAIDTEXT("\\")+str) ); }	//<- ついでに小文字化しておく
        
			}break;

		case Shell::FindObject::TYPE_DIRECTORY:
			{
        if( Dir.empty() ) { SerachFile( str, List ); }
        else { SerachFile( Dir+MAIDTEXT("\\")+str, List ); }
				
			}break;
		}
	}
}


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

  ARCHIVECONFIG ArchiveConfig;

  { //  設定ファイルの読み込み
    XMLFileReader ConfigFile;
    {
      ConfigFile.Load( MAIDTEXT("filearchiver_config.xml") );

      while( true )
      {
        if( ConfigFile.IsLoading() ) { ::Sleep(1); continue; }
        break;
      }
    }

    XMLReader& reader = ConfigFile.GetReader();

    {
      reader.AscendNode();
      while( true )
      {
        if( reader.IsEndNode() ) { break; }

        if( reader.GetNodeType()==XMLReader::NODETYPE_ELEMENT )
        {
          ARCHIVECONFIG& conf = ArchiveConfig;
          const String nodename = reader.GetNodeName();
          std::string str = String::ConvertMAIDtoSJIS(nodename);
          if     ( nodename==MAIDTEXT("archivename") )  { conf.ArchiveName = reader.GetString(); }
          else if( nodename==MAIDTEXT("dividesize" ) )  { conf.DivideSize = reader.GetInteger(); }
          else if( nodename==MAIDTEXT("rootdirectory")) { conf.RootDirectory = reader.GetString(); }
          else if( nodename==MAIDTEXT("archivefile") )  { conf.ArchiveFile.push_back(boost::wregex(String::ConvertMAIDtoUNICODE(reader.GetString()))); }
          else if( nodename==MAIDTEXT("noarchivefile")) { conf.NoArchiveFile.push_back(boost::wregex(String::ConvertMAIDtoUNICODE(reader.GetString()))); }
          else if( nodename==MAIDTEXT("nocomplessfile")){ conf.NoComplessFile.push_back(boost::wregex(String::ConvertMAIDtoUNICODE(reader.GetString()))); }
        }

        reader.NextNode();
      }
      reader.DescendNode();
    }
  }

  std::vector<PackFileCreater::FILEINFO>  ArchiveFileList;

  {
    //  アーカイブするファイルの選定
    const ARCHIVECONFIG& conf = ArchiveConfig;


    Shell::SetCurrentDirectory(conf.RootDirectory);

    std::list<String> ObjectList;
    SerachFile( MAIDTEXT(""), ObjectList );


    for( std::list<String>::iterator ite=ObjectList.begin(); ite!=ObjectList.end(); ++ite )
    {
      const std::wstring source = String::ConvertMAIDtoUNICODE(*ite);

      bool IsAdd  = false;
      bool IsComp = true;

      for( int i=0; i<(int)conf.ArchiveFile.size(); ++i )
      {
        if( boost::regex_match(source, conf.ArchiveFile[i] ) ) { IsAdd = true; break; }
      }

      for( int i=0; i<(int)conf.NoArchiveFile.size(); ++i )
      {
        if( boost::regex_match(source, conf.NoArchiveFile[i] ) ) { IsAdd = false; break; }
      }

      for( int i=0; i<(int)conf.NoComplessFile.size(); ++i )
      {
        if( boost::regex_match(source, conf.NoComplessFile[i] ) ) { IsComp = false; break; }
      }

      if( IsAdd )
      {
        ArchiveFileList.push_back(
            PackFileCreater::FILEINFO( *ite, IsComp )
          );
      }
    }
  }


  if( ArchiveFileList.empty() ) { return ; }


  PackFileCreater m_Packer;



  m_Packer.BeginArchive( ArchiveConfig.ArchiveName, ArchiveConfig.DivideSize, 
    MAIDTEXT(""), ArchiveFileList );

  while( true )
  {
    const PackFileCreater::STATUS stat = m_Packer.GetStatus();

    if( stat.State!=PackFileCreater::STATUS::STATE_EXECUTING ) { break; }

    Sleep(100);

  }
}
