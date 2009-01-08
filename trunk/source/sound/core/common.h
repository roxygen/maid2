#ifndef maid2_sound_core_common_h
#define maid2_sound_core_common_h

#include"../../config/define.h"

#include<stdlib.h>

namespace Maid { namespace Sound {

  struct PCMFORMAT
  {
    PCMFORMAT():SamplesPerSecond(0),BitPerSamples(0),Channels(0){}
    PCMFORMAT( size_t sample, size_t bit, size_t chan ):SamplesPerSecond(sample),BitPerSamples(bit),Channels(chan){}

    size_t SamplesPerSecond;  //  １秒あたりのサンプル数
    size_t BitPerSamples;     //  １サンプルあたりのビット数
    size_t Channels;          //  チャンネル数

    size_t CalcBlockSize() const { return Channels * BitPerSamples/8; }
    size_t CalcBytePerLength()const { return BitPerSamples/8*Channels*SamplesPerSecond; }
    size_t CalcLength( float time )const
    {
      if( time<0 ) { return 0; }

      const float byte_per_second = float(CalcBytePerLength());
      const size_t pos = size_t(byte_per_second*time+0.5f);
      const size_t block = CalcBlockSize();
      return pos/block*block;
    }

    float CalcTime( size_t pos ) const
    {
      const float p = (float)pos;
      const float byte_per_second = float(CalcBytePerLength());

      return p / byte_per_second;
    }
  };


}}


#endif
