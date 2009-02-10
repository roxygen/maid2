#include"profile.h"

#include"../../config/win32.h"
#include"../thread/win32/threadmutex.h"
#include"../thread/win32/threadcontroller.h"
#include<map>
#include<stack>
#include<stdio.h>

namespace Maid
{
  class Profile::Impl
  {
  public:
    Impl( const std::string& FileName, int Line );
    ~Impl();
    static void PrintLog( const std::string& FileName );


  private:

    LARGE_INTEGER m_BeginTime;
    std::string   m_MapIndex;


    struct DATA
    {
      DATA():ClockTotal(0),CallCount(0){}
      unt64 ClockTotal; //  クロックの合計
      unt   CallCount;  //  呼び出し回数
    };

    typedef std::map<std::string,DATA> PROFILEMAP;

    struct THREADDATA
    {
      THREADDATA():PrevCallTime(0){}
      PROFILEMAP  Profile;
      unt64       PrevCallTime; //  直前にかかった関数の時間
    };

    typedef std::map<unt,THREADDATA> THREADMAP;



    static THREADMAP   s_ThreadMap;
    static ThreadMutex s_Mutex;
  };

Profile::Impl::THREADMAP  Profile::Impl::s_ThreadMap;
ThreadMutex Profile::Impl::s_Mutex;

  Profile::Impl::Impl( const std::string& FileName, int Line )
  {
    char buf [1024];
    sprintf( buf, "line:%d %s", Line, FileName.c_str() );
    m_MapIndex = buf;


    {
      ThreadMutexLocker lock(s_Mutex);
      
      const unt id = ThreadController::GetCurrentID();

      s_ThreadMap[id].PrevCallTime = 0;
    }

    QueryPerformanceCounter( &m_BeginTime );
  }

  Profile::Impl::~Impl()
  {
    LARGE_INTEGER EndTime;
    QueryPerformanceCounter( &EndTime );

    //  この値は関数全部の時間なので
    //  直前にプロファイルされている時間をさっぴく
    const unt64 TotalTime = EndTime.QuadPart - m_BeginTime.QuadPart;

    {
      ThreadMutexLocker lock(s_Mutex);
      const unt id = ThreadController::GetCurrentID();
      THREADDATA& threaddata = s_ThreadMap[id];
      DATA& dat = threaddata.Profile[m_MapIndex];

      const unt64 PrevTime = threaddata.PrevCallTime;
      const unt64 time = TotalTime - PrevTime;

      dat.ClockTotal += time;
      dat.CallCount  += 1;

      threaddata.PrevCallTime = TotalTime;
    }
  }

  void Profile::Impl::PrintLog( const std::string& FileName )
  {
    unt64 TotalClock = 0;

    {
      for( THREADMAP::const_iterator ite=s_ThreadMap.begin(); ite!=s_ThreadMap.end(); ++ite )
      {
        for( PROFILEMAP::const_iterator ite2=ite->second.Profile.begin(); ite2!=ite->second.Profile.end(); ++ite2 )
        {
          TotalClock += ite2->second.ClockTotal;
        }
      }

    }


    FILE* hFile = fopen( FileName.c_str(), "w" );

    for( THREADMAP::const_iterator ite=s_ThreadMap.begin(); ite!=s_ThreadMap.end(); ++ite )
    {
      //  スレッド単位で処理時間のおおい順にソートする
      typedef std::multimap<unt64,PROFILEMAP::const_iterator> SORTOBJECT;
      SORTOBJECT Sort;
      for( PROFILEMAP::const_iterator ite2=ite->second.Profile.begin(); ite2!=ite->second.Profile.end(); ++ite2 )
      {
        Sort.insert( std::make_pair(ite2->second.ClockTotal,ite2 ) );
      }

      fprintf( hFile, "thread %d------------------------\n", ite->first );

      for( SORTOBJECT::reverse_iterator ite2=Sort.rbegin(); ite2!=Sort.rend(); ++ite2 )
      {
        const std::string& name = ite2->second->first;
        const unt64 total = ite2->second->second.ClockTotal;
        const unt count = ite2->second->second.CallCount;

        const double norm = double(total*100*10000/TotalClock) / 10000.0;
        const double average = norm / double(count);

        fprintf( hFile, "use:%f%% call:%d avg:%f%% %s\n", norm, count, average, name.c_str() );
      }
    }

    fclose(hFile);
  }


  Profile::Profile( const std::string& FileName, int Line )
    :m_pImpl( new Impl(FileName,Line) )
  {

  }

  void Profile::PrintLog( const std::string& FileName )
  {
    Impl::PrintLog( FileName );
  }


}