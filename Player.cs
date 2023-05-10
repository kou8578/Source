using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;


public class Player : Unit
{
    public delegate void MonsyerKill(Monster data);
    public delegate void MonsyerKillPassive();
    public delegate void MonsyerKillPassivePos(Vector3 pos);
    private StageManager stageManager;
    public event MonsyerKill KillEnemy;
    public event MonsyerKillPassive PassiveEnemyKill;
    public event MonsyerKillPassivePos PassiveEnemyKillPos;

    public float fEXP = 0;
    public float fMaxExp = 100;
    public float fLevelUpAddHp = 1000;
    public int LEVEL = 1;
    private Player_AI PlayerAI;
    private Animation_Contorller _AnimationCon;

    [Header("\n현재 활성화 스킬 넘버, 스킬 해금 레벨 설정 (1, 2, 3 )")]
    public int CurrentAtiveSkill = 0;
    public int[] SkillAtiveLevel = new int [] { 1,2,3 };


    [Header("\n현재 히어로 데이터")]
    public Hero SeleteHero;

    [Header("\n골드 스텟 레벨")]
    public int iAddGoldHP_LV;
    public int iAddGoldAD_LV;
    public int iAddGoldATK_SPD_LV;
    public int iAddGoldCRI_LV;
    public int iAddGoldCRI_DMG_LV;

    [Header("\n골드 스텟 배율")]
    public float fGold_ATKPercent = 100;
    [Range(0, 0.1f)]
    public float fGoldATK_SPDPercent = 0.01f;
    public float Base_AnimationSpeed = 1;
    public float Base_ATKSpeed = 3;
    public State Get_State() { return state; }

    public void Awake()
    {
        KillEnemy += AddExp;
        PlayerAI = GetComponent<Player_AI>();
        stageManager = GameObject.FindObjectOfType<StageManager>();
        _AnimationCon = GetComponent<Animation_Contorller>();

        LEVEL = PlayerData.InstancePlayerDatat.Datas.LEVEL;
        fEXP = PlayerData.InstancePlayerDatat.Datas.fEXP;
        fMaxExp = PlayerData.InstancePlayerDatat.Datas.fMaxEXP;
        CurrentAtiveSkill = PlayerData.InstancePlayerDatat.Datas.CurrentAtiveSkill;
        iAddGoldHP_LV = PlayerData.InstancePlayerDatat.Datas.iAddGoldHP_LV;
        iAddGoldAD_LV = PlayerData.InstancePlayerDatat.Datas.iAddGoldAD_LV;
        iAddGoldATK_SPD_LV = PlayerData.InstancePlayerDatat.Datas.iAddGoldATK_SPD_LV;
        iAddGoldCRI_LV = PlayerData.InstancePlayerDatat.Datas.iAddGoldCRI_LV;
        iAddGoldCRI_DMG_LV = PlayerData.InstancePlayerDatat.Datas.iAddGoldCRI_DMG_LV;
        GameObject.Find("UI").transform.Find("레벨").GetComponent<Text>().text = "LEVEL " + LEVEL;
    }

    public IEnumerator Start()
    {
        yield return null;
        yield return null;
        yield return null;
        string CharaterName = PlayerData.InstancePlayerDatat.Datas.CurrentCharater;
        FindObjectOfType<PlayerManager>().ChangeCharater(CharaterName);
        //SetATKSpeed();
        //
    }

    public void AddExp(Monster monster_data)
    {
        fEXP += monster_data.DropEXP * stageManager.CurrentStage;
        CheckLevelUp();
        FindObjectOfType<EXP_Bar>().Exp_Up();
    }

    private void CheckLevelUp()
    {
        if (fEXP >= fMaxExp)
        {
            LEVEL++;
            if (CurrentAtiveSkill < SkillAtiveLevel.Length)
            {
                CheackSkillAtiveLevel(SkillAtiveLevel[CurrentAtiveSkill]);
            }
            fEXP = fEXP - fMaxExp;
            fMaxExp = fMaxExp * LEVEL * 0.8f;
            SetMaxHp();
            FindObjectOfType<EXP_Bar>().Exp_Up();
            GameObject.Find("UI").transform.Find("레벨").GetComponent<Text>().text = "LEVEL " + LEVEL;
            CheckLevelUp();
        }
        else
        {
            PlayerData.InstancePlayerDatat.Datas.fEXP = fEXP;
            PlayerData.InstancePlayerDatat.Datas.fMaxEXP = fMaxExp;
            PlayerData.InstancePlayerDatat.Datas.CurrentAtiveSkill = CurrentAtiveSkill;
            PlayerData.InstancePlayerDatat.Datas.LEVEL = LEVEL;
            PlayerData.InstancePlayerDatat.SavePlayerData();
        }
    }

