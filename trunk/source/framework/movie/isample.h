#ifndef maid2_framework_isample_h
#define maid2_framework_isample_h


#include"../../config/define.h"
#include"../../auxiliary/buffer.h"
#include"../../auxiliary/mathematics.h"

#include <boost/smart_ptr.hpp>
#include <vector>
#include <list>



namespace Maid {

  class ISample
  {
  public:
    virtual ~ISample(){}
  };

  typedef boost::shared_ptr<ISample> SPSAMPLE;

  class SampleImage
    : public ISample
  {
  public:
    SIZE2DI       SizeY;        //  YUV の Y 平面の大きさ
    int           PitchY;       //  Y平面のピッチ
    std::vector<unt08>  BitsY;  //  Y平面のデータ

    SIZE2DI       SizeUV;       //  YUV の UV 平面の大きさ
    int           PitchUV;      //  UV平面のピッチ
    std::vector<unt08>  BitsU;  //  U平面のデータ
    std::vector<unt08>  BitsV;  //  V平面のデータ
  };

  typedef boost::shared_ptr<SampleImage> SPSAMPLEIMAGE;


  class SamplePCM
    : public ISample
  {
  public:
    typedef std::vector<float32>  BUFFER;
    std::vector<BUFFER> Data; //  １チャンネルごとに並んでる
  };

  typedef boost::shared_ptr<SamplePCM> SPSAMPLEPCM;



  struct STREAMSAMPLE
  {
    double BeginTime; //  開始時間（単位は秒）
    double EndTime;   //  終了時間（単位は秒）
    SPSAMPLE pSample;

    //  BeginTime <= x < EndTime の範囲になる
    STREAMSAMPLE()
      :BeginTime(0)
      ,EndTime(0)
    {
    }
  };
  typedef std::list<STREAMSAMPLE> STREAMSAMPLELIST;

}

#endif
