#include "QuadTree.h"
#include "Frustum.h"

USING(Engine)

Engine::CQuadTree::CQuadTree(void)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_dwCorner, sizeof(_ulong) * CORNER_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * EDGE_END);
}

Engine::CQuadTree::~CQuadTree(void)
{
	Free();
}

HRESULT Engine::CQuadTree::Ready_QuadTree(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_dwCorner[CORNER_LB] = 0;
	m_dwCorner[CORNER_RB] = dwCntX - 1;
	m_dwCorner[CORNER_LT] = dwCntX * dwCntZ - dwCntX;
	m_dwCorner[CORNER_RT] = dwCntX * dwCntZ - 1;

	m_dwCenter = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB] + m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 2;

	Make_ChildQuadTree();

	return S_OK;
}

void Engine::CQuadTree::CullingForTerrain(CFrustum* pFrustum,
	const _vec3* pVtxPos,
	INDEX32* pIndex,
	_ulong* pTriCnt)
{
	m_fRadius = D3DXVec3Length(&(pVtxPos[m_dwCorner[CORNER_LT]] - pVtxPos[m_dwCenter]));

	_bool bIn = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCenter], m_fRadius);

	if (true == bIn)
	{
		_ulong LT[EDGE_END], RT[EDGE_END], LB[EDGE_END], RB[EDGE_END];
		_int Center[EDGE_END];
		for (_ulong i = 0; i < EDGE_END; ++i)
		{
			LT[i] = m_dwCorner[CORNER_LT];
			RT[i] = m_dwCorner[CORNER_RT];
			LB[i] = m_dwCorner[CORNER_LB];
			RB[i] = m_dwCorner[CORNER_RB];
		}
		
		//여기에서 이웃노드 생성 ,출력할것이기 때문에
		Center[EDGE_UP]		=	Get_Node_Index(EDGE_UP,		128, LT[EDGE_UP]	, RT[EDGE_UP]	, LB[EDGE_UP]	, RB[EDGE_UP]	, pFrustum, pVtxPos);
		Center[EDGE_DOWN]	=	Get_Node_Index(EDGE_DOWN,	128, LT[EDGE_DOWN]	, RT[EDGE_DOWN]	, LB[EDGE_DOWN]	, RB[EDGE_DOWN]	, pFrustum, pVtxPos);
		Center[EDGE_LEFT]	=	Get_Node_Index(EDGE_LEFT,	128, LT[EDGE_LEFT]	, RT[EDGE_LEFT]	, LB[EDGE_LEFT]	, RB[EDGE_LEFT]	, pFrustum, pVtxPos);
		Center[EDGE_RIGHT]	=	Get_Node_Index(EDGE_RIGHT,	128, LT[EDGE_RIGHT]	, RT[EDGE_RIGHT], LB[EDGE_RIGHT], RB[EDGE_RIGHT], pFrustum, pVtxPos);
		if (Center[EDGE_UP] == -1 && Center[EDGE_DOWN] == -1 && Center[EDGE_LEFT] == -1 && Center[EDGE_RIGHT] == -1)
			goto Jump;
			for (_ulong i = 0; i < EDGE_END; ++i)
			{
				if (Center[i] >= 0)
				{
					m_pNeighbor[i] = Find_Node(pVtxPos, pFrustum, LT[i], RT[i], LB[i], RB[i]);
				}
			}
			_bool bIn_Neightbor[EDGE_END] = { FALSE };
			for (_ulong i = 0; i < EDGE_END; ++i)
			{
				if (m_pNeighbor[i] != nullptr)
				{
					bIn_Neightbor[i] = pFrustum->IsIn_Frustum(&pVtxPos[m_pNeighbor[i]->m_dwCenter], m_pNeighbor[i]->m_fRadius);
				}
			}

			//이웃노드들이 모두 다 출력이 가능하면 현재 노드와 이웃노드들이 같은 LOD이므로 분할할 필요 없다.
			if (bIn_Neightbor[EDGE_UP] && bIn_Neightbor[EDGE_DOWN] && bIn_Neightbor[EDGE_LEFT] && bIn_Neightbor[EDGE_RIGHT])
			{
				//출력이 가능할때 (오른쪽위)
				pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
				++(*pTriCnt);
				//출력이 가능할때 (왼쪽 아래)
				pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
				++(*pTriCnt);
			}
			_ulong Neightbor_Center = 0;
			if (!bIn_Neightbor[EDGE_UP])
			{
				Neightbor_Center = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) / 2;
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LT];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = Neightbor_Center;
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RT];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = Neightbor_Center;
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RT];
				++(*pTriCnt);
			}

			if (!bIn_Neightbor[EDGE_DOWN])
			{
				Neightbor_Center = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB]) / 2;
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = Neightbor_Center;
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
			}

			if (!bIn_Neightbor[EDGE_LEFT])
			{
				Neightbor_Center = (m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_LB]) / 2;
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = Neightbor_Center;
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LT];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
			}

			if (!bIn_Neightbor[EDGE_RIGHT])
			{
				Neightbor_Center = (m_dwCorner[CORNER_RT] + m_dwCorner[CORNER_RB]) / 2;
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = Neightbor_Center;
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
				++(*pTriCnt);
			}
			else
			{
				pIndex[*pTriCnt]._0 = m_dwCenter;
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
				pIndex[*pTriCnt]._2 = Neightbor_Center;
				++(*pTriCnt);
			}
		


		Jump:
		//if (nullptr == m_pChild[0])
		if (true == LevelOfDetail(pFrustum->GetDevice(), pVtxPos))
		{
			_bool		bVtxInFrustum[3] = { false };

			bVtxInFrustum[0] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_LT]]);
			bVtxInFrustum[1] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_RT]]);
			bVtxInFrustum[2] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_RB]]);

			if (true == bVtxInFrustum[0] ||
				true == bVtxInFrustum[1] ||
				true == bVtxInFrustum[2])
			{
				//출력이 가능할때 (오른쪽위)
				pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RT];
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_RB];
				++(*pTriCnt);
			}

			bVtxInFrustum[0] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_LT]]);
			bVtxInFrustum[1] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_RB]]);
			bVtxInFrustum[2] = pFrustum->IsIn_Frustum(&pVtxPos[m_dwCorner[CORNER_LB]]);

			if (true == bVtxInFrustum[0] ||
				true == bVtxInFrustum[1] ||
				true == bVtxInFrustum[2])
			{
				//출력이 가능할때 (왼쪽 아래)
				pIndex[*pTriCnt]._0 = m_dwCorner[CORNER_LT];
				pIndex[*pTriCnt]._1 = m_dwCorner[CORNER_RB];
				pIndex[*pTriCnt]._2 = m_dwCorner[CORNER_LB];
				++(*pTriCnt);
			}

			return;
		}

		for (_ulong i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChild[i])
				m_pChild[i]->CullingForTerrain(pFrustum, pVtxPos, pIndex, pTriCnt);
		}
	}
}

