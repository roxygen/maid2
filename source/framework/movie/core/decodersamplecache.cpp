﻿#include"decodersamplecache.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"


namespace Maid { namespace Movie { 

DecoderSampleCache::DecoderSampleCache()
{

}

DecoderSampleCache::~DecoderSampleCache()
{
  Clear();
}


void DecoderSampleCache::PushBack( const DECODERSAMPLE& sample )
{
  MAID_ASSERT( sample.pSample.get()==NULL, "データがありません" );

  m_List.push_back( sample );
}


void DecoderSampleCache::Clear()
{
  m_List.clear();
}

double DecoderSampleCache::GetBeginTime() const
{
  if( GetSize()==0 ) { return -1; }

  return m_List.front().BeginTime;
}


double DecoderSampleCache::GetTotalTime() const
{
  if( GetSize()==0 ) { return 0; }

  double total = 0;
  for( DECODERSAMPLELIST::const_iterator ite=m_List.begin(); ite!=m_List.end(); ++ite )
  {
    const double begin = ite->BeginTime;
    const double end = ite->EndTime;

    total += end - begin;
  }

  return total;
}

size_t DecoderSampleCache::GetSize() const
{
  return m_List.size();
}


//  指定した時間までのデータを全部取得する
int DecoderSampleCache::Pop( double TargetTime, DECODERSAMPLELIST& Out )
{
  if( m_List.empty() ) { return 0; }

  DECODERSAMPLELIST::iterator ite  = m_List.begin();
  while( true )
  {
    if( ite==m_List.end() ) { break; }

    const double begin = ite->BeginTime;
    const double end   = ite->EndTime;

    if( TargetTime < begin ){ break; }
    ++ite;
  }

  Out.splice( Out.end(), m_List, m_List.begin(), ite );

  return m_List.size();
}


}}