#ifndef maid2_auxiliary_thread_threadcontroller_h
#define maid2_auxiliary_thread_threadcontroller_h

/*!
  @file
  @brief	スレッド制御クラス
 */

#include"../../../config/define.h"
#include"../../string.h"
#include<process.h>
#include<boost/function.hpp>
#include<boost/bind.hpp>

namespace Maid
{
	class ThreadController
	{
	public:

		/*!
		 	@class	BRIGEDATA ThreadController.h
		 	@brief	スレッドを作成したクラスとスレッドを呼ばれたクラスが
		\n			受け渡すデータ
		 */
		struct BRIGEDATA
		{
			volatile bool	IsExit;     //!<	強制終了命令がでているか？
			volatile bool	IsExecute;  //!<	スレッドが起動中か？
			volatile bool	IsAssert;   //!<	スレッドが強制終了しちゃったか？
		};

	public:
		ThreadController();
		~ThreadController();

		//!	スレッドの優先度
		enum PRIORITY
		{
			PRIORITY_HIGH2,		//!<	一番高い
			PRIORITY_HIGH1,
			PRIORITY_NORMAL,	//!<	デフォルト
			PRIORITY_LOW1,
			PRIORITY_LOW2,		//!<	一番低い
		};

		typedef boost::function1<unt, volatile BRIGEDATA&>	THREADFUNCTION;
		void	SetFunc( const THREADFUNCTION& Function );
		void	Execute( PRIORITY priority = PRIORITY_NORMAL );
		void	Close();

		void SetPriority( PRIORITY priority );

		void Pause();
		void Resume();

		bool IsThreadExited() const;
		bool IsThreadAssert() const;
		unt GetReturnCode() const;

		static void Sleep( unt time );

	private:
		static  unsigned __stdcall Thread( void* pVoid );

		THREADFUNCTION	m_Func;
		uintptr_t	    	m_hThread;

		volatile BRIGEDATA		m_BrigeData;
		String			        	m_ExceptionText;
	};

	#define MakeThreadObject(pFunc,pObject) boost::bind( pFunc, pObject, _1 )

}

#endif


