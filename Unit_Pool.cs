using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unit_Pool : MonoBehaviour
{
    [SerializeField]
    private GameObject Pooling_Unit_GameObject_prefab;
    private Transform Player;

    public int Pooling_Count = 10;
    public int Field_Unit_Count = 0;
    public float Pooling_Delay_Time = 2f;
    private float Pooling_Current_Time = 0f;
    public StageManager stageManager = null;
    public float SpawnDistance = 15f;

    public int CurrentStatge = 1;

    Queue<Unit> Pooling_Unit_Queue = new Queue<Unit>();
    List<Unit> Field_Unit_List = new List<Unit>();

    private void Awake()
    {
        Player = GameObject.Find("Player").transform;
    }

    public void Set_Pooling_Object(GameObject _Pooling_Unit_GameObject_prefab, int StageNumber)
    {
        CurrentStatge = StageNumber;
        Pooling_Unit_GameObject_prefab = _Pooling_Unit_GameObject_prefab;
        while(Pooling_Unit_Queue.Count != 0)
        {
            Destroy(Pooling_Unit_Queue.Peek().gameObject);
            Pooling_Unit_Queue.Dequeue();
        }
        
        for (int i = 0; i< Field_Unit_List.Count; i++)
        {
            Destroy(Field_Unit_List[i].gameObject);
        }
        Field_Unit_List.Clear();
        ResetPool();
        Initialize(Pooling_Count);
    }

    public void ResetPool()
    {
        //Field_Unit_List.Clear();
        for (int i = Field_Unit_List.Count - 1; i >= 0; i--) 
        {
            ReturnObject(Field_Unit_List[i]);
            Field_Unit_List.Remove(Field_Unit_List[i]);
        }
        Field_Unit_Count = 0;
        Pooling_Current_Time = 0;
    }

    void Update()
    {
        Pooling_Current_Time += Time.deltaTime;
        Pooling();
        Pooling_Unit_Return_Check();
     
    }

    private void Initialize(int initCount)
    {
        for (int i = 0; i < initCount; i++)
        {
            Pooling_Unit_Queue.Enqueue(CreateNewObject());
        }
    }
    private Unit CreateNewObject()
    {
        var newObj = Instantiate(Pooling_Unit_GameObject_prefab).GetComponent<Unit>();
        newObj.gameObject.SetActive(false);
        newObj.transform.position = gameObject.transform.position;
        try { newObj.GetComponent<Monster>().SetStateMonster(CurrentStatge); }
        catch { }
        return newObj;
    }
    public  Unit GetObject()
    {
        Vector3 SpawnPoint = (Random.insideUnitCircle.normalized) * SpawnDistance;
        if (Pooling_Unit_Queue.Count > 0)
        {
            var obj = Pooling_Unit_Queue.Dequeue();
            obj.GetComponent<Monster>().SetStateMonster(CurrentStatge);
            obj.transform.position = transform.position;
            obj.gameObject.transform.position = SpawnPoint + Player.position;
            obj.gameObject.SetActive(true);
            return obj;
        }
        else
        {
            var newObj = CreateNewObject();
            newObj.gameObject.SetActive(true);
            newObj.transform.position = SpawnPoint + Player.position;
            return newObj;
        }
    }
    public  void ReturnObject(Unit obj)
    {
        obj.gameObject.SetActive(false);
        obj.transform.position = transform.position;
        obj.GetComponent<Monster>().SetStateMonster(CurrentStatge);

        Pooling_Unit_Queue.Enqueue(obj);
    }


    private void Pooling()
    {
        if (Field_Unit_Count >= Pooling_Count) return;
        if(Pooling_Current_Time >= Pooling_Delay_Time)
        {
            Pooling_Current_Time = 0f;
            Field_Unit_List.Add(GetObject());
            Field_Unit_Count++;
        }
    }
    private void Pooling_Unit_Return_Check()
    {
        for (int i = Field_Unit_List.Count - 1; i >= 0; i--) //순회시 오류 방지를 위한 역순 순회
        {
            if (Field_Unit_List[i].HP <= 0)
            {
                stageManager.EnemyDie(Field_Unit_List[i].gameObject.GetComponent<Monster>());
                ReturnObject(Field_Unit_List[i]);
                Field_Unit_List.Remove(Field_Unit_List[i]);
            }
        }
    }
}
