#ifndef framework_oggplayer_h
#define framework_oggplayer_h

#include"../auxiliary/thread.h"
#include"../auxiliary/string.h"
#include"../auxiliary/buffer.h"
#include"../auxiliary/timer.h"
#include"../sound/common.h"

#include"xiph/oggcontainer.h"
#include"xiph/oggstream.h"
#include"xiph/samplecache.h"
#include"xiph/decodertheora.h"
#include"xiph/decodervorbis.h"


#include<map>
#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{

  class OggPlayer
  {
  public:
    OggPlayer();

    void Initialize( const String& FileName );

    bool IsInitialized() const;
    bool IsSeeking() const;

    struct FILEINFO
    {
      struct IMAGE
      {
        SIZE2DI EncodedSize;    //  データとして入っている大きさ
        SIZE2DI DisplaySize;    //  表示しなくてはいけない大きさ
        POINT2DI DisplayOffset; //  配置座標
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
        PCMFORMAT Format;
      };

      bool IsPCM; //  サウンドがあるか？
      PCM  Pcm;

      FILEINFO()
        :IsImage(false)
        ,IsPCM(false)
      {
      }
    };

    const FILEINFO& GetFileInfo() const;


    void FlushImage( double time, Xiph::SPBUFFERIMAGE& pOutput );

    void FlushPCM( double time, Buffer& Output );

    void Seek( double time );

  private:

    bool IsSetSeek()const;

  private:
    Xiph::OggContainer m_Buffer;

    struct DATA
    {
      Xiph::OggStream   Stream;
      Xiph::SPDECODER   pDecoder;
      Xiph::SampleCache Cache;
    };

   #pragma COMPILERMSG("TODO:コピー処理を書いてないのでスマポにしておく。そのうちなおす")	
    typedef std::map<int,boost::shared_ptr<DATA> > BINDDATA;
    BINDDATA m_BindData;  //  こいつにアクセスするとき同期を取ってないんだけど
                          //  STATE_INITIALIZING 以降は変更を加えることがないのでいらないです。

    enum
    {
      DECODER_EMPTY = -1,
    };


    int m_TheoraSerial;
    int m_VorbisSerial;


    enum STATE
    {
      STATE_EMPTY,
      STATE_INITIALIZING,
      STATE_SEEKING,
      STATE_WORKING,
      STATE_FINALIZING,
    };

    STATE m_State;

    double m_SeekPosition;  //  シーク命令が出たときに設定される値（単位は秒）シークしないなら負数

    String    m_FileName;
    FILEINFO  m_FileInfo;

    Timer     m_Timer;
  private:
    void PageSeek( int Serial );
    void Sleep( volatile ThreadController::BRIGEDATA& brige );
    bool IsCacheFull( int DecoderSerial )const;
    void StreamCheck( volatile ThreadController::BRIGEDATA& brige );

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    void InitializeStream( const String& FileName );
    void FinalizeStream();
    void PlayDecode( double begintime, volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };
}

#endif
