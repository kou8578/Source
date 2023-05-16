using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using static Save_Load_Sc;

public class PlayerManager : MonoBehaviour
{
    public GameObject PlayerData_Pos;
    public Transform Player;
    public List<Hero> ListHero;
    public Hero CurrentHero;

    [System.Obsolete]
    private void Awake()
    {
        instance.HeroList = JsonUtility.FromJson<HeroDatas>(instance.Load("json/HERO_Data.json"));
        if (instance.HeroList.HERO_TABLE.Count == 0)
        {
            Debug.LogError("Json is null or Different Path");
        }
    }

    public void ChangeCharater(string CodeName)
    {
        try
        {
            GameObject CharaterDataPrefab = Instantiate(Resources.Load<GameObject>("CharacterDataPrefab/" + CodeName), Player);
            if (CharaterDataPrefab == null)
            {
                return;
            }
            Destroy(PlayerData_Pos);
            PlayerData_Pos = CharaterDataPrefab;
            //애니메이션 교체
            Player.GetComponent<Animation_Contorller>().Change_Animator(PlayerData_Pos);
            //상태 및 체력등의 수치 교체
            Player.GetComponent<Player>().Change_State(Unit.State.IDLE);
            //Player.GetComponent<Player>().fEXP = 0.0f;

            ListHero = Save_Load_Sc.instance.HeroList.HERO_TABLE;
            int index = ListHero.FindIndex(x => x.HERO_CODE_NAME == CodeName);
            CurrentHero = ListHero[index];

            Player.GetComponent<Player>().SeleteHero = CurrentHero;
            Player.GetComponent<Player>().SetMaxHp();
            
            //기타 설정 들
            Player.GetComponent<Hit>().Unit_hp = CharaterDataPrefab.GetComponentInChildren<Hp_Bar>();
            Player.GetComponent<Player>().fLevelUpAddHp = CharaterDataPrefab.GetComponentInChildren<Hp_Bar>().Set_Hp();
            
            StartCoroutine(SettingData());
        }
        catch(System.Exception e) 
        {
            Debug.LogError(e.Message);
        }
        PlayerData.InstancePlayerDatat.Datas.CurrentCharater = CodeName;
        PlayerData.InstancePlayerDatat.SavePlayerData();
    }
    
    private IEnumerator SettingData()
    {
        yield return null;
        if (Player.GetComponent<Player>().Find_Skill_To_Tag("Attack") != null)
        {
            Player.GetComponent<Player>().Base_ATKSpeed = CurrentHero.ATK_SPD;
            Player.GetComponent<Player>().SetATKSpeed();
        }
        Player.GetComponent<Player>().Change_State(Unit.State.IDLE);
    }
}
