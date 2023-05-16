#include "Collider.h"

USING(Engine)

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
}

Engine::CCollider::~CCollider(void)
{
	Free();
}

HRESULT Engine::CCollider::Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride)
{
	D3DXComputeBoundingBox(pPos, dwNumVtx, sizeof(_vec3), &m_vMin, &m_vMax);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
														0,							
														Engine::FVF_CUBE,			
														D3DPOOL_MANAGED,			
														&m_pVB,						
														NULL), E_FAIL);				


	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX32) * 12,
														0,
														D3DFMT_INDEX32,
														D3DPOOL_MANAGED,
														&m_pIB,
														NULL), E_FAIL);

	VTXCUBE*		pVtxCube = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTexUV = pVtxCube[0].vPos;

	pVtxCube[1].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTexUV = pVtxCube[1].vPos;

	pVtxCube[2].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTexUV = pVtxCube[2].vPos;

	pVtxCube[3].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTexUV = pVtxCube[3].vPos;


	pVtxCube[4].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTexUV = pVtxCube[4].vPos;

	pVtxCube[5].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTexUV = pVtxCube[5].vPos;

	pVtxCube[6].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTexUV = pVtxCube[6].vPos;

	pVtxCube[7].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);;
	pVtxCube[7].vTexUV = pVtxCube[7].vPos;


	INDEX32*	pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// -x
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;


	// +y
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// -y	 
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// +z
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// -z
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	_vec3 vU;
	_vec3 vV;
	for (_ushort i = 0; i < 9; i += 4)
	{
		vU = pVtxCube[pIndex[i]._1].vPos - pVtxCube[pIndex[i]._0].vPos;
		vV = pVtxCube[pIndex[i]._2].vPos - pVtxCube[pIndex[i]._0].vPos;
		D3DXVec3Cross(&m_tOBB.vNormalLine[i / 4], &vU, &vV);
		D3DXVec3Normalize(&m_tOBB.vNormalLine[i / 4], &m_tOBB.vNormalLine[i / 4]);

	}
	m_tOBB.vCenterPos = (m_vMax - m_vMin) * 0.5f;
	m_tOBB.fNormalLine_Len[LINE_X] = m_vMax.x - m_vMin.x;
	m_tOBB.fNormalLine_Len[LINE_Y] = m_vMax.y - m_vMin.y;
	m_tOBB.fNormalLine_Len[LINE_Z] = m_vMax.z - m_vMin.z;

	m_pVB->Unlock();
	m_pIB->Unlock();

	

	m_vMax_OBB.x = m_tOBB.vCenterPos.x + m_tOBB.fNormalLine_Len[LINE_X] * 0.5f;
	m_vMax_OBB.y = m_tOBB.vCenterPos.x + m_tOBB.fNormalLine_Len[LINE_Y] * 0.5f;
	m_vMax_OBB.z = m_tOBB.vCenterPos.x + m_tOBB.fNormalLine_Len[LINE_Z] * 0.5f;

	m_vMin_OBB.x = m_tOBB.vCenterPos.x - m_tOBB.fNormalLine_Len[LINE_X] * 0.5f;
	m_vMin_OBB.y = m_tOBB.vCenterPos.x - m_tOBB.fNormalLine_Len[LINE_Y] * 0.5f;
	m_vMin_OBB.z = m_tOBB.vCenterPos.x - m_tOBB.fNormalLine_Len[LINE_Z] * 0.5f;

//#ifdef _DEBUG

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);
		m_pTexture[i]->UnlockRect(0);
	}



//#endif


	return S_OK;
}

void Engine::CCollider::Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix)
{
	m_matWorld = *pColliderMatrix;

//#ifdef _DEBUG

	m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));

	m_pGraphicDev->SetFVF(FVF_CUBE);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);



//#endif
}


Engine::CCollider* Engine::CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pPos, dwNumVtx, dwStride)))
		Engine::Safe_Delete(pInstance);
	
	return pInstance;
}

void Engine::CCollider::Free(void)
{
	Safe_Release(m_pGraphicDev);
//#ifdef _DEBUG

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	

	for (_uint i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);
//#endif
}

