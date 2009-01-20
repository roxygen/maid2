#ifndef maid2_sound_isoundobject_h
#define maid2_sound_isoundobject_h

#include"../config/define.h"
#include<boost/smart_ptr.hpp>
#include"soundmessagebase.h"

namespace Maid {

  /*!
      @class  ISoundObject isoundobject.h
      @brief  SoundCore が管理してるベースクラス
  */
  class ISoundObject
	{
	public:

    enum TYPE
    {
      TYPE_NULL,
      TYPE_STATIC,
      TYPE_STREAM,
    };
    ISoundObject( TYPE t ):m_Type(t){}

    virtual ~ISoundObject(){}

    //! 定期的な更新
    /*!
        SoundCore が呼び出すので、バッファを更新するなどしてください
     */
    virtual void Update()=0;

    //! 再生の開始
    /*!
        終端までいったら再生がとまります
        SetLoopState(true) の時は Stop() が呼ばれない限り再生してます
     */
    virtual void Play()=0;

    //! 再生中のバッファをとめる
    /*!
        停止中に呼んでも問題なし
     */
    virtual void Stop()=0;

    //! 再生位置を変更する
    /*!
        @param  time  [i ]  位置（1.0f= １秒）
     */
    virtual void SetPosition( float time )=0;

    //! ボリュームの設定
    /*!
        @param  volume  [i ]  新しいボリューム(0.0f:無音 1.0f:等倍)
     */
    virtual void SetVolume( float volume )=0;

    //! ジャンプポイントの設定
    /*!
        ジャンプポイントっていうのは、Ａの場所まできたら
        Ｂの場所にジャンプする機能

        @param  list  [i ]  設定リスト
     */
    virtual void SetJumpPoint( const std::vector<JUMPPOINT>& list )=0;

    //! ループ再生の設定
    /*!
        @param  IsLoop  [i ]  true ならループ再生する
     */
    virtual void SetLoopState( bool IsLoop )=0;

    //! 再生中か？
    /*!
        @return  再生中なら true
     */
    virtual bool IsPlay()const=0;

    //! 現在の位置
    /*!
        @return  位置（1.0f=1秒)
     */
    virtual float GetPosition() const=0;

    //! 現在のボリューム
    /*!
        @return  ボリューム
     */
    virtual float GetVolume() const=0;

    TYPE GetType() const { return m_Type; }
  private:
    TYPE  m_Type;
  };

  typedef	boost::shared_ptr<ISoundObject>	SPSOUNDOBJECT;

}


#endif