    public void SetMaxHp()
    {
        MaxHP = SeleteHero.HP + Mathf.CeilToInt(LEVEL * fLevelUpAddHp);
        HP = MaxHP;
        GetComponentInChildren<Hp_Bar>().Set_Hp();
    }

    public void CheackSkillAtiveLevel(int SkillNumber)
    {
        if(SkillNumber <= LEVEL)
        {
            PlayerAI.MaxCount = CurrentAtiveSkill + 1;
            CurrentAtiveSkill = 
                (CurrentAtiveSkill < SkillAtiveLevel.Length) ? CurrentAtiveSkill + 1 : SkillAtiveLevel.Length-1;
        }
    }

    public void PlayerKillMonster(Monster monster_data)
    {
        if (KillEnemy != null)
        {
            KillEnemy(monster_data);
        }
        if (PassiveEnemyKill != null)
        {
            PassiveEnemyKill();
        }
        if(PassiveEnemyKillPos != null)
        {
            PassiveEnemyKillPos(monster_data.transform.position);
        }
    }

    //치확=골드 능력치 + 아이템 능력치 + 버프 스킬 능력치
    //치명타 데미지 = 최종 공격력 * (기본 치명타 데미지 + 골드 능력치 + 아이템 능력치 + 버프 스킬 능력치)(단위는 %)
    //치명타 데미지가 공격력의 0.5배인걸로 Ex 공 100짜리는 150 이런 형식

    public int GetPlayerDamage(float ATKPercent)
    {
        //치명타가 떳는지 안떳는지 확인 후 치명타가 떳으면 1.5 + 치명타 데미지 만큼 곱해서 return;
        //아이템은 추가해야함.
        float CriP = CRI * (iAddGoldCRI_LV + iAddSkillCRI) / 100;
        bool IsCri = CriP > Random.value;
        if (IsCri)
        {
            return Mathf.FloorToInt((SeleteHero.ATK + ((LEVEL * ATKPercent) + (iAddGoldAD_LV * fGold_ATKPercent)) * +iAddSkillAD) * (1 + ((CRI_DMG + iAddGoldCRI_DMG_LV) / 100f)));
        }
        return Mathf.FloorToInt(SeleteHero.ATK + ((LEVEL * ATKPercent) + (iAddGoldAD_LV * fGold_ATKPercent)) * +iAddSkillAD);
    }
    //만약 치명타일 경우 어떤 효과가 있을땐 bool 형태로 받아서 처리.
    public int GetPlayerDamage(float ATKPercent ,ref bool CriCheck)
    {
        //치명타가 떳는지 안떳는지 확인 후 치명타가 떳으면 1.5 + 치명타 데미지 만큼 곱해서 return;
        //아이템은 추가해야함.
        float CriP = CRI * (iAddGoldCRI_LV + iAddSkillCRI) / 100;
        CriCheck = CriP > Random.value;
        if(CriCheck)
        {
            return Mathf.FloorToInt((SeleteHero.ATK + ((LEVEL * ATKPercent) + (iAddGoldAD_LV * fGold_ATKPercent)) * +iAddSkillAD) * (1 + ((CRI_DMG + iAddGoldCRI_DMG_LV) / 100f)));
        }
        return Mathf.FloorToInt(SeleteHero.ATK + ((LEVEL * ATKPercent) + (iAddGoldAD_LV * fGold_ATKPercent)) * +iAddSkillAD);
    }

    public void SetATKSpeed()
    {
        if (Find_Skill_To_Tag("Attack") != null)
        {
            if (_AnimationCon != null)
            {
                _AnimationCon.Set_Animation_Speed(Base_AnimationSpeed + (0.01f * iAddGoldATK_SPD_LV));
                Find_Skill_To_Tag("Attack").Skill_CoolTime = Base_ATKSpeed - (Base_ATKSpeed * (fGoldATK_SPDPercent * iAddGoldATK_SPD_LV));
            }
        }
    }
}
