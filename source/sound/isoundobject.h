#ifndef maid2_sound_isoundobject_h
#define maid2_sound_isoundobject_h

#include"../config/define.h"
#include<boost/smart_ptr.hpp>
#include"soundmessagebase.h"

namespace Maid {



  class ISoundObject
	{
	public:
    virtual ~ISoundObject(){}

    virtual void Update()=0;
    virtual void Play( bool IsLoop )=0;
    virtual void Stop();
    virtual void SetPosition( float pos )=0;
    virtual void SetVolume( float volume )=0;
    virtual void SetJumpPoint( const std::vector<JUMPPOINT>& list )=0;
  };

  typedef	boost::shared_ptr<ISoundObject>	SPSOUNDOBJECT;

}


#endif
