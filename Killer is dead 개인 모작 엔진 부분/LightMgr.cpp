#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{
	m_vec_Light.clear();
}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}

const D3DLIGHT9 * CLightMgr::Get_Light_Info(const _uint & iIndex)
{
	if (!m_vec_Light.empty())
	{
		if (m_vec_Light[iIndex] != nullptr)
			return m_vec_Light[iIndex]->Get_Light_Info();
		else
			return nullptr;
	}
	else
		return nullptr;
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_vec_Light.begin(), m_vec_Light.end(), CDeleteObj());
	m_vec_Light.clear();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);

	if (nullptr == pLight)
		return E_FAIL;

	m_vec_Light.push_back(pLight);

	return S_OK;
}

HRESULT CLightMgr::Render_Light(LPD3DXEFFECT pEffect,const _uint& iIndex)
{
	for (auto& pLight : m_vec_Light)
	{
		if (nullptr != pLight)
		{
			//인덱스가 성립하거나 방향성 광원일경우 출력
			if (pLight->Get_index() == iIndex  || pLight->Get_Light_Info()->Type == D3DLIGHT_DIRECTIONAL)
				if (FAILED(pLight->Render_Light(pEffect)))
					return E_FAIL;
		}
	}

	return NOERROR;
}

