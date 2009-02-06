#ifndef framework_xiph_idecoder_h
#define framework_xiph_idecoder_h


#include"../../config/define.h"

#include"oggpacket.h"
#include"idecodedbuffer.h"

#include <boost/smart_ptr.hpp>

namespace Xiph {

  class IDecoder
  {
  public:
    virtual ~IDecoder(){}

    virtual void Initialize()=0;
    virtual void Finalize()=0;

    //  デコード
    virtual void Decode( const OggPacket& NewPacket, SPOGGDECODEDBUFFER& pOut )=0;

    //  初期設定が終わったか？
    virtual bool IsSetupped() const =0;

    //  現在の時間の取得
    virtual double GetTime()=0;

    //  シークモードの開始
    //  TargetTime を過ぎたら元の状態に戻る
    virtual void BeginSeekMode( double TargetTime )=0;


    //  溜まっているバッファを全部奪取する
    virtual void Flush( OGGDECODESAMPLELIST& List )=0;

    //  バッファリングしてる最中か？
    virtual bool IsBuffering() const=0;
  };

  typedef boost::shared_ptr<IOggDecoder> SPOGGDECODER;
}

#endif
