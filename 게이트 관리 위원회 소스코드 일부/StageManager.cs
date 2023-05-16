using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

public class StageManager : MonoBehaviour
{
    [Header("\nStage Setting ")]
    public List<StageData> stageDatas;
    public Tile_Setting TileManager;
    public List<Unit_Pool> UnitPools;
    public int CountEnemy = 0;
    public int CurrentStage = 1;
    public int ChangeStageCount = 10;
    public int AddMonsterCurrentStage = 0;
    public int MixCountMonster = 10;

    [Header("\nFade Setting ( 페이드 관련 설정 ) ")]
    public float FadeTime;
    public AnimationCurve fadeCurver;
    private Image _Image;
    public Text DieText;
    
    private int StageCount = 1;
    private Player player;
    public Transform UICanvers;
    private Cost_Controller cost_controller;
    public AudioSource BGMAudio;
    private int DieEffectNumber = -1;
    private int LiveEffectNumber = -1;

    private void Awake()
    {
        CurrentStage = PlayerData.InstancePlayerDatat.Datas.CurrentStage;
        Initialized_Compent();
        Initalized_Other();
        StartCoroutine(SetStageData());
    }
    private void Start()
    {
        Stage_UI_Count();
    }

    private IEnumerator SetStageData()
    {
        yield return null;yield return null;yield return null;
        CurrentStage -= 1;
        ChangeStageData();
    }
    private void Update()
    {
        if(player.HP <= 0)
        {
            PlayerDie();
        }
    }

    public void PlayerDie()
    {
        if((DieEffectNumber == -1) && (LiveEffectNumber == -1))
        {
            LiveEffectNumber = EffectPools.instance.GetEffectNumber("LiveEffect");
            DieEffectNumber = EffectPools.instance.GetEffectNumber("DieEffect");
        }
        if (CurrentStage % 10 == 0)
        {
            CurrentStage -= 10;
        }
        else
        {
            CurrentStage -= CurrentStage % 10;
        }

        player.SetMaxHp();
        player.gameObject.GetComponent<Player_AI>().enabled = false;
        player.animation_Contorller.Character_sprite.color = Color.blue;
        player.GetComponent<Move>().enabled = false; //이동 뿐 아니라 Hit자체를 비활성화 할것.
        player.GetComponent<Collider2D>().enabled= false;
        player.animation_Contorller.Move_Stop();

        EffectPools.instance.SpawnEffect(player.transform.position, DieEffectNumber);

    }
    private void Initalized_Other()
    {
        int count = 0;
        while (count < UnitPools.Count)
        {
            UnitPools[count].stageManager = this;
            CountEnemy += UnitPools[count].Pooling_Count;
            count++;
        }
    }

    private void Initialized_Compent()
    {
        try
        {
            _Image = GetComponentInChildren<Image>();
            player = FindObjectOfType<Player>();
            cost_controller = GetComponent<Cost_Controller>();
        }
        catch (System.Exception ex)
        {
            Debug.LogException(ex);
        }
    }
    private void Stage_UI_Count()
    {
        UICanvers.transform.Find("STAGE").GetComponent<Text>().text = "STAGE " + CurrentStage.ToString();
    }
    public void ChangeStageData()
    {
        try
        {
            player.SetMaxHp();
            CountEnemy = 0;
            CurrentStage = CurrentStage + 1;
            Stage_UI_Count();
            int CurrentCount = StageCount;
            StageCount = Mathf.FloorToInt(CurrentStage / ChangeStageCount);
            StageCount %= stageDatas.Count;
            int count = 0;

            while (count < UnitPools.Count)
            {
                if (StageCount != CurrentCount)
                {
                    UnitPools[count].Set_Pooling_Object(
                        stageDatas[StageCount].
                        Monster[Random.Range(0, stageDatas[StageCount].Monster.Count)],
                        CurrentStage
                        );
                }
                UnitPools[count].ResetPool();
                UnitPools[count].Pooling_Count = Mathf.Min(CurrentStage, MixCountMonster);
                UnitPools[count].enabled = true;
                CountEnemy += UnitPools[count].Pooling_Count;
                UnitPools[count].CurrentStatge = CurrentStage;
                count++;
            }

            TileManager.Change_Random(stageDatas[StageCount].TilePath);

            if ((BGMAudio != null) && (stageDatas[StageCount].BGM != null))
            {
                if (BGMAudio.clip != stageDatas[StageCount].BGM)
                {
                    BGMAudio.clip = stageDatas[StageCount].BGM;
                    BGMAudio.Play();
                }
            }
            PlayerData.InstancePlayerDatat.Datas.CurrentStage = CurrentStage;
            PlayerData.InstancePlayerDatat.SavePlayerData();
        }
        catch (System.Exception error)
        {
            Debug.LogError("스테이지 교체 에러 !! 내용 : " + error.Message);
        }
    }

