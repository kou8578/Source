#ifndef Component_h__
#define Component_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CComponent 
{
	// 컴포넌트 속성에 따른 분류
	// DYNAMIC : 실시간으로 매 프레임마다 업데이트 되어야 할 컴포넌트 집합
	// STATIC :  프레임마다 업데이트 할 필요가 없는 컴포넌트 집합
public:
	enum COMPONENTID {		ID_DYNAMIC, ID_STATIC, ID_END	};

protected:
	explicit CComponent();
public:
	~CComponent();

public:
	virtual void Update_Component(const _float& fTimeDelta) {}
	virtual CComponent*	Clone(void) { return nullptr; }

public:
	virtual void Free(void)PURE;

};

END
#endif // Component_h__

