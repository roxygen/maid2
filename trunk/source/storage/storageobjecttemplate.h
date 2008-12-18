#ifndef maid2_storage_storageobjecttemplate_h
#define maid2_storage_storageobjecttemplate_h



#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/string.h"
#include"../auxiliary/globalpointer.h"

#include"storage.h"
#include"storagemessagebase.h"
#include<vector>

namespace Maid
{
	//	オブジェクトを共有するための補助クラス
/*

  StorageObjectTemplate<共有アルゴリズム,共有データ,それっぽく動くクラス>
*/

	template<class ID,class OPTION,class OBJECT>
	class StorageObjectTemplate : public GlobalPointer<Storage>, public OBJECT
	{
	private:
		struct STORAGEINFO
		{
      OPTION  Option;
      bool    IsCreated;  //  Optionが設定されているか？
      int		  ReferenceCount;
			SPSTORAGEOBJECT pObject;
			SPSTORAGEFUNCTION pFunc;
		};

		typedef std::map<ID,STORAGEINFO> STORAGEMAP;
		static STORAGEMAP	s_Storage;

		enum STATE
		{
			STATE_EMPTY,	//	まだ何もしてない
			STATE_LOADING,	//	読み込み中
			STATE_WORKING,	//	稼動中
		};

		STATE   m_State;
		ID	    m_ID;
//    OBJECT  m_Object;

	private:
//		void DeleteFirst( const ID& id  ) { return m_Object.DeleteFirst(id); }
//		void LoadEndFirst( const IStorageFunction* param, OPTION& option  ) { return m_Object.LoadEndFirst(param,option); }
//		void LoadEnd( const OPTION& option  ) { return m_Object.LoadEnd(option); }
//    IStorageFunction* CreateFunction( const ID& id ) { return m_Object.CreateFunction(id); }
//    ID ConvertID( const ID& id ) { return m_Object.ConvertID(id); }

	public:

    void Load( const ID& id )
		{
			Delete();

			m_ID = ConvertID(id);

      STORAGEMAP::iterator ite = s_Storage.find(m_ID);

			if( ite==s_Storage.end() )
			{	//	もし初めての読み込みだったら、ファイトです！
        SPSTORAGEFUNCTION pFunc( CreateFunction(m_ID) );
				SPSTORAGEOBJECT pObj = GlobalPointer<Storage>::Get()->ExecuteFunction( pFunc );

				STORAGEINFO& info = s_Storage[m_ID];

				info.ReferenceCount = 1;
				info.pObject = pObj;
				info.pFunc = pFunc;
				info.IsCreated = false;
			}
			else
			{
				//	２回目以降はキャッシュのみ
				ite->second.ReferenceCount += 1;
			}

      m_State = STATE_LOADING;

			//	とりあえず調べておく
			LoadingCheck();
		}

		void Delete()
		{
			if( m_State==STATE_EMPTY ) { return ; }
			
			STORAGEMAP::iterator ite = s_Storage.find(m_ID);
			MAID_ASSERT( ite==s_Storage.end(), "これはありえない" );
			MAID_ASSERT( ite->second.ReferenceCount==0, "これもありえない" );

			STORAGEINFO& info = ite->second;
			info.ReferenceCount -= 1;

			if( info.ReferenceCount==0 )
			{
        DeleteFirst( m_ID );
				s_Storage.erase( ite );
			}

			m_ID = ID();
			m_State = STATE_EMPTY;
		}

		bool IsLoading()
		{
      if( m_State==STATE_EMPTY ) { return false; }  //  読み込みしてないならfalseでしょう
      LoadingCheck();
      return m_State!=STATE_WORKING;
		}

		StorageObjectTemplate()
			:m_State(STATE_EMPTY)
		{
		}

		StorageObjectTemplate( const StorageObjectTemplate<ID,OPTION,OBJECT>& rha )
//		StorageObjectTemplate( const StorageObjectTemplate<ID,OPTION>& rha )
			:m_State(STATE_EMPTY)
		{
			if( rha.m_State == STATE_EMPTY ) { return ; }
      Load( rha.m_ID );
		}

		StorageObjectTemplate<ID,OPTION,OBJECT>& operator = ( const StorageObjectTemplate<ID,OPTION,OBJECT>& rha )
//		StorageObjectTemplate<ID,OPTION>& operator = ( const StorageObjectTemplate<ID,OPTION>& rha )
    {
      Delete();
			if( rha.m_State!=STATE_EMPTY ) { Load( rha.m_ID ); }
      return *this;
    }

		virtual ~StorageObjectTemplate()
		{
      Delete();
		}

  private:

    void LoadingCheck()
    {
			MAID_ASSERT( m_State==STATE_EMPTY, "読み込みをしていない" );

			if( m_State==STATE_WORKING ) { return; }

			STORAGEMAP::iterator ite = s_Storage.find(m_ID);
			MAID_ASSERT( ite==s_Storage.end(), "これはありえない" );
			MAID_ASSERT( ite->second.ReferenceCount==0, "これもありえない" );

			STORAGEINFO& info = ite->second;

			//	すでに作成されているなら、それだけ
			if( info.IsCreated )
			{
				m_State = STATE_WORKING;
				LoadEnd( info.Option );
				return; 
			}

			//	まだなら調べる
      if( info.pObject->GetMessageCount()!=0 ) { return ;}

      info.IsCreated = true;	//	読み込み終了！
	    m_State = STATE_WORKING;

	    LoadEndFirst( info.pFunc.get(), info.Option );
      info.pFunc.reset();
      info.pObject.reset();
     }
	};
}


#endif