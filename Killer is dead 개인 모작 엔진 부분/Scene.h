#ifndef Scene_h__
#define Scene_h__

#include "Engine_Defines.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene 
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CScene(void);

public:
	CComponent*		Get_Component(const TCHAR* pLayerTag, 
								const TCHAR* pObjTag, 
								const TCHAR* pComponentTag, 
								CComponent::COMPONENTID eID);


public:
	virtual HRESULT	Ready_Scene(void);
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(void)PURE;

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	map<const TCHAR*, CLayer*>	m_MapLayer;

public:
	virtual void Free(void);

};

END
#endif // Scene_h__
