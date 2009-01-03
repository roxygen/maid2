#ifndef maid2_sound_soundfile_h
#define maid2_sound_soundfile_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"


#include"soundcore.h"


namespace Maid {

  namespace KEEPOUT
  {
    class SoundFileInput : public IJobInput
    {
    public:
      SoundFileInput(){}
      SoundFileInput( const String& name, SoundCore* p )
        :FileName(name), pCore(p){}
      String FileName;
      SoundCore* pCore;
    };
    inline bool operator < ( const SoundFileInput& lha, const SoundFileInput& rha ) 
    {
      return lha.FileName < rha.FileName; 
    }

    class SoundFileOutput : public IJobOutput
    {
    public:
      SPSOUNDOBJECTINFO pInfo;
    };

    class SoundFileFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );
    };
  }

  class SoundFile
    : public GlobalPointer<SoundCore>
	{
	public:
		SoundFile();
		virtual ~SoundFile();

    void LoadFile( const String& filename );
    void Destroy();
    bool IsLoading()const;

    void Play();
    void Stop();

  private:
    typedef JobCacheTemplate<KEEPOUT::SoundFileInput,KEEPOUT::SoundFileFunction, KEEPOUT::SoundFileOutput> CACHE;
    CACHE m_Cache;

    SPSOUNDOBJECTINFO m_pInfo;
  };
}


#endif
