#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement 
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
public:
	virtual ~CManagement(void);

public:
	CComponent*		Get_Component(const TCHAR* pLayerTag,
								const TCHAR* pObjTag,
								const TCHAR* pComponentTag,
								CComponent::COMPONENTID eID);

public:
	HRESULT		SetUp_CurrentScene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		Render_Scene(LPDIRECT3DDEVICE9 _pGrapicDev);

private:
	CScene*		m_pScene = nullptr;

public:
	virtual	void Free(void);

};

END
#endif // Management_h__
