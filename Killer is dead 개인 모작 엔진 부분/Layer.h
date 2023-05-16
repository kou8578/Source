#ifndef Layer_h__
#define Layer_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer 
{

private:
	explicit CLayer(void);
public:
	~CLayer(void);

public:
	CComponent*		Get_Component(const TCHAR* pObjTag, const TCHAR* pComponentTag, CComponent::COMPONENTID eID);
	vector<CGameObject*>* Get_MapObject(const TCHAR* _StrMapTag);

public:
	virtual HRESULT	Ready_Layer(void);
	virtual _int	Update_Layer(const _float& fTimeDelta);
	virtual void	Render_Layer(void);

public:
	HRESULT	Add_GameObject(const TCHAR* pObjTag, CGameObject* pGameObject);

private:
	map<const TCHAR*, vector<CGameObject*>>		m_MapObject;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);

};
END
#endif // Layer_h__
