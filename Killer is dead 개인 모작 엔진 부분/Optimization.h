#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "Frustum.h"
#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
public:
	enum USEAGE { DISABLE, ENABLE };

private:
	explicit COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COptimization(const COptimization& rhs);
public:
	virtual ~COptimization(void);

public:
	HRESULT		Ready_Optimization(USEAGE eType,
									const _ulong& dwCntX,
									const _ulong& dwCntZ);

	_bool		Is_InFrustum_ForObject(const _vec3* pPosWorld,
										const _float fRadius);

	void		Is_InFrustum_ForTerrain(const _vec3* pVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										Engine::INDEX32* pIndex,
										_ulong* pTriCnt);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bClone;
	CFrustum*				m_pFrustum;
	CQuadTree*				m_pQuadTree;

public:
	static COptimization*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										USEAGE eType,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);
	CComponent*					Clone(void);
	virtual void				Free(void);

};

END
#endif // Optimization_h__
