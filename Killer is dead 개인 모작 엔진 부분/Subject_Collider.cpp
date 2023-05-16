#include "Subject_Collider.h"
USING(Engine)
IMPLEMENT_SINGLETON(CSubject_Collider)

CSubject_Collider::CSubject_Collider(VOID)
{
}

CSubject_Collider::~CSubject_Collider(VOID)
{
	Release();
}

VOID CSubject_Collider::Data_Add(CONST INT & _iMessage, CONST INT & _iDataID, VOID * _pData)
{
	if (nullptr == _pData)
	{
		MSG_BOX("Subject_Collider :: Data_Add :: �߰��Ϸ��� Data�� Nullptr�Դϴ�");
		return;
	}
	//ù��°�� Map������ �޽��� �����ִ��� �˻�
	auto &Iter0 = m_mapDatamap.find(_iMessage);

	if (Iter0 == m_mapDatamap.end())
	{
		m_mapDatamap[_iMessage] = map<INT, VOID*>();
	}
	//�ִٸ� �ι�°�� Map���� �����;��̵� �ִ��� �˻�

	auto &Iter1 = m_mapDatamap[_iMessage].find(_iDataID);//.find(_iDataID); //find(Iter0->second.begin(), Iter0->second.end(), _pData);

	if (Iter1 != m_mapDatamap[_iMessage].end())
	{
		MSG_BOX("Subject_Collider : :Data_Add :: �߰��Ϸ��� DataID�� �̹� Map�� �����մϴ�");
		return;
	}

	m_mapDatamap[_iMessage].emplace(_iDataID, _pData);
}

VOID CSubject_Collider::Data_Remove(CONST INT & _iMessage, CONST INT & _iDataID, VOID * _pData)
{
	if (nullptr == _pData)
	{
		MSG_BOX("Subject_Collider :: Data_Remove :: �����Ϸ��� Data�� Nullptr�Դϴ�");
		return;
	}
	//ù��°�� Map������ �޽��� �����ִ��� �˻�
	auto &Iter0 = m_mapDatamap.find(_iMessage);

	if (Iter0 == m_mapDatamap.end())
	{
		MSG_BOX("Subject_Collider :: Data_Remove :: �޼��� Ÿ���� Map�� ���������ʽ��ϴ�");
		return;
	}
	//�ִٸ� �ι�°�� Map���� �����;��̵� �ִ��� �˻�

	auto &Iter1 = Iter0->second.find(_iDataID); //find(Iter0->second.begin(), Iter0->second.end(), _pData);

	if (Iter1 == Iter0->second.end())
	{
		MSG_BOX("Subject_Collider :: Data_Remove :: �����Ϸ��� DataID�� Map�� ���������ʽ��ϴ�");
		return;
	}

	Iter0->second.erase(Iter1);
}

VOID* CSubject_Collider::Get_Data(CONST INT & _iMessage, CONST INT & _iDataID)
{
	//ù��°�� Map������ �޽��� �����ִ��� �˻�
	auto &Iter0 = m_mapDatamap.find(_iMessage);

	if (Iter0 == m_mapDatamap.end())
	{
		MSG_BOX("Subject_Collider :: Get_Data :: �޼��� Ÿ���� Map�� ���������ʽ��ϴ�");
		return nullptr;
	}
	//�ִٸ� �ι�°�� Map���� �����;��̵� �ִ��� �˻�

	auto &Iter1 = Iter0->second.find(_iDataID); //find(Iter0->second.begin(), Iter0->second.end(), _pData);

	if (Iter1 == Iter0->second.end())
	{
		MSG_BOX("Subject_Collider :: Get_Data :: �����Ϸ��� DataID�� Map�� ���������ʽ��ϴ�");
		return nullptr;
	}

	return Iter1->second;
}

VOID CSubject_Collider::Release(VOID)
{
	for (auto &FirstMapIter : m_mapDatamap)
	{
		for (auto &SecondMapIter : FirstMapIter.second)
		{
				Safe_Delete(SecondMapIter.second);
		}
	}
	m_mapDatamap.clear();
}

VOID CSubject_Collider::All_Remove(VOID)
{
	m_mapDatamap.clear();
	/*for (auto &FirstMapIter : m_mapDatamap)
	{
	for (auto &SecondMapIter : FirstMapIter.second)
	{

	}
	}

	unordered_map<INT, map<INT, VOID*>>
	m_mapDatamap*/
}
