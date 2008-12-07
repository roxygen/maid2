#include"core.h"



namespace Maid { namespace Graphics {

	/*!
	 	@class Core core.h
	 	@brief  グラフィック周りを管理するクラス
	 */

Core::Core()
{
}

Core::~Core()
{
	Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
	@param	pDriver	[i ]	使うドライバ
 */
void Core::Initialize( const SPDEVICE& pDevice )
{
	m_pDevice = pDevice;
	m_pDevice->Initialize();
}

void Core::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

	m_pDevice->Finalize();
	m_pDevice.reset();
}

const SPDEVICE& Core::GetDevice()
{
  return m_pDevice;
}


}}

