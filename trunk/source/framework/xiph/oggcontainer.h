#ifndef oggcontainer_h
#define oggcontainer_h


#include"../../source/config/define.h"
#include"../../storage/filereadersync.h"
#include"../../auxiliary/string.h"

#include<ogg/ogg.h>

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

    bool PageOut( OggPage& Page );

    bool IsEnd() const;

  private:
    void UpdateBuffer();

  private:
//    FileReaderSync  m_hFile;
    FileReadNormal  m_hFile;
    ogg_sync_state  m_State;
  };

}}

#endif
