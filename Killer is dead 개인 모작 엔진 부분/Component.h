#ifndef Component_h__
#define Component_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CComponent 
{
	// ������Ʈ �Ӽ��� ���� �з�
	// DYNAMIC : �ǽð����� �� �����Ӹ��� ������Ʈ �Ǿ�� �� ������Ʈ ����
	// STATIC :  �����Ӹ��� ������Ʈ �� �ʿ䰡 ���� ������Ʈ ����
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

