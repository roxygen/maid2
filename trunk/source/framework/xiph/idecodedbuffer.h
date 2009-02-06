#ifndef framework_xiph_idecodedbuffer_h
#define framework_xiph_idecodedbuffer_h


#include"../../config/define.h"
#include"../../auxiliary/buffer.h"

#include <boost/smart_ptr.hpp>
#include <list>



namespace Xiph {

  class IBuffer
  {
  public:
    virtual ~IDecodedBuffer(){}
  };

  typedef boost::shared_ptr<IDecodedBuffer> SPDECODEDBUFFER;
  typedef std::list<SPOGGDECODEDBUFFER> OGGDECODEDBUFFERLIST;


  class OggDecodedBufferImage : public IOggDecodedBuffer
  {
  public:
    SIZE2DI SizeY;  //  YUV の Y 平面の大きさ
    int           PitchY; //  Y平面のピッチ
    std::vector<unt08>  BitsY;  //  Y平面のデータ

    Maid::SIZE2DI SizeUV;  //  YUV の UV 平面の大きさ
    int           PitchUV; //  UV平面のピッチ
    std::vector<unt08>  BitsU;  //  U平面のデータ
    std::vector<unt08>  BitsV;  //  V平面のデータ
  };

  typedef boost::shared_ptr<OggDecodedBufferImage> SPOGGDECODEDBUFFERIMAGE;

  class OggDecodedBufferPCM : public IOggDecodedBuffer
  {
  public:
    Maid::Buffer  WaveData;
  };


  typedef boost::shared_ptr<OggDecodedBufferPCM> SPOGGDECODEDBUFFERPCM;




}

#endif