void Engine::CQuadTree::Make_ChildCorner(CHILD eType,
	const _ulong& dwPLT,
	const _ulong& dwPRT,
	const _ulong& dwPLB,
	const _ulong& dwPRB,
	const _ulong& dwPCenter,
	_ulong* pLT,
	_ulong* pRT,
	_ulong* pLB,
	_ulong* pRB)
{
	_ulong	dwLT, dwRT, dwLB, dwRB;
	_ulong  dwLC, dwTC, dwRC, dwBC;

	dwLC = (dwPLT + dwPLB) >> 1;
	dwTC = (dwPLT + dwPRT) >> 1;
	dwRC = (dwPRT + dwPRB) >> 1;
	dwBC = (dwPLB + dwPRB) >> 1;

	switch (eType)
	{
	case CHILD_LT:
		dwLT = dwPLT;
		dwLB = dwLC;
		dwRT = dwTC;
		dwRB = dwPCenter;
		break;

	case CHILD_RT:
		dwLT = dwTC;
		dwLB = dwPCenter;
		dwRT = dwPRT;
		dwRB = dwRC;
		break;

	case CHILD_LB:
		dwLT = dwLC;
		dwLB = dwPLB;
		dwRT = dwPCenter;
		dwRB = dwBC;
		break;

	case CHILD_RB:
		dwLT = dwPCenter;
		dwLB = dwBC;
		dwRT = dwRC;
		dwRB = dwPRB;
		break;
	}

	*pLT = dwLT;
	*pRT = dwRT;
	*pLB = dwLB;
	*pRB = dwRB;
}

void Engine::CQuadTree::Make_ChildQuadTree(void)
{
	if (1 == m_dwCorner[CORNER_RT] - m_dwCorner[CORNER_LT])
		return;

	for (_uint i = 0; i < CHILD_END; ++i)
	{
		_ulong	dwLT, dwRT, dwLB, dwRB;

		Make_ChildCorner(CHILD(i),
			m_dwCorner[CORNER_LT],
			m_dwCorner[CORNER_RT],
			m_dwCorner[CORNER_LB],
			m_dwCorner[CORNER_RB],
			m_dwCenter,
			&dwLT, &dwRT, &dwLB, &dwRB);

		m_pChild[i] = CQuadTree::Create(dwLT, dwRT, dwLB, dwRB);

		if (nullptr != m_pChild[i])
			m_pChild[i]->Make_ChildQuadTree();
	}
}

