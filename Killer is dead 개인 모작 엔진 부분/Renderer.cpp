#include "Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "LightMgr.h"
#include "Shader.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
	: m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLightMgr::GetInstance())
	, m_Light_Index(0)
{
	m_pLight_Manager->AddRef();

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	if (nullptr == m_pGraphic_Device)
	{
		m_pGraphic_Device = pGraphic_Device;
		m_pGraphic_Device->AddRef();
	}

	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3DVIEWPORT9			ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphic_Device->GetViewport(&ViewPort);

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, FVF_SCREEN, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXSCREEN*			pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec4(0.0f, 0.0f, 0.f, 1.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPos = _vec4(ViewPort.Width, 0.0f, 0.f, 1.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPos = _vec4(ViewPort.Width, ViewPort.Height, 0.f, 1.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPos = _vec4(0.0f, ViewPort.Height, 0.f, 1.f);
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


	// For.Target_Diffuse
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Diffuse", ViewPort.Width, ViewPort.Height, D3DFMT_A8R8G8B8, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Diffuse", 0.f, 0.f, 100.f, 100.f)))
		return E_FAIL;

	// For.Target_Normal
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Normal", 0.f, 100.f, 100.f, 100.f)))
		return E_FAIL;
	// For.Target_Depth
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.2f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Depth", 0.f, 200.f, 100.f, 100.f)))
		return E_FAIL;
	// For.Target_Shade
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Shade", 100.f, 0.0f, 100.f, 100.f)))
		return E_FAIL;
	// For.Target_Specular
	if (FAILED(m_pTarget_Manager->Add_Target(pGraphic_Device, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(L"Target_Specular", 100.f, 100.0f, 100.f, 100.f)))
		return E_FAIL;
	// For.MRT_Deferred
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Deferred", L"Target_Depth")))
		return E_FAIL;
	// For.MRT_LightAcc
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	m_pShader_LightAcc = CShader::Create(m_pGraphic_Device, L"../../Reference/Headers/Shader_LightAcc.fx");
	if (nullptr == m_pShader_LightAcc)
		return E_FAIL;

	m_pShader_Blend = CShader::Create(m_pGraphic_Device, L"../../Reference/Headers/Shader_Blend.fx");
	if (nullptr == m_pShader_Blend)
		return E_FAIL;

	return NOERROR;
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup,
	CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);

}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{

	Render_Priority(pGraphicDev);

	Render_Deferred();
	Render_LightAcc();
	Render_Blend();

	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	if (nullptr == m_pTarget_Manager)
		return;

	m_pTarget_Manager->Render_DebugBuffer(L"MRT_Deferred");
	m_pTarget_Manager->Render_DebugBuffer(L"MRT_LightAcc");

	Clear_RenderGroup();

}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		//for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		if(!m_RenderGroup[i].empty())
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->AddRef();

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	Safe_Release(pGraphicDev);
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->AddRef();

	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_Object();

	Safe_Release(pGraphicDev);
}

_bool Compare_Z(CGameObject* pDest, CGameObject* pSour)
{
	return pDest->Get_ViewZ() > pSour->Get_ViewZ();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->AddRef();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0.001f);

	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	Safe_Release(pGraphicDev);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->AddRef();
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	Safe_Release(pGraphicDev);
}

void CRenderer::Render_Deferred()
{
	if (nullptr == m_pTarget_Manager)
		return;

	// 타겟을 교체한다.
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_Deferred")))
		return;

	Render_NonAlpha(m_pGraphic_Device);

	// 타겟을 복구한다.
	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_Deferred")))
		return;
}

void CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pLight_Manager ||
		nullptr == m_pShader_LightAcc)
		return;

	// 타겟을 교체한다.
	if (FAILED(m_pTarget_Manager->Begin_MRT(L"MRT_LightAcc")))
		return;


	LPD3DXEFFECT pEffect = m_pShader_LightAcc->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	// 셰이더 전역변수에 노멀 텍스쳐를 전달하자.
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture")))
		return;
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture")))
		return;
	m_pLight_Manager->Render_Light(pEffect,m_Light_Index);

	// 타겟을 복구한다.
	if (FAILED(m_pTarget_Manager->End_MRT(L"MRT_LightAcc")))
		return;



	Safe_Release(pEffect);
}

void CRenderer::Render_Blend()
{
	LPD3DXEFFECT pEffect = m_pShader_Blend->Get_EffectHandle();
	if (nullptr == pEffect)
		return;

	pEffect->AddRef();

	// 셰이더 전역변수에 노멀 텍스쳐를 전달하자.
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Diffuse", "g_DiffuseTexture")))
		return;

	// 셰이더 전역변수에 노멀 텍스쳐를 전달하자.
	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Shade", "g_ShadeTexture")))
		return;

	if (FAILED(m_pTarget_Manager->SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture")))
		return;
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphic_Device->SetFVF(FVF_SCREEN);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();

	Safe_Delete(m_pShader_Blend);
	Safe_Delete(m_pShader_LightAcc);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pGraphic_Device);
}

