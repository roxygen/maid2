﻿#ifndef maid2_sound_common_h
#define maid2_sound_common_h

#include"../config/define.h"

#include<stdlib.h>

namespace Maid { 


  /*!
      @struct   PCMFORMAT common.h
      @brief    PCM波形のフォーマットを表すクラス
  */
  struct PCMFORMAT
  {
    PCMFORMAT():SamplesPerSecond(0),BitPerSamples(0),Channels(0){}
    PCMFORMAT( const PCMFORMAT& rha ):SamplesPerSecond(rha.SamplesPerSecond),BitPerSamples(rha.BitPerSamples),Channels(rha.Channels){}
    PCMFORMAT( size_t sample, size_t bit, size_t chan ):SamplesPerSecond(sample),BitPerSamples(bit),Channels(chan){}

    size_t SamplesPerSecond;  //  １秒あたりのサンプル数
    size_t BitPerSamples;     //  １サンプルあたりのビット数
    size_t Channels;          //  チャンネル数

    //! １ブロックの大きさを計算する
    /*!
 	      @return ブロックの大きさ（バイト単位）
     */
    size_t CalcBlockSize() const { return Channels * BitPerSamples/8; }

    //! １秒あたりの大きさを計算する
    /*!
 	      @return １秒あたりの大きさ（バイト単位）
     */
    size_t CalcBytePerLength()const { return BitPerSamples/8*Channels*SamplesPerSecond; }

    //! 指定した時間の大きさを計算する
    /*!
        @param  time  [i ]  時間（1.0f == １秒）

 	      @return 大きさ（バイト単位）
     */
    size_t CalcLength( double time )const
    {
      if( time<0 ) { return 0; }

      const double byte_per_second = double(CalcBytePerLength());
      const size_t pos = size_t(byte_per_second*time+0.5f);
      const size_t block = CalcBlockSize();
      return pos/block*block;
    }

    //! 長さが何秒あるか計算する
    /*!
        @param  pos  [i ]  長さ（バイト単位）

 	      @return 時間（1.0f == １秒）
     */
    double CalcTime( size_t pos ) const
    {
      const double p = (double)pos;
      const double byte_per_second = double(CalcBytePerLength());

      return p / byte_per_second;
    }
  };


}


#endif