void Engine::CQuadTree::Ready_Child(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	m_dwCorner[CORNER_LB] = dwLB;
	m_dwCorner[CORNER_RB] = dwRB;
	m_dwCorner[CORNER_LT] = dwLT;
	m_dwCorner[CORNER_RT] = dwRT;

	m_dwCenter = (m_dwCorner[CORNER_LB] + m_dwCorner[CORNER_RB] + m_dwCorner[CORNER_LT] + m_dwCorner[CORNER_RT]) >> 2;

}

_int CQuadTree::Get_Node_Index(EDGE _eType, _ulong _cntX, 
								_ulong & _LT, _ulong & _RT, 
								_ulong & _LB, _ulong & _RB ,
								CFrustum* pFrustum , const _vec3* pVtxPos)
{
	_ulong Interval;
	_ulong LT, RT, LB, RB;
	_ulong Center;
	LT = _LT;
	RT = _RT;
	LB = _LB;
	RB = _RB;
	
	Interval = _RB - _LB;
	switch (_eType)
	{
	case Engine::CQuadTree::EDGE_UP:
		_LT = LT - _cntX * Interval;
		_RT = RT - _cntX * Interval;
		_LB = LT;
		_RB = _RT;
		break;
	case Engine::CQuadTree::EDGE_DOWN:
		_LT = LB;
		_RT = RB;
		_LB = LB + _cntX * Interval;
		_RB = RB + _cntX * Interval;
		break;
	case Engine::CQuadTree::EDGE_LEFT:
		_LT = LT - Interval;
		_RT = LT;
		_LB = LB - Interval;
		_RB = LB;
		break;
	case Engine::CQuadTree::EDGE_RIGHT:
		_LT = RT ;
		_RT = RT + Interval;
		_LB = RB ;
		_RB = RB + Interval;
		break;
	}
	Center = (LT + RT + LB + RB) / 4;
	if (pFrustum->IsIn_Frustum(&pVtxPos[Center]))
		return Center;
	else
		return -1;
}

CQuadTree * CQuadTree::Find_Node(const _vec3 * pVtxPos, CFrustum* pFrustum, _ulong _LT, _ulong _RT, _ulong _LB, _ulong _RB)
{
	CQuadTree* p = nullptr;

	if ((m_dwCorner[0] == _LT) && (m_dwCorner[1] == _RT) && m_dwCorner[2] == _LB && m_dwCorner[3] == _RB)
	{
		//자기자신이 일치하는 노드라면 자기자신을 리턴
		return this;
	}
	for (_ulong i = 0; i < CHILD_END; ++i)
	{
		if (m_pChild[i] != nullptr)
		{
			if (pFrustum->IsIn_Frustum(&pVtxPos[m_pChild[i]->m_dwCenter]))
			{
				return m_pChild[i]->Find_Node(pVtxPos, pFrustum, _LT, _RT, _LB, _RB);
			}

		}
	}
	return nullptr;
}

Engine::CQuadTree* Engine::CQuadTree::Create(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	CQuadTree* pInstance = new CQuadTree;

	if (FAILED(pInstance->Ready_QuadTree(dwCntX, dwCntZ)))
		Safe_Delete(pInstance);

	return pInstance;
}

Engine::CQuadTree* Engine::CQuadTree::Create(const _ulong& dwLT, const _ulong& dwRT, const _ulong& dwLB, const _ulong& dwRB)
{
	CQuadTree*	pInstance = new CQuadTree;

	pInstance->Ready_Child(dwLT, dwRT, dwLB, dwRB);

	return pInstance;
}

void Engine::CQuadTree::Free(void)
{
	for (_ulong i = 0; i < CHILD_END; ++i)
	{
		if (nullptr != m_pChild[i])
			m_pChild[i]->Free();
		Safe_Delete(m_pChild[i]);
	}
}

_bool Engine::CQuadTree::LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVtxPos)
{
	_matrix	matView;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3 vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	_float	fDistance = D3DXVec3Length(&(vCamPos - pVtxPos[m_dwCenter]));

	_float	fWidth = D3DXVec3Length(&(pVtxPos[m_dwCorner[CORNER_RT]] - pVtxPos[m_dwCorner[CORNER_LT]]));
	return fDistance * 0.1f > fWidth;


}

