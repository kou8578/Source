#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(void);
public:
	~CTransform(void);

public:
	const _matrix*		Get_NRotWorldMatrix(_matrix* pWorldMatrix)
	{
		if (nullptr != pWorldMatrix)
			*pWorldMatrix = m_matNRotWorld;

		return &m_matNRotWorld;
	}
	void				Get_Information(INFO eType, _vec3* pInfo) { memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3)); }
	_vec3*				Get_Information(INFO eType) { return &m_vInfo[eType]; }
	_float				Get_Rotation(ROTATION _eType) { return *(((float*)&m_vAngle) + _eType); }
	_vec3*				Get_Rotation() { return &m_vAngle; }
	_vec3*				Get_Scale()					{ return &m_vScale; }
	_matrix				Get_MatWorld()				{ return m_matWorld; }
	const _matrix*		Get_WorldMatrixPointer(void) const { return &m_matWorld; }


	void		Set_ParentMatrix(const _matrix* pParentMatrix) 
	{ 
		m_matWorld *= *pParentMatrix;
		/*m_matWorld._11 = m_vScale.x;
		m_matWorld._22 = m_vScale.y;
		m_matWorld._33 = m_vScale.z;*/
	}
	void		Set_vecScale(_vec3 _vecScale) { m_vScale = _vecScale; }
	void		Set_Pos(_vec3 _Pos)
	{
		m_vInfo[INFO_POS].x = _Pos.x;
		m_vInfo[INFO_POS].y = _Pos.y;
		m_vInfo[INFO_POS].z = _Pos.z;
	}
	void		Set_Rotation(ROTATION _eType, const _float& fAngle)	{ *(((float*)&m_vAngle) + _eType) = fAngle; }
	void		Move_Position(const _vec3* const pDirection) { m_vInfo[INFO_POS] += *pDirection; }
	void		Rotation(ROTATION eType, const _float& fAngle)	{		*(((float*)&m_vAngle) + eType) += fAngle;	}
	void		World_Identity() { D3DXMatrixIsIdentity(&m_matWorld); }

	void			Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);
	void			LookAT_Target(const _vec3 _vTargerPos, const _float& _fspeed);
	void			LookAT_Target_Quick(const _vec3 _vTargerPos);
	const _matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);

public:
	HRESULT		Ready_Transform(void);
	virtual		void Update_Component(const _float& fTimeDelta);


public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;
	_matrix			m_matNRotWorld;

public:
	static CTransform*	Create(void);
private:
	virtual void Free(void);

};

END
#endif // Transform_h__
