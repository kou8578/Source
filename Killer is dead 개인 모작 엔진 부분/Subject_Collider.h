#ifndef Subject_Collider_h__
#define Subject_Collider_h__

#include "Subject.h"
#include <unordered_map>

BEGIN(Engine)
class ENGINE_DLL CSubject_Collider
	:public CSubject
{
	DECLARE_SINGLETON(CSubject_Collider)
private:
	explicit CSubject_Collider(VOID);
public:
	~CSubject_Collider(VOID);
public:
	VOID Data_Add(CONST INT& _iMessage, CONST INT& _iDataID, VOID* _pData);
	VOID Data_Remove(CONST INT& _iMessage, CONST INT& _iDataID, VOID* _pData);

public:
	VOID* Get_Data(CONST INT& _iMessage, CONST INT& _iDataID);
private:
	VOID Release(VOID);
public:
	VOID All_Remove(VOID);
private:

	unordered_map<INT, map<INT, VOID*>> m_mapDatamap;
};
END
#endif // Engine_Subject_Collider_h__
