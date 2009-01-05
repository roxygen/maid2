#ifndef maid2_graphics_core_win32_id3d09object_h
#define maid2_graphics_core_win32_id3d09object_h


#include"../../../../config/define.h"

#include"common.h"
#include<set>

namespace Maid { namespace Graphics {

  class DeviceD3D09;
  class ID3D09Object
  {
    friend DeviceD3D09;
  public:
    ID3D09Object();
    virtual ~ID3D09Object();

  protected:
    virtual void  Escape()=0;
    virtual void  Restore( DeviceD3D09& Device )=0;

  private:
    static std::set<ID3D09Object*>  s_ObjectList;
  };

}}

#endif