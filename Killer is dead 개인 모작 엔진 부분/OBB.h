#ifndef OBB_h__
#define OBB_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL COBB : public CComponent
{
private:
	explicit COBB(void);
public:
	~COBB(void);

public:
	_bool CheckOBB(OBB* _A_OBB , OBB* _B_OBB);
public:
	static COBB*	Create(void);
private:
	virtual void Free(void);

};

END
#endif // OBB_h__
