#ifndef maid2_sound_soundobjectstatic_h
#define maid2_sound_soundobjectstatic_h

#include"../config/define.h"
#include"isoundobject.h"
#include"core/ibuffer.h"

namespace Maid {

  class SoundObjectPCMStatic : public ISoundObject
  {
  public:
    SoundObjectPCMStatic():ISoundObject(TYPE_STATIC){}

		void Initialize( const Sound::SPBUFFER& pBuffer, const boost::shared_ptr<std::vector<unt08> >& pData );

    void Update();
    void Play();
    void Stop();
    void SetPosition( float time );
    void SetVolume( float volume );
    void SetJumpPoint( const std::vector<JUMPPOINT>& list );
    void SetLoopState( bool IsLoop );
    bool IsPlay()const;
    float GetPosition() const;

    const Sound::SPBUFFER& GetBuffer() const { return m_pBuffer; }
    const boost::shared_ptr<std::vector<unt08> >& GetData() const { return m_pData; }

  private:
    Sound::SPBUFFER  m_pBuffer;
    bool m_IsLoop;
    boost::shared_ptr<std::vector<unt08> >  m_pData;  //  生データ
  };



}


#endif
