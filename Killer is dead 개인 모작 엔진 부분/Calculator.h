#ifndef Calculator_h__
#define Calculator_h__
#include "TerrainTex.h"
#include "TerrainCol.h"
#include "Collider.h"
#include "Transform.h"
#include "SphereMesh.h"
BEGIN(Engine)

class CTerrainTex;
class CTerrainCol;
class CTransform;
class CCollider;
class CSphereMesh;
class ENGINE_DLL CCalculator
{
	DECLARE_SINGLETON(CCalculator)
private:
	explicit CCalculator();
	virtual ~CCalculator(void);
private:
	typedef	struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3]; // 객체 당 세 개의 면을 향해 뻗어나가는 벡터
		_vec3		vAxis[3];     // 객체 평면과 평행한 축 벡터

	}OBB;




public:
	_bool Collision_Sphere(CSphereMesh* _Dsc_Sphere, CSphereMesh* _Src_Sphere);

	_float		Compute_HeightOnTerrain(const _vec3* pPos,
		const _vec3* pTerrainVtxPos,
		const _ulong& dwCntX,
		const _ulong& dwCntZ);

	_vec3		Picking_OnTerrain(HWND hWnd,
									LPDIRECT3DDEVICE9 _pGrapicDev,
									const CTerrainTex* pTerrainVtx,
									CTransform* pTerrainTransCom);
	_vec3		Picking_OnTerrain(HWND hWnd,
									LPDIRECT3DDEVICE9 _pGrapicDev,
									const CTerrainCol* pTerrainVtx,
									CTransform* pTerrainTransCom);
	_vec3		Picking_Test(HWND hWnd, 
							LPDIRECT3DDEVICE9 _pGrapicDev,
							const CTerrainCol* pTerrainVtx, 
							CTransform* pTerrainTransCom, 
							const	_vec3*	vPos, 
							const	_vec3*	vAt);
	_float		Collision_Picking_MeshObj(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, CCollider* _pCollider /*, const _vec3* _pPos ,const POINT _pt*/);
	_bool		Collision_Picking_Sphere(HWND hWnd, LPDIRECT3DDEVICE9 _pGrapicDev, CTransform * pSphereTransCom , _float _fRadius);
	_bool		Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
		const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
		const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

private:
	void	Set_Point(OBB*pObb, const _vec3* pMin, const _vec3* pMax);
	void	Set_Axis(OBB*pObb);

private:
	virtual void Free(void);
};

END
#endif // Calculator_h__
