#ifndef maid2_sound_core_ibuffer_h
#define maid2_sound_core_ibuffer_h

#include"../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Sound {

  struct CREATEBUFFERPARAM
  {
    size_t  Length;   //  バッファサイズ（バイト単位）
    int     Channels; //  チャンネル数
    int     SamplesPerSecond;  //  １秒あたりのサンプル数
    int     BitPerSamples;    //  １サンプルあたりのビット数
    int     BlockSize;        //  １ブロックあたりのサイズ（バイト単位）
  };

  class IBuffer
	{
	public:
    virtual ~IBuffer(){}

    struct LOCKDATA
    {
      void*   pData1;
      size_t  Data1Length;
      void*   pData2;
      size_t  Data2Length;
    };

    virtual void Lock( size_t begin, size_t Length, LOCKDATA& data )=0;
    virtual void Unlock( LOCKDATA& data )=0;

    virtual void Play( bool IsLoop )=0;
    virtual void SetPosition( size_t pos )=0;

    virtual void SetVolume( float vol )=0;
    virtual void Stop()=0;
  };

  typedef	boost::shared_ptr<IBuffer>	SPBUFFER;

}}


#endif
