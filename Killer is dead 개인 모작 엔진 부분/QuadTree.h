#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CFrustum;
class ENGINE_DLL CQuadTree 
{
public:
	enum CHILD	{ CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	enum EDGE	{ EDGE_UP, EDGE_DOWN, EDGE_LEFT, EDGE_RIGHT , EDGE_END};

private:
	explicit CQuadTree(void);
public:
	virtual ~CQuadTree(void);

public:
	HRESULT	Ready_QuadTree(const _ulong& dwCntX,
		const _ulong& dwCntZ);

	void	CullingForTerrain(CFrustum* pFrustum,
		const _vec3* pVtxPos,
		INDEX32* pIndex,
		_ulong* pTriCnt);

	_bool	LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVtxPos);

private:
	void	Make_ChildCorner(CHILD eType,
		const _ulong& dwPLT,
		const _ulong& dwPRT,
		const _ulong& dwPLB,
		const _ulong& dwPRB,
		const _ulong& dwPCenter,
		_ulong* pLT,
		_ulong* pRT,
		_ulong* pLB,
		_ulong* pRB);

	void	Make_ChildQuadTree(void);

	void	Ready_Child(const _ulong& dwLT,
		const _ulong& dwRT,
		const _ulong& dwLB,
		const _ulong& dwRB);
	_int		Get_Node_Index(EDGE _eType, _ulong _cntX, _ulong& _LT, _ulong& _RT, _ulong& _LB, _ulong& _RB , CFrustum* pFrustum , const _vec3* pVtxPos);
	CQuadTree*	Find_Node(const _vec3* pVtxPos, CFrustum* pFrustum ,_ulong _LT, _ulong _RT, _ulong _LB, _ulong _RB );

private:
	CQuadTree*				m_pChild[CHILD_END];
	CQuadTree*				m_pNeighbor[EDGE_END];
	_ulong					m_dwCorner[CORNER_END];
	_ulong					m_dwCenter;
	_float					m_fRadius;

public:
	static CQuadTree*	Create(const _ulong& dwCntX,
		const _ulong& dwCntZ);

	CQuadTree*			Create(const _ulong& dwLT,
		const _ulong& dwRT,
		const _ulong& dwLB,
		const _ulong& dwRB);

	virtual	void	Free(void);

};
END
#endif // QuadTree_h__
