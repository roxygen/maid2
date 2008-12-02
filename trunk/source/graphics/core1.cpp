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

  m_pCommand.reset( m_pDevice->CreateDrawCommandPlayer() );
}

void Core::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

	m_pCommand.reset();
	m_pDevice->Finalize();
	m_pDevice.reset();
}

}}

