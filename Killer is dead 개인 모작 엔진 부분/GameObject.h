#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject 
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CGameObject(void);

public:
	CComponent*		Find_Component(const TCHAR* pComponentTag, CComponent::COMPONENTID eID);
	CComponent*		Get_Component(const TCHAR* pComponentTag, CComponent::COMPONENTID eID);
	_float			Get_ViewZ(void) const { return m_fViewZ; }
	void			Compute_ViewZ(const _vec3* pPos);

public:
	virtual HRESULT	Ready_Object(void);
	virtual _int	Update_Object(const _float& fTimeDelta);
	virtual void	Render_Object(void);

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	map<const TCHAR*, CComponent*>		m_mapComponent[CComponent::ID_END];
	_float		m_fViewZ = 0.f;

public:
	virtual void	Free(void);

};
END
#endif // GameObject_h__
