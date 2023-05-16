#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	virtual HRESULT	Ready_Object(void);
	virtual _int	Update_Object();

protected:
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFovY, m_fAspect, m_fNear, m_fFar;
	_matrix		m_matView, m_matProj;

protected:
	virtual void	Free(void);
};

END
#endif // Camera_h__


