#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

Engine::CCamera::~CCamera(void)
{
	Free();
}

HRESULT Engine::CCamera::Ready_Object(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	

	return S_OK;
}

_int Engine::CCamera::Update_Object()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return S_OK;
}

void Engine::CCamera::Free(void)
{
	CGameObject::Free();
}

