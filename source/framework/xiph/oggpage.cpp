#include"oggpage.h"

#include"../../auxiliary/macro.h"

#pragma COMPILERMSG("各種関数が const ogg_page* じゃないせいでコンパイルできないので、強引にキャストしてます。もうね（ｒｙ")	

namespace Maid { namespace Xiph {

OggPage::OggPage()
{
  ZERO( &m_Page, sizeof(m_Page) );
}

bool OggPage::IsBeginOfStream() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_bos(p)!=0;
}

bool OggPage::IsEndOfStream() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_eos(p)!=0;
}

int OggPage::GetSerialNo() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_serialno(p);
}

int OggPage::GetPageNo() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_pageno(p);
}

ogg_int64_t OggPage::GetGranulePosition() const
{
  ogg_page* p = (ogg_page*)&m_Page;

  return (size_t)ogg_page_granulepos(p);
}


void OggPage::Copy( std::vector<unsigned char>& header, std::vector<unsigned char>& body, const OggPage& SrcPage )
{
  const ogg_page& src = SrcPage.m_Page;

  header.resize( src.header_len );
  ::memcpy( &(header[0]), src.header, src.header_len );

  body.resize( src.body_len );
  ::memcpy( &(body[0]), src.body, src.body_len );
  
  m_Page.header = &(header[0]);
  m_Page.header_len = src.header_len;
  m_Page.body = &(body[0]);
  m_Page.body_len = src.body_len;
}


}}