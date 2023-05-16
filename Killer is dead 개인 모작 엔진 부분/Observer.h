#ifndef Observer_h__
#define Observer_h__

#include "Engine_Defines.h"
BEGIN(Engine)

class ENGINE_DLL CObserver
{
protected:
	explicit CObserver(VOID);
public:
	virtual ~CObserver(VOID);
public:
	virtual VOID Update(INT _iMessage, INT _iDataID) PURE;
};
END
#endif // Engine_Observer_h__
