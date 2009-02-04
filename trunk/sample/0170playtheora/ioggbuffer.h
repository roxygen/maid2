#ifndef ioggbuffer_h
#define ioggbuffer_h


#include"../../source/config/define.h"
#include"../../source/graphics/surface.h"

#include <boost/smart_ptr.hpp>




class IOggDecodedBuffer
{
public:
};

typedef boost::shared_ptr<IOggDecodedBuffer> SPOGGDECODEDBUFFER;


class OggDecodedBufferImage : public IOggDecodedBuffer
{
public:
  Maid::SIZE2DI SizeY;  //  YUV の Y 平面の大きさ
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
  std::vector<int16>  Buffer;
};


typedef boost::shared_ptr<OggDecodedBufferPCM> SPOGGDECODEDBUFFERPCM;


struct OGGDECODEDINFO
{
  double Time;
  SPOGGDECODEDBUFFER pBuffer;
};

#endif
