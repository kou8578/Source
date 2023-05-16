#include "Calculator.h"


USING(Engine)
IMPLEMENT_SINGLETON(CCalculator)

Engine::CCalculator::CCalculator()
{
}

Engine::CCalculator::~CCalculator(void)
{
}

_bool CCalculator::Collision_Sphere(CSphereMesh * _Dsc_Sphere, CSphereMesh * _Src_Sphere)
{
	_float fDist;
	_vec3 vDiff, vDscPos,vSrcPos;
	_matrix matDsc = _Dsc_Sphere->Get_TransCom()->Get_MatWorld();
	_matrix matSrc = _Src_Sphere->Get_TransCom()->Get_MatWorld();
	vDscPos = _vec3(matDsc._41, matDsc._42, matDsc._43);
	vSrcPos = _vec3(matSrc._41, matSrc._42, matSrc._43);
	vDiff = vSrcPos - vDscPos;
	fDist = D3DXVec3Length(&vDiff);

	if (fDist <= (_Dsc_Sphere->Get_Radius() + _Src_Sphere->Get_Radius()))
		return TRUE;//충돌
	return FALSE;//비 충돌
}

_float Engine::CCalculator::Compute_HeightOnTerrain(const _vec3* pPos,
	const _vec3* pTerrainVtxPos,
	const _ulong& dwCntX,
	const _ulong& dwCntZ)
{
	_ulong		dwIndex = _ulong(pPos->z / 1.f) * dwCntX + _ulong(pPos->x / 1.f);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / 1.f;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / 1.f;

	_float	fHeight[4] = { pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y };

	// 오른쪽 위

	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}
	// 왼쪽 아래
	else
	{
		return fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}



}
void Engine::CCalculator::Free(void)
{
}

