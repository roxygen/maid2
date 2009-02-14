#ifndef maid2_framework_xiph_oggpage_h
#define maid2_framework_xiph_oggpage_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"

#include<ogg/ogg.h>
#include<vector>

namespace Maid { namespace Xiph {

  class OggContainer;
  class OggStream;
  class OggPage
  {
    friend OggContainer;
    friend OggStream;
  public:
    OggPage();

    bool IsBeginOfStream() const;
    bool IsEndOfStream() const;
    bool IsContinued() const;

    int GetSerialNo() const;
    int GetPageNo() const;
    int GetPacketCount() const;

    ogg_int64_t GetGranulePosition() const;

    void Copy( std::vector<unsigned char>& header, std::vector<unsigned char>& body, const OggPage& SrcPage );

    std::string GetDebugString() const;

  private:
    ogg_page  m_Page;
  };

}}

#endif
