#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
{

}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT Engine::CManagement::SetUp_CurrentScene(CScene* pScene)
{
	Engine::Get_Renderer()->Clear_RenderGroup();
	Engine::Safe_Delete(m_pScene);


	m_pScene = pScene;

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9 _pGrapicDev)
{
	Engine::Get_Renderer()->Render_GameObject(_pGrapicDev);

	// debug ÄÚµå
	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	m_pScene->Free();
	Engine::Safe_Delete(m_pScene);
}

Engine::CComponent* Engine::CManagement::Get_Component(const TCHAR* pLayerTag, 
														const TCHAR* pObjTag, 
														const TCHAR* pComponentTag, 
														CComponent::COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

