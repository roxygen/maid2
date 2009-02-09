#include"samplecache.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid { namespace Xiph {

SampleCache::SampleCache()
  :m_BeginTime(0)
  ,m_TotalTime(0)
  ,m_SampleCount(0)
{

}

SampleCache::~SampleCache()
{
  Clear();
}


void SampleCache::PushBack( const SAMPLE& sample )
{
  MAID_ASSERT( sample.pBuffer.get()==NULL, "データがありません" );

  ThreadMutexLocker lock(m_Mutex);
  m_List.push_back( sample );
  UpdateCache();
}


void SampleCache::Clear()
{
  ThreadMutexLocker lock(m_Mutex);
  m_List.clear();
  UpdateCache();
}

double SampleCache::GetBeginTime() const
{
  return m_BeginTime;
}

double SampleCache::GetTotalTime() const
{
  return m_TotalTime;
}

size_t SampleCache::GetSize() const
{
  return m_SampleCount;
}


//  指定した時間までのデータを全部取得する
void SampleCache::Pop( double TargetTime, SAMPLELIST& Out )
{
  if( TargetTime < m_BeginTime )
  {
    //  キャッシュがすべて未来のデータならスルー
    return ; 
  } 
  
  ThreadMutexLocker lock(m_Mutex);
  if( m_List.empty() ) { return ; }

  SAMPLELIST::iterator ite  = m_List.begin();
  while( true )
  {
    if( ite==m_List.end() ) { return; }

    const double begin = ite->BeginTime;
    const double end   = ite->EndTime;
    ++ite;

    if( begin==end && begin==TargetTime ) { break; }
    if( begin<= TargetTime && TargetTime < end ){ break; }
  }

  Out.splice( Out.end(), m_List, m_List.begin(), ite );

  UpdateCache();
}

void SampleCache::UpdateCache()
{
  ////////////////////////////////
  //  この関数を呼ぶときはm_Mutexの同期待ちをしていること！！
  ////////////////////////////////

  double begin = m_BeginTime;
  double total = 0;
  size_t count = 0;

  if( !m_List.empty() )
  {
    begin = m_List.front().BeginTime;

    for( SAMPLELIST::const_iterator ite=m_List.begin(); ite!=m_List.end(); ++ite )
    {
      const double begin = ite->BeginTime;
      const double end = ite->EndTime;

      total += end - begin;
    }

    count = m_List.size();
  }

  m_BeginTime = begin;
  m_TotalTime = total;
  m_SampleCount = count;
}


}}