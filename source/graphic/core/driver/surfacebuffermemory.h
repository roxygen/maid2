#ifndef maid2_graphic_core_surfacebuffermemory_h
#define maid2_graphic_core_surfacebuffermemory_h

#include"../../../config/define.h"
#include"../../../config/typedef.h"

#include<vector>

#include"surfacebuffer.h"



namespace Maid
{
	/*!
	 	@class	SurfaceBufferMemory surfacebuffermemory.h
	 	@brief	サーフェスバッファ オンメモリ Ver.
	 */
	class SurfaceBufferMemory : public SurfaceBuffer
	{
	public:
		SurfaceBufferMemory();
		~SurfaceBufferMemory();

		virtual SIZE2DI GetSize() const;
		virtual PIXELFORMAT GetPixelFormat() const;

		virtual void Lock( SurfaceBufferLockInfo& info );
		virtual void Unlock();

		void Create( const SIZE2DI& size, PIXELFORMAT fmt );
		void Destroy();

		void SetReverseSurface( bool IsReverse );

		void* GetPlanePTR();

	private:
		std::vector<unt08>	m_Plane;
		std::vector<unt08>	m_CLUT;

		SIZE2DI		m_Size;
		PIXELFORMAT m_PixelFormat;
		bool m_IsReverse;
	};

	typedef boost::shared_ptr<SurfaceBufferMemory> SPSURFACEBUFFERMEMORY;

}


#endif