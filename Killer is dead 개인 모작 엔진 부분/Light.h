#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase 
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);
public:
	const D3DLIGHT9*	Get_Light_Info(void) const { return &m_tLightInfo; }
	const _uint			Get_index() { return m_iIndex; }
public:
	HRESULT	Ready_Light(const D3DLIGHT9* pLightInfo,
						const _uint& iIndex);
	HRESULT Render_Light(LPD3DXEFFECT pEffect);
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	D3DLIGHT9					m_tLightInfo;
	_uint						m_iIndex;
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;

public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const D3DLIGHT9* pLightInfo, 
								const _uint& iIndex);
private:
	virtual void		Free(void);
};

END
#endif // Light_h__
