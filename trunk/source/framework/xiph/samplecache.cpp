#include"samplecache.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"


namespace Maid { namespace Xiph {

SampleCache::SampleCache()
  :m_TotalTime(0)
  ,m_SampleCount(0)
  ,m_BeginTime(0)
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

  const double begin = sample.BeginTime;
  const double end   = sample.EndTime;

  m_TotalTime   += end - begin;
  m_SampleCount += 1;

}


void SampleCache::Clear()
{
  ThreadMutexLocker lock(m_Mutex);
  m_List.clear();
  
  m_TotalTime   = 0;
  m_SampleCount = 0;
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
  if( GetSize()==0 ) { return ; }

  ThreadMutexLocker lock(m_Mutex);

  SAMPLELIST::iterator ite  = m_List.begin();
  while( true )
  {
    if( ite==m_List.end() ) { break; }

    const double begin = ite->BeginTime;
    const double end   = ite->EndTime;

    if( TargetTime < begin ){ break; }
    ++ite;
  }

  Out.splice( Out.end(), m_List, m_List.begin(), ite );

  UpdateCache();
}

void SampleCache::UpdateCache()
{
  ////////////////////////////////
  //  この関数を呼ぶときはm_Mutexの同期待ちをしていること！！
  ////////////////////////////////

  double total = 0;
  size_t count = 0;

  if( !m_List.empty() )
  {
    m_BeginTime = m_List.front().BeginTime;
    for( SAMPLELIST::const_iterator ite=m_List.begin(); ite!=m_List.end(); ++ite )
    {
      const double begin = ite->BeginTime;
      const double end = ite->EndTime;

      total += end - begin;
    }

    count = m_List.size();
  }

  m_TotalTime = total;
  m_SampleCount = count;
}


}}