#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider :public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CCollider(void);

public:
	HRESULT		Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride);
	void		Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix);
public:
	_vec3		Get_vMin() { return m_vMin; }
	_vec3		Get_vMax() { return m_vMax; }
	VOID		Set_vMin(_vec3 _vMin) { m_vMin = _vMin; }
	VOID		Set_vMax(_vec3 _vMax) { m_vMax = _vMax; }


	_vec3		Get_vMin_OBB() { return m_vMin_OBB; }
	_vec3		Get_vMax_OBB() { return m_vMax_OBB; }
	VOID		Set_vMin_OBB(_vec3 _vMin_OBB) { m_vMin_OBB = _vMin_OBB; }
	VOID		Set_vMax_OBB(_vec3 _vMax_OBB) { m_vMax_OBB = _vMax_OBB; }

	OBB*		Get_OBB() { return &m_tOBB; }
	LPDIRECT3DVERTEXBUFFER9	Get_VB() { return m_pVB; }
	LPDIRECT3DINDEXBUFFER9	Get_IB() { return m_pIB; }
private:
	_vec3		m_vMin, m_vMax;
	OBB			m_tOBB;
	D3DXMATRIX	m_matWorld;

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
//#ifdef _DEBUG
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
//#endif
	_vec3 m_vMin_OBB;
	_vec3 m_vMax_OBB;
	


public:
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _vec3* pPos, 
									const _ulong& dwNumVtx, 
									const _ulong& dwStride);

	virtual void	Free(void);

};

END
#endif // Collider_h__
