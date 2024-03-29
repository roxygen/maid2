﻿#include"soundcore.h"

#include"pcmreader/pcmbufferlist.h"
#include"pcmreader/vorbisfile.h"
#include"pcmreader/wavefile.h"


namespace Maid {


//! 指定したデータに対応しているPCMリーダーの作成
/*!
    @param  pData [i ] 調べたいデータ
    @param  Size  [i ] データの長さ（バイト単位）

    return PCMリーダー。見つからなかったら SPPCMREADER()
 */
SPPCMREADER SoundCore::CreatePCMReader( const void* pData, size_t Size ) const
{
  SPPCMREADER pDec;

  //  先頭の数バイトを見て、デコーダを作るのがいいか

  if( WaveFile  ::Check(pData,Size) ) { pDec.reset( new WaveFile(pData,Size)   ); }
  else if( VorbisFile::Check(pData,Size) ) { pDec.reset( new VorbisFile(pData,Size) ); }

  return pDec;
}


}


