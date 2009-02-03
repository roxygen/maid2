#ifndef decoder_h
#define decoder_h

#include"../../source/auxiliary/thread.h"
#include"../../source/graphics/surface.h"

#include"oggcontainer.h"
#include"oggstream.h"
#include"ioggdecoder.h"
#include"oggdecodertheora.h"
#include"oggdecodervorbis.h"


#include<map>
#include<list>
#include<boost/smart_ptr.hpp>


class Decoder
{
public:
  Decoder();

  void Initialize();

  bool IsInitialized() const;

  struct FILEINFO
  {
    struct IMAGE
    {
      Maid::SIZE2DI EncodedSize;    //  データとして入っている大きさ
      Maid::SIZE2DI DisplaySize;    //  表示しなくてはいけない大きさ
      Maid::POINT2DI DisplayOffset; //  配置座標
      int FpsNumerator;       //  FPS の分子
      int FpsDenominator;     //  FPS の分母
      int AspectNumerator;    //  アスペクト の分子
      int AspectDenominator;  //  アスペクト の分母

      enum PIXELFORMAT
      {
        PIXELFORMAT_YUV444,
        PIXELFORMAT_YUV422,
        PIXELFORMAT_YUV420,
      };

      PIXELFORMAT PixelFormat;
    };
    bool IsImage; //  画像があるか？
    IMAGE  Image;

    struct PCM
    {
      int Channel;
    };

    bool IsPCM; //  サウンドがあるか？
    PCM  PCM;
  };

  void GetFileInfo( FILEINFO& Info ) const;


  void FlushImage( double time, SPOGGDECODEDBUFFERIMAGE& pOutput );

  void FlushPCM( std::vector<unt08>& Buffer );


private:
  OggContainer m_Buffer;

  struct DATA
  {
    OggStream     Stream;
    SPOGGDECODER  pDecoder;

    Maid::ThreadMutex Mutex;
    std::list<OGGDECODEDINFO>  DecodedInfo;
  };

  typedef std::map<int,DATA> BINDDATA;
  BINDDATA m_BindData;

  int m_TheoraSerial;
  int m_VorbisSerial;


  enum STATE
  {
    STATE_EMPTY,
    STATE_INITIALIZING,
    STATE_WORKING,
    STATE_FINALIZING,
  };

  STATE m_State;

private:
  int Sleep( DATA& dat, volatile Maid::ThreadController::BRIGEDATA& brige ); 

private:
  unt ThreadFunction( volatile Maid::ThreadController::BRIGEDATA& state );
  Maid::ThreadController  m_Thread;
};

#endif