    public void EnemyDie(Monster Monster)
    {
        CountEnemy -= 1;
        player.PlayerKillMonster(Monster);
        cost_controller.Cost_changer("Gold", CurrentStage * Monster.DropMoney);//임시
        
        if (CountEnemy == 0)
        {
            StartCoroutine(Fade(0.0f));
        }
    }

    private IEnumerator Fade(float StartColor_a)
    {
        float currentTime = 0.0f;
        float percent = 0.0f;
        float End = StartColor_a == 0 ? 1.0f : 0.0f;
        Color color = _Image.color;

        while (percent < 1)
        {
            currentTime += Time.deltaTime;
            percent = currentTime / FadeTime;
            
            color.a = Mathf.Lerp(StartColor_a, End, fadeCurver.Evaluate(percent));

            _Image.color = color;

            yield return null;
        }

        currentTime = 0.0f;
        percent = 0.0f;

        ChangeStageData();

        while (percent < 1)
        {
            currentTime += Time.deltaTime;
            percent = currentTime / FadeTime;

            color.a = Mathf.Lerp(End, StartColor_a, fadeCurver.Evaluate(percent));

            _Image.color = color;

            yield return null;
        }
    }
    public void TimeScaleUp(float scaletime)
    {
        Time.timeScale = scaletime;
    }

    public void MoreFastTime(Toggle ToggleButton)
    {
        if (ToggleButton.isOn) { TimeScaleUp(2f); }
        else { TimeScaleUp(1f); }
    }
    public IEnumerator FadePlayerDieUI()
    {
        float currentTime = 0.0f;
        float percent = 0.0f;
        float End = 1.0f;
        Color color = DieText.color;
       
        while (percent < 1)
        {
            currentTime += Time.deltaTime;
            percent = currentTime / FadeTime;

            color.a = Mathf.Lerp(0.0f, End, fadeCurver.Evaluate(percent));

            _Image.color = color;

            yield return null;
        }
        player.animation_Contorller.Character_sprite.enabled = false;
        currentTime = 0.0f;
        percent = 0.0f;
        //요시점에 모든 몬스터 비활성화.
        List<GameObject> Enemys = GameObject.FindGameObjectsWithTag("Monster").ToList();
        for(int i = 0; i < Enemys.Count; i++)
        {
            Enemys[i].SetActive(false);
        }
        Enemys.Clear();
        while (percent < 1)
        {
            currentTime += Time.deltaTime;
            percent = currentTime / FadeTime;

            color.a = Mathf.Lerp(End, 0.0f, fadeCurver.Evaluate(percent));

            _Image.color = color;

            yield return null;
        }

        EffectPools.instance.SpawnEffect(player.transform.position, LiveEffectNumber);
    }

    public void PlayerLiveEffectEndFunction()
    {
        player.animation_Contorller.Character_sprite.enabled = true;
        player.animation_Contorller.Character_sprite.color = Color.white;

        player.gameObject.GetComponent<Player_AI>().enabled = true;
        player.GetComponent<Move>().enabled = true;
        player.GetComponent<Collider2D>().enabled = true;

        ChangeStageData();
    }

    public void PlayerDieEffectEndFunction()
    {
        StartCoroutine(FadePlayerDieUI());
    }
}