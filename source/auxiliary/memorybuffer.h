﻿/*!
 	@file 
 	@brief	バッファをあらわすクラス
 */

#ifndef maid2_auxiliary_memorybuffer_h
#define maid2_auxiliary_memorybuffer_h

#include"../config/define.h"
#include"../config/typedef.h"
#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class MemoryBuffer
  {
  public:
    void Resize( size_t size );
    void* GetPointer( size_t size );
    const void* GetPointer( size_t size )const;

    size_t GetSize() const;

    void Add( const void* pData, size_t size );
    void Get( void* pDst, size_t pos, size_t size )const;

  private:
    void* _GetPointer( size_t size );

  private:
    std::vector<unt08> m_Image;
  };

  typedef boost::shared_ptr<MemoryBuffer> SPMEMORYBUFFER;
}



#endif
