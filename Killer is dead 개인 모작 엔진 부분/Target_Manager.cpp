#include "Target_Manager.h"
#include "Target.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

CTarget_Manager::~CTarget_Manager()
{
	Free();
}


HRESULT CTarget_Manager::Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr != pTarget)
		return E_FAIL;

	pTarget = CTarget::Create(pGraphic_Device, iWidth, iHeight, Format, ClearColor);

	if (nullptr == pTarget)
		return E_FAIL;

	m_mapTargets.insert(MAPTARGET::value_type(pTargetTag, pTarget));

	return NOERROR;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	list<CTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CTarget*>	MRTList;

		MRTList.push_back(pTarget);

		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
	}

	else
		pMRTList->push_back(pTarget);

	return NOERROR;
}

HRESULT CTarget_Manager::Begin_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
		{
			if (FAILED(pTarget->Clear_Target()))
				return E_FAIL;
		}
	}

	_uint		iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
		{
			if (FAILED(pTarget->SetUp_OnGraphicDev(iIndex++)))
				return E_FAIL;
		}
	}

	return NOERROR;
}


HRESULT CTarget_Manager::End_MRT(const _tchar * pMRTTag)
{
	list<CTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;




	_uint		iIndex = 0;

	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
		{
			if (FAILED(pTarget->Release_OnGraphicDev(iIndex++)))
				return E_FAIL;
		}
	}



	return NOERROR;
}

HRESULT CTarget_Manager::Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->Ready_DebugBuffer(fStartX, fStartY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pTarget : *pMRTList)
	{
		if (nullptr != pTarget)
		{
			if (FAILED(pTarget->Render_DebugBuffer()))
				return E_FAIL;
		}
	}


	return NOERROR;


}


HRESULT CTarget_Manager::SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar * pTargetTag, const char* pConstantName)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (nullptr == pTarget)
		return E_FAIL;

	return pTarget->SetUp_OnShader(pEffect, pConstantName);
}

CTarget* CTarget_Manager::Find_Target(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_mapTargets.begin(), m_mapTargets.end(), CTag_Finder(pTargetTag));

	if (iter == m_mapTargets.end())
		return nullptr;

	return iter->second;
}


list<CTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_mapTargets)
		Safe_Release(Pair.second);

	m_mapTargets.clear();
}

