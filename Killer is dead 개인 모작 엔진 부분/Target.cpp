#include "Target.h"

USING(Engine)

CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CTarget::Ready_Target(const _uint & iWidth, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor)
{
	// 텍스쳐를 생성한다.
	if (FAILED(D3DXCreateTexture(m_pGraphic_Device, iWidth, iHeight, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTarget_Texture)))
		return E_FAIL;

	// 텍스쳐로부터 렌더타겟에 대한 정보를 얻는다.
	if (FAILED(m_pTarget_Texture->GetSurfaceLevel(0, &m_pTarget_Surface)))
		return E_FAIL;

	m_Clear_Color = ClearColor;

	return NOERROR;
}

HRESULT CTarget::Clear_Target()
{
	if (nullptr == m_pTarget_Surface ||
		nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->GetRenderTarget(0, &m_pOld_Surface);

	m_pGraphic_Device->SetRenderTarget(0, m_pTarget_Surface);

	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET, m_Clear_Color, 1.f, 0);

	m_pGraphic_Device->SetRenderTarget(0, m_pOld_Surface);

	return NOERROR;
}

HRESULT CTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	if (nullptr == m_pTarget_Surface ||
		nullptr == m_pGraphic_Device)
		return E_FAIL;



	m_pGraphic_Device->GetRenderTarget(iIndex, &m_pOld_Surface);

	m_pGraphic_Device->SetRenderTarget(iIndex, m_pTarget_Surface);



	return NOERROR;
}

HRESULT CTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (nullptr == pEffect ||
		nullptr == m_pTarget_Texture)
		return E_FAIL;

	pEffect->SetTexture(pConstantName, m_pTarget_Texture);

	return NOERROR;
}

HRESULT CTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderTarget(iIndex, m_pOld_Surface);

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (0 == iIndex)
			D3DXSaveTextureToFile(L"../Bin/Diffuse.jpg", D3DXIFF_JPG, m_pTarget_Texture, nullptr);
		if (1 == iIndex)
			D3DXSaveTextureToFile(L"../Bin/Normal.jpg", D3DXIFF_JPG, m_pTarget_Texture, nullptr);

	}

	return NOERROR;
}

//#ifdef _DEBUG
HRESULT CTarget::Ready_DebugBuffer(const _float & fStartX, const _float & fStartY, const _float & fSizeX, const _float & fSizeY)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, FVF_SCREEN, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXSCREEN*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec4(fStartX, fStartY, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPos = _vec4(fStartX + fSizeX, fStartY, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPos = _vec4(fStartX + fSizeX, fStartY + fSizeY, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPos = _vec4(fStartX, fStartY + fSizeY, 0.f, 1.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return NOERROR;
}
HRESULT CTarget::Render_DebugBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetTexture(0, m_pTarget_Texture);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphic_Device->SetFVF(FVF_SCREEN);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	return NOERROR;
}
//#endif

CTarget * CTarget::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint & iWidht, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor)
{
	CTarget*		pInstance = new CTarget(pGraphic_Device);

	if (FAILED(pInstance->Ready_Target(iWidht, iHeight, Format, ClearColor)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTarget::Free()
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pTarget_Surface);
	Safe_Release(m_pTarget_Texture);
	Safe_Release(m_pGraphic_Device);
}
