#ifndef framework_xiph_oggstream_h
#define framework_xiph_oggstream_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"

#include<ogg/ogg.h>
#include<boost/smart_ptr.hpp>

#include"oggpage.h"
#include"oggpacket.h"


namespace Maid { namespace Xiph {

  class OggStream
  {
  public:
    OggStream();
    ~OggStream();

    void Initialize( int serial );
    void Finalize();

    void PageIn( const OggPage& Page );

    bool PacketOut( OggPacket& out );

    int GetSerialNo() const;

    bool IsEnd() const;

    std::string GetDebugString() const;

  private:
    ogg_stream_state m_Stream;
  };

  typedef boost::shared_ptr<OggStream> SPOGGSTREAM;

}}

#endif
