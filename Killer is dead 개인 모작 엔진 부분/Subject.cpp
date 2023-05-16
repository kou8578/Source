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
		MSG_BOX("Subject :: Observer_Add :: �߰��Ϸ��� �������� Nullptr �̾����ϴ�");
		return;
	}

	m_ListObserver.emplace_back(_pObserver);
}

VOID CSubject::Observer_Remove(CObserver * _pObserver)
{
	if (nullptr == _pObserver)
	{
		MSG_BOX("Subject :: Observer_Remove :: �����Ϸ��� �������� Nullptr �̾����ϴ�");
		return;
	}

	auto &Iter = find(m_ListObserver.begin(), m_ListObserver.end(), _pObserver);

	if (Iter == m_ListObserver.end())
	{
		MSG_BOX("Subject :: Observer_Remove :: �����Ϸ��� �������� ����Ʈ�� ���������ʽ��ϴ�");
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
