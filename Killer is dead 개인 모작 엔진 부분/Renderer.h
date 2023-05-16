#ifndef Renderer_h__
#define Renderer_h__

#include "GameObject.h"

BEGIN(Engine)

class CTarget_Manager;
class CLightMgr;
class CShader;
class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	void	Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Clear_RenderGroup(void);
	void	Setting_Light_Index(_uint iIndex) { m_Light_Index = iIndex; }

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;

private:
	list<CGameObject*>		m_RenderGroup[RENDER_END];
	CTarget_Manager*		m_pTarget_Manager = nullptr;
	CLightMgr*				m_pLight_Manager = nullptr;
private:
	CShader*				m_pShader_LightAcc = nullptr;
	CShader*				m_pShader_Blend = nullptr;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
private:
	_uint	m_Light_Index;
private:
	void	Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void	Render_Deferred();
	void	Render_LightAcc();
	void	Render_Blend();



private:
	virtual void Free(void);
};

END
#endif // Renderer_h__
