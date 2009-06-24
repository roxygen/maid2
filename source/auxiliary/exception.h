﻿/*!
 	@file 
 	@brief	ライブラリで使用している例外処理のベースクラス
 */

#ifndef maid2_auxiliary_exception_h
#define maid2_auxiliary_exception_h

#include"../config/define.h"
#include"streambuffer.h"
#include<ostream>

namespace Maid
{
	//	ライブラリ内部で使われる例外のベースクラス
	//	なんでもいいから投げておけな時にも使う
  class Exception : public std::exception
  {
  public:
    class ExceptionString : public std::ostream
    {
    public:
      ExceptionString( const char* szFile, int Line );

      std::string GetLog() const;

    private:
      const std::string	m_FileName;
      const int	m_Line;
	    StreamBuffer	m_StreamBuf;
    };

    Exception();
    Exception( const ExceptionString& e );
    void AddLog( const std::string& str );
    const std::string& GetLog()const;
    void WriteLog();

  private:
    std::string m_Log;
  };
}

#define MAID_THROWEXCEPTION( mess )   \
  {                                   \
    Maid::Exception::ExceptionString e(__FILE__,__LINE__);     \
    e<<mess;                          \
    throw Maid::Exception(e);         \
  }                                   \



#endif