_vec3 Engine::CCalculator::Picking_OnTerrain(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, const CTerrainTex* pTerrainVtx, CTransform* pTerrainTransCom)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	_vec3		vMousePos;
	vMousePos.x = ptMouse.x / (980.f * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(600 * 0.5f) + 1.f;
	vMousePos.z = 0.f;
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	_pGrapicDev->GetViewport(&ViewPort);


	_matrix			matProj, matView;

	_pGrapicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	_pGrapicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_matrix		matWorld;

	matWorld = pTerrainTransCom->Get_MatWorld();
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong	dwVtxCntX = pTerrainVtx->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainVtx->Get_VtxCntZ();

	const	_vec3*		pTerrainVtxPos = pTerrainVtx->Get_VtxPos();

	_ulong	dwVtxIdx[3];
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtxPos[dwVtxIdx[1]].z + (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[1]].z) * fV);
			}

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtxPos[dwVtxIdx[2]].x + (pTerrainVtxPos[dwVtxIdx[2]].x - pTerrainVtxPos[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtxPos[dwVtxIdx[2]].z + (pTerrainVtxPos[dwVtxIdx[0]].z - pTerrainVtxPos[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, const CTerrainCol * pTerrainVtx, CTransform * pTerrainTransCom)
{

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	_pGrapicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / ((ViewPort.Width-300) * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix			matProj, matView;

	_pGrapicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	_pGrapicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_matrix		matWorld;

	//pTerrainTransCom->Get_WorldMatrix(&matWorld);
	matWorld = pTerrainTransCom->Get_MatWorld();
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong	dwVtxCntX = pTerrainVtx->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainVtx->Get_VtxCntZ();

	const	_vec3*		pTerrainVtxPos = pTerrainVtx->Get_VtxPos();

	_ulong	dwVtxIdx[3];
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return vRayPos + vRayDir * fDist;
			}

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return vRayPos + vRayDir * fDist;
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_vec3 CCalculator::Picking_Test(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, const CTerrainCol * pTerrainVtx, CTransform * pTerrainTransCom, const _vec3 * vPos, const _vec3 * vAt)
{

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	_vec3		vMousePos;
	vMousePos.x = ptMouse.x / (980.f * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(600 * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix			matProj, matView;

	_pGrapicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;


	_pGrapicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matView);

	vRayPos = *vPos;
	vRayDir = vMousePos - vRayPos;

	D3DXVec3Normalize(&vRayDir, &vRayDir);
	//_matrix		matWorld;

	/*matWorld = pTerrainTransCom->Get_MatWorld();*/

	//_pGrapicDev->GetTransform(D3DTS_WORLD, &matWorld);
	//D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	//D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	//D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_ulong	dwVtxCntX = pTerrainVtx->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainVtx->Get_VtxCntZ();

	const	_vec3*		pTerrainVtxPos = pTerrainVtx->Get_VtxPos();

	_ulong	dwVtxIdx[3];
	_float	fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
				&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtxPos[dwVtxIdx[1]].z + (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[1]].z) * fV);
			}

			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[2]],
				&pTerrainVtxPos[dwVtxIdx[0]],
				&pTerrainVtxPos[dwVtxIdx[1]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(pTerrainVtxPos[dwVtxIdx[2]].x + (pTerrainVtxPos[dwVtxIdx[2]].x - pTerrainVtxPos[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtxPos[dwVtxIdx[2]].z + (pTerrainVtxPos[dwVtxIdx[0]].z - pTerrainVtxPos[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_float CCalculator::Collision_Picking_MeshObj(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, CCollider * _pCollider /*, const _vec3* _pPos ,const POINT _pt*/)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	_vec3		vMousePos;

	vMousePos.x = ptMouse.x / (980.f * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(600 * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix			matProj, matView, matWorld;

	_pGrapicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f,0.f,0.f);
	vRayDir = vMousePos - vRayPos;

	_pGrapicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_pGrapicDev->GetTransform(D3DTS_WORLD, &matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);



	_float	fU, fV, fDist;
	VTXCUBE*		pVtxCube = nullptr;
	INDEX32*		pIndex = nullptr;
	_pCollider->Get_VB()->Lock(0, 0, (void**)&pVtxCube, 0);
	_pCollider->Get_IB()->Lock(0, 0, (void**)&pIndex, 0);
	for (_ulong i = 0; i < 12; ++i)
	{
		if (D3DXIntersectTri(&pVtxCube[pIndex[i]._0].vPos,
			&pVtxCube[pIndex[i]._1].vPos,
			&pVtxCube[pIndex[i]._2].vPos,
			&vRayPos, &vRayDir,
			&fU, &fV, &fDist))
		{
			return fDist;
		}
	}
	_pCollider->Get_VB()->Unlock();
	_pCollider->Get_IB()->Unlock();
		return -1.f;
}

_bool CCalculator::Collision_Picking_Sphere(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, CTransform * pSphereTransCom , _float _fRadius)
{

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	_pGrapicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / ((ViewPort.Width - 300) * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix			matProj, matView, matWorld;



	_pGrapicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3		vRayDir, vRayPos;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;
	D3DXVec3Normalize(&vRayDir, &vRayDir);
	_pGrapicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);

	_vec3	vSphere_Center = *pSphereTransCom->Get_Information(INFO::INFO_POS);

	D3DXMatrixIdentity(&matWorld);
	matWorld._41 = -vSphere_Center.x;
	matWorld._42 = -vSphere_Center.y;
	matWorld._43 = -vSphere_Center.z;
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);

	_float VV = D3DXVec3Dot(&vRayDir, &vRayDir);
	_float QV = D3DXVec3Dot(&vRayPos, &vRayDir);
	_float QQ = D3DXVec3Dot(&vRayPos, &vRayPos);
	_float RR = _fRadius * _fRadius;
	if (QV * QV - VV * (QQ - RR) >= 0)
		return TRUE;
	return FALSE;
}

_bool Engine::CCalculator::Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
	const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld)
{
	_vec3		vDestMin, vDestMax, vSourMin, vSourMax;
	_float	    fMin, fMax;

	D3DXVec3TransformCoord(&vDestMin, pDestMin, pDestWorld);
	D3DXVec3TransformCoord(&vDestMax, pDestMax, pDestWorld);

	D3DXVec3TransformCoord(&vSourMin, pSourMin, pSourWorld);
	D3DXVec3TransformCoord(&vSourMax, pSourMax, pSourWorld);

	// x축에서 바라봤을 때
	fMin = max(vDestMin.x, vSourMin.x);
	fMax = min(vDestMax.x, vSourMax.x);

	if (fMax < fMin)
		return false;

	// y축에서 바라봤을 때
	fMin = max(vDestMin.y, vSourMin.y);
	fMax = min(vDestMax.y, vSourMax.y);

	if (fMax < fMin)
		return false;

	// z축에서 바라봤을 때
	fMin = max(vDestMin.z, vSourMin.z);
	fMax = min(vDestMax.z, vSourMax.z);

	if (fMax < fMin)
		return false;


	return true;
}

_bool Engine::CCalculator::Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld, const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld)
{
	OBB			tObb[2];
	ZeroMemory(tObb, sizeof(OBB) * 2);

	Set_Point(&tObb[0], pDestMin, pDestMax);
	Set_Point(&tObb[1], pSourMin, pSourMax);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], pSourWorld);
	}

	D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, pSourWorld);

	for (_uint i = 0; i < 2; ++i)
		Set_Axis(&tObb[i]);

	_float	fDistance[3]{}; // 1. 첫 번째 obb에서 벡터를 임의의 축으로 투영한 길이의 합
							// 2. 두 번째 obb에서 벡터를 임의의 축으로 투영한 길이의 합
							// 3. 각 obb의 중점을 이은 벡터를 임의의 축으로 투영한 길이의 합


	for (_int i = 0; i < 2; ++i)
	{
		for (_int j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j])) +
				fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j])) +
				fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j])) +
				fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j])) +
				fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));

			if (fDistance[2] > fDistance[0] + fDistance[1])
				return false;

		}
	}

	return true;

}

void CCalculator::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
{
	pObb->vPoint[0] = _vec3(pMin->x, pMax->y, pMin->z);
	pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
	pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
	pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);

	pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
	pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
	pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
	pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

	pObb->vCenter = (*pMin + *pMax) * 0.5f;
}

void CCalculator::Set_Axis(OBB * pObb)
{
	pObb->vProjAxis[0] = (pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[1] = (pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[2] = (pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;

	pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3];
	pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3];
	pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3];

	for (_uint i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);

}

