#ifndef maid2_sound_decoder_ipcmdecoder_h
#define maid2_sound_decoder_ipcmdecoder_h 


#include"../../config/define.h"
#include<stdlib.h>

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IPCMDecoder
  {
  public:
    virtual ~IPCMDecoder(){};

    enum OPENRESULT
    {
      OPENRESULT_OK =0,
      OPENRESULT_ERROR,
    };

    virtual OPENRESULT Open( const void* pSrc, size_t size )=0;
    virtual void Close()=0;

    virtual size_t  Read( void* pDst, size_t size )=0;
    virtual void    SetPosition( size_t Offset )=0;
    virtual size_t  GetPosition()		const=0;
    virtual size_t  GetLength()			const=0;
    virtual size_t  GetSamplesPerSec()	const=0;
    virtual size_t  GetChannels()		const=0;
    virtual size_t  GetBitsPerSamples()	const=0;
  };

  typedef	boost::shared_ptr<IPCMDecoder>	SPPCMDECODER;

}

#endif