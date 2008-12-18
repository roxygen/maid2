#ifndef maid2_storage_storagemessagebase_h
#define maid2_storage_storagemessagebase_h

#include"../config/define.h"
#include"../config/typedef.h"

#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Storage;
  class IStorageObject
  {
    friend Storage;
  public:
    IStorageObject() : MessageCount(0){}
    virtual ~IStorageObject(){}

    int GetMessageCount() const{ return MessageCount; }
  private:
    int MessageCount;
  };

	typedef	boost::shared_ptr<IStorageObject>	SPSTORAGEOBJECT;

  class IStorageFunction
  {
  public:
    IStorageFunction():m_IsSucess(true){}
    virtual ~IStorageFunction(){}
    virtual void Execute()=0;

    bool IsSucess()const { return m_IsSucess; }
    const String& GetErrorMessage()const { return m_ErrorMSG; }
  protected:
    void OnFaile() { m_IsSucess = true; }
    void OnFaile(const String& ErrorMSG )
    {
      OnFaile();
      m_ErrorMSG = ErrorMSG; 
    }
  private:
    bool  m_IsSucess;
    String  m_ErrorMSG;
  };


	typedef	boost::shared_ptr<IStorageFunction>	SPSTORAGEFUNCTION;

	//	描画スレッドに投げる　メッセージクラス
  namespace StorageMessage
  {
    struct Base
    {
      enum MESSAGE
      {
        NONE,				    //!<	何もしない

        FILE_W_CREATE,  //!<  ファイルの作成＆オープン
        FILE_W_OPEN,    //!<  ファイルのオープン
        FILE_W_ADD,     //!<  ファイルの追記オープン
        FILE_W_WRITE,   //!<  ファイルの書き込み
        FILE_W_SEEK,    //!<  ファイルポジションの移動
        FILE_W_CLOSE,   //!<  ファイルを閉じる

        FILE_R_OPEN,    //!<  ファイルのオープン
        FILE_R_READ,    //!<  ファイルの読み込み
        FILE_R_SEEK,    //!<  ファイルポジションの移動
        FILE_R_CLOSE,   //!<  ファイルを閉じる

        FUNCTION_EXECUTE//!<  独自関数の実行
      };

      Base( MESSAGE mess ) : Message(mess){}
      const MESSAGE	Message;
      SPSTORAGEOBJECT pObject;
    };
  }

	typedef	boost::shared_ptr<StorageMessage::Base>	SPSTORAGEMESSAGE;
	typedef	std::vector<SPSTORAGEMESSAGE>			      STORAGEMESSAGEQUE;
}



#endif
