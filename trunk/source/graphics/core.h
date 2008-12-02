/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_core_h
#define maid2_graphics_core_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"



#include"core/idevice.h"
#include"core/idrawcommandplayer.h"


namespace Maid { namespace Graphics {

  class Core : public GlobalPointer<Core>
	{
	public:
		Core();
		virtual ~Core();

		void Initialize( const SPDEVICE& pDevice );
		void Finalize();

	private:
    SPDEVICE	          m_pDevice;
    SPDRAWCOMMANDPLAYER m_pCommand;
	};
}}


#endif
