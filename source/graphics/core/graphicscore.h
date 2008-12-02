/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_core_graphicscore_h
#define maid2_graphics_core_graphicscore_h

#include"../../config/define.h"



#include"idevice.h"


namespace Maid {
	class GraphicsCore
	{
	public:
		GraphicsCore();
		virtual ~GraphicsCore();

		void Initialize( const SPDEVICE& pDevice );
		void Finalize();

	private:
		SPDEVICE	m_pDevice;
	};
}


#endif
