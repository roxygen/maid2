#ifndef framework_xiph_oggcontainer_h
#define framework_xiph_oggcontainer_h


#include"../../source/config/define.h"
#include"../../storage/filereader.h"
#include"../../storage/filereadersync.h"
#include"../../auxiliary/string.h"

#include<ogg/ogg.h>
#include<string>

#include"oggpage.h"

#include"../../storage/fileio/filereadnormal.h"

namespace Maid { namespace Xiph {

  class OggContainer
  {
  public:
    OggContainer();
    ~OggContainer();

    void Initialize( const String& FileName );
    void Finalize();


    const OggPage& GetCurrentPage() const;
    void NextPage();

    bool IsEnd() const;

    std::string GetDebugString() const;

  private:
    FileReader  m_hFile;
    OggPage   m_CurrentPage;
    ogg_sync_state  m_State;
    size_t  m_ReadSize;

    size_t  m_ReadingSize;
    bool    m_IsEnd;
    bool    m_IsLastPage;
  };

}}

#endif
