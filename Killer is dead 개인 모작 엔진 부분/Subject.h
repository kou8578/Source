#ifndef Subject_h__
#define Subject_h__

#include "Engine_Defines.h"

BEGIN(Engine)
class CObserver;
class ENGINE_DLL CSubject
{
protected:
	explicit CSubject(VOID);
public:
	virtual ~CSubject(VOID);
public:
	VOID Observer_Add(CObserver* _pObserver);
	VOID Observer_Remove(CObserver* _pObserver);
	VOID Notify(INT _iMessage, INT _iDataID);
private:
	VOID Release(VOID);
private:
	list<CObserver*> m_ListObserver;
};
END
#endif // Engine_Subject_h__
