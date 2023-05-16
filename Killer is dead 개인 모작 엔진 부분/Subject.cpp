#include "Subject.h"
#include "Observer.h"

USING(Engine)

CSubject::CSubject(VOID)
{
}

CSubject::~CSubject(VOID)
{
	Release();
}

VOID CSubject::Observer_Add(CObserver * _pObserver)
{
	if (nullptr == _pObserver)
	{
		MSG_BOX("Subject :: Observer_Add :: 추가하려는 옵저버가 Nullptr 이었습니다");
		return;
	}

	m_ListObserver.emplace_back(_pObserver);
}

VOID CSubject::Observer_Remove(CObserver * _pObserver)
{
	if (nullptr == _pObserver)
	{
		MSG_BOX("Subject :: Observer_Remove :: 제거하려는 옵저버가 Nullptr 이었습니다");
		return;
	}

	auto &Iter = find(m_ListObserver.begin(), m_ListObserver.end(), _pObserver);

	if (Iter == m_ListObserver.end())
	{
		MSG_BOX("Subject :: Observer_Remove :: 제거하려는 옵저버는 리스트에 존재하지않습니다");
		return;
	}
	m_ListObserver.erase(Iter);
}


VOID CSubject::Notify(INT _iMessage, INT _iDataID)
{
	for (auto &Iter : m_ListObserver)
		Iter->Update(_iMessage, _iDataID);
}

VOID CSubject::Release(VOID)
{
	m_ListObserver.clear();
}
