#ifndef framework_xiph_oggpage_h
#define framework_xiph_oggpage_h


#include"../../config/define.h"

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

    int GetSerialNo() const;
    int GetPageNo() const;

    ogg_int64_t GetGranulePosition() const;

    void Copy( std::vector<unsigned char>& header, std::vector<unsigned char>& body, const OggPage& SrcPage );


  private:
    ogg_page  m_Page;
  };

}}

#endif
