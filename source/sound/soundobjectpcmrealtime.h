#ifndef maid2_sound_soundobjectpcmrealtime_h
#define maid2_sound_soundobjectpcmrealtime_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"
#include"pcmreader/ipcmreader.h"

namespace Maid {

  class SoundObjectPCMRealTime
    : public ISoundObject
  {
  public:
    SoundObjectPCMRealTime();

		void Initialize( const SPPCMREADER& pDecoder );

    void Update();
    void Play();
    void Stop();
    void SetPosition( double time );
    void SetVolume( double volume );
    void SetLoopState( bool IsLoop );
    bool IsPlay()const;
    double GetPosition() const;
    double GetVolume() const;

 
  private:
    void UpdateBuffer();
    size_t CalcLength( size_t prev, size_t now )const;
    size_t CalcUpdateScape()const;

  private:
    Sound::SPBUFFER m_pBuffer;
		SPPCMREADER     m_pDecoder;

    size_t  m_NowPlayPosition;    //  再生位置
    size_t  m_PrevBufferPosition; //  前回の Update() 時にあった 再生位置
    size_t  m_WrittedBufferPosition;
  };
}


#endif

