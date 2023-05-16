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
		MSG_BOX("Subject_Collider :: Data_Add :: 추가하려는 Data가 Nullptr입니다");
		return;
	}
	//첫번째의 Map에에서 메시지 맵이있는지 검사
	auto &Iter0 = m_mapDatamap.find(_iMessage);

	if (Iter0 == m_mapDatamap.end())
	{
		m_mapDatamap[_iMessage] = map<INT, VOID*>();
	}
	//있다면 두번째의 Map에서 데이터아이디가 있는지 검사

	auto &Iter1 = m_mapDatamap[_iMessage].find(_iDataID);//.find(_iDataID); //find(Iter0->second.begin(), Iter0->second.end(), _pData);

	if (Iter1 != m_mapDatamap[_iMessage].end())
	{
		MSG_BOX("Subject_Collider : :Data_Add :: 추가하려는 DataID가 이미 Map에 존재합니다");
		return;
	}

	m_mapDatamap[_iMessage].emplace(_iDataID, _pData);
}

VOID CSubject_Collider::Data_Remove(CONST INT & _iMessage, CONST INT & _iDataID, VOID * _pData)
{
	if (nullptr == _pData)
	{
		MSG_BOX("Subject_Collider :: Data_Remove :: 제거하려는 Data가 Nullptr입니다");
		return;
	}
	//첫번째의 Map에에서 메시지 맵이있는지 검사
	auto &Iter0 = m_mapDatamap.find(_iMessage);

	if (Iter0 == m_mapDatamap.end())
	{
		MSG_BOX("Subject_Collider :: Data_Remove :: 메세지 타입이 Map에 존재하지않습니다");
		return;
	}
	//있다면 두번째의 Map에서 데이터아이디가 있는지 검사

	auto &Iter1 = Iter0->second.find(_iDataID); //find(Iter0->second.begin(), Iter0->second.end(), _pData);

	if (Iter1 == Iter0->second.end())
	{
		MSG_BOX("Subject_Collider :: Data_Remove :: 제거하려는 DataID가 Map에 존재하지않습니다");
		return;
	}

	Iter0->second.erase(Iter1);
}

VOID* CSubject_Collider::Get_Data(CONST INT & _iMessage, CONST INT & _iDataID)
{
	//첫번째의 Map에에서 메시지 맵이있는지 검사
	auto &Iter0 = m_mapDatamap.find(_iMessage);

	if (Iter0 == m_mapDatamap.end())
	{
		MSG_BOX("Subject_Collider :: Get_Data :: 메세지 타입이 Map에 존재하지않습니다");
		return nullptr;
	}
	//있다면 두번째의 Map에서 데이터아이디가 있는지 검사

	auto &Iter1 = Iter0->second.find(_iDataID); //find(Iter0->second.begin(), Iter0->second.end(), _pData);

	if (Iter1 == Iter0->second.end())
	{
		MSG_BOX("Subject_Collider :: Get_Data :: 제거하려는 DataID가 Map에 존재하지않습니다");
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
