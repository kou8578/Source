#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
}

Engine::CGameObject::~CGameObject(void)
{
}

CComponent * CGameObject::Find_Component(const TCHAR * pComponentTag, CComponent::COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if(iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

CComponent * CGameObject::Get_Component(const TCHAR * pComponentTag, CComponent::COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

void CGameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));
}

HRESULT Engine::CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	for (auto& iter : m_mapComponent[CComponent::ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void Engine::CGameObject::Render_Object(void)
{
	
}

void Engine::CGameObject::Free(void)
{
	for (_ulong i = 0; i < CComponent::ID_END; ++i)
	{
		for (auto& iter : m_mapComponent[i])
		{
			iter.second->Free();
			Engine::Safe_Delete(iter.second);
		}
		m_mapComponent[i].clear();		
	}
	Safe_Release(m_pGraphicDev);
	
}

