/*!
  @file 
  @brief	特定のクラス間だけでアクセスできるようにするテンプレート
 */

#ifndef maid2_auxiliary_globalpointer_h
#define maid2_auxiliary_globalpointer_h


#include"../config/define.h"
#include"debug/assert.h"

namespace Maid
{
	template<class TYPE>
	class GlobalPointer
	{
	protected:

		void Set()
		{
			MAID_ASSERT( s_pPointer!=NULL, "すでに実態が存在します" );
			s_pPointer = static_cast<TYPE*>(this);
		}

		TYPE* Get()
		{
			MAID_ASSERT( s_pPointer==NULL, "まだ設定されていません" );
			return s_pPointer;
		}

		void Clear() { s_pPointer = NULL; }

	private:
		static TYPE*	s_pPointer;
	};

}


#endif
