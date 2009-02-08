#include"samplecache.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid { namespace Xiph {

void SampleCache::PushBack( const SAMPLE& sample )
{
  MAID_ASSERT( sample.pBuffer.get()==NULL, "データがありません" );

  ThreadMutexLocker lock(m_Mutex);

  m_List.push_back( sample );
}


void SampleCache::Clear()
{
  ThreadMutexLocker lock(m_Mutex);
  m_List.clear();
}


double SampleCache::GetBeginTime() const
{
  ThreadMutexLocker lock(m_Mutex);
  if( m_List.empty() ) { return 0; }

  return m_List.front().BeginTime;
}


SAMPLE SampleCache::GetLastSample() const
{
  ThreadMutexLocker lock(m_Mutex);
  return m_List.back();
}


double SampleCache::GetTotalTime() const
{
  ThreadMutexLocker lock(m_Mutex);
  double ret = 0;

  for( SAMPLELIST::const_iterator ite=m_List.begin(); ite!=m_List.end(); ++ite )
  {
    const double begin = ite->BeginTime;
    const double end = ite->EndTime;

    ret += end - begin;
  }

  return ret;
}

size_t SampleCache::GetSize() const
{
  ThreadMutexLocker lock(m_Mutex);
  return m_List.size();
}

void SampleCache::PopFront( SAMPLE& Out )
{
  ThreadMutexLocker lock(m_Mutex);
  Out = m_List.front();

  m_List.pop_front();
}

void SampleCache::PopAll( SAMPLELIST& Out )
{
  ThreadMutexLocker lock(m_Mutex);
  m_List.swap( Out );
}


//  指定した時間までのデータを全部取得する
void SampleCache::Pop( double TargetTime, SAMPLELIST& Out )
{
  ThreadMutexLocker lock(m_Mutex);
  if( m_List.empty() ) { return ; }

  if( TargetTime < m_List.front().BeginTime )
  {
    //  キャッシュがすべて未来のデータならスルー
    return ; 
  }

  if( m_List.back().EndTime < TargetTime )
  {
    //  キャッシュがすべて過去のデータなら、とりあえず全部送る
    Out.swap( m_List );
    return ; 
  }

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

}


bool SampleCache::IsEmpty() const
{
  ThreadMutexLocker lock(m_Mutex);

  return m_List.empty();

}


}}