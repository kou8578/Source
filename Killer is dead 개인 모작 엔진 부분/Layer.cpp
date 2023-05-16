#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{
	Free();
}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	isEnd = 0;

	for (auto& vec_iter : m_MapObject)
	{
		for (auto& iter : vec_iter.second)
		{
			isEnd = iter->Update_Object(fTimeDelta);

			if (isEnd & 0x8000000)
				return isEnd;
		}
	}

	return isEnd;
}

void Engine::CLayer::Render_Layer(void)
{
	for (auto& vec_iter : m_MapObject)
	{
		for (auto& iter : vec_iter.second)
		{
			iter->Render_Object();
		}
	}

}

Engine::CLayer* Engine::CLayer::Create(void)
{
	CLayer*	pInstance = new	CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Engine::Safe_Delete(pInstance);
	
	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto& vec_iter : m_MapObject)
	{
		for (auto& iter : vec_iter.second)
		{
			iter->Free();
			Engine::Safe_Delete(iter);
		}
	}
	m_MapObject.clear();
}

HRESULT Engine::CLayer::Add_GameObject(const TCHAR* pObjTag, 
										CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
	{
		MSG_BOX("Layer :: GameObject_Add :: 추가하려는 게임오브젝트가 Nullptr 입니다");
		return E_FAIL;
	}
	auto& vec_iter = m_MapObject.find(pObjTag);
	//Map
	if (vec_iter == m_MapObject.end())
	{
		m_MapObject[pObjTag] = vector<CGameObject*>();
	}
	m_MapObject[pObjTag].emplace_back( pGameObject);

	return S_OK;
}

Engine::CComponent* Engine::CLayer::Get_Component(const TCHAR* pObjTag, const TCHAR* pComponentTag, CComponent::COMPONENTID eID)
{
	auto	iter = find_if(m_MapObject.begin(), m_MapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_MapObject.end())
		return nullptr;
	
	return iter->second.front()->Get_Component(pComponentTag, eID);
}

vector<CGameObject*>* CLayer::Get_MapObject(const TCHAR * _StrMapTag)
{
	auto	iter = find_if(m_MapObject.begin(), m_MapObject.end(), CTag_Finder(_StrMapTag));

	if (iter == m_MapObject.end())
		return nullptr;

	return &iter->second;

}

