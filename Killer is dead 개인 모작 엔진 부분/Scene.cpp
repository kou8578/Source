#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
}

Engine::CScene::~CScene(void)
{
	Free();
}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int	isEnd = 0;

	for (auto& iter : m_MapLayer)
	{
		isEnd = iter.second->Update_Layer(fTimeDelta);

		if (isEnd & 0x8000000)
			return -1;
	}
	
	return 0;
}

void Engine::CScene::Render_Scene(void)
{
// 	for (auto& iter : m_MapLayer)
// 		iter.second->Render_Layer();
}

void Engine::CScene::Free(void)
{
	for (auto& iter : m_MapLayer)
	{
		Engine::Safe_Delete(iter.second);
	}
	m_MapLayer.clear();
	Safe_Release(m_pGraphicDev);
}

Engine::CComponent* Engine::CScene::Get_Component(const TCHAR* pLayerTag, 
													const TCHAR* pObjTag, 
													const TCHAR* pComponentTag, 
													CComponent::COMPONENTID eID)
{
	auto	iter = find_if(m_MapLayer.begin(), m_MapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_MapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

