#ifndef maid2_graphics_core_idrawcommandplayer_h
#define maid2_graphics_core_idrawcommandplayer_h

#include"../../config/define.h"

#include"idrawcommand.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	IDrawCommandPlayer idrawcommandplayer.h
      @brief	描画コンテキスト 即時実行タイプ
  */

  class IDrawCommandPlayer : public IDrawCommand
  {
  public:

  };
  typedef	boost::shared_ptr<IDrawCommandPlayer>	SPDRAWCOMMANDPLAYER;

}}

#endif