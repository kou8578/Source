using System.Collections;
using System.Collections.Generic;
using System.Text;
using Unity.VisualScripting;
using UnityEngine;
//using UnityEngine.Purchasing;
using UnityEngine.UI;


[System.Serializable]
public struct Price
{
    public int Hp_Price;
    public int Atk_Price;
    public int Atk_Spd_Price;
};

public class Shop : MonoBehaviour
{
    public int Hp_Price;
    public int Atk_Price;
    public int Atk_Spd_Price;
    public int CRI_Price;
    public int CRI_Dmg_Price;

    private int Hp_Lv_UP = 1;
    private int Atk_Lv_UP = 1;
    private int Atk_Spd_Lv_Up = 1;
    private int Cri_Lv_Up = 1;
    private int Cri_Dmg_Lv_Up = 1;

    public Text MessageText;
    public float ColorA = 0.01f;
    private StringBuilder MessageBuilder;
    private Player player;
    private Cost_Controller cost;

    public Transform StatePanel;
    Color ChangeColor;

    private void Awake()
    {
        ChangeColor = new Color(0, 0, 0, ColorA);
        Hp_Price = PlayerData.InstancePlayerDatat.Datas.Hp_Price;
        Atk_Price = PlayerData.InstancePlayerDatat.Datas.Atk_Price;
        Atk_Spd_Price = PlayerData.InstancePlayerDatat.Datas.Atk_Spd_Price;
        CRI_Price = PlayerData.InstancePlayerDatat.Datas.CRI_Price;
        CRI_Dmg_Price = PlayerData.InstancePlayerDatat.Datas.CRI_Dmg_Price;
    }
    private IEnumerator Start()
    {
        yield return null;
        yield return null;

        Get_Text("HP_LV").text = player.iAddGoldHP_LV.ToString();
        Get_Text("HP골드").text = Hp_Price.ToString();

        Get_Text("ATK_LV").text = player.iAddGoldAD_LV.ToString();
        Get_Text("ATK골드").text = Atk_Price.ToString();

        Get_Text("ATK_SPD_LV").text = player.iAddGoldATK_SPD_LV.ToString();
        Get_Text("ATK_SPD골드").text = Atk_Spd_Price.ToString();

        Get_Text("CRI_LV").text = player.iAddGoldCRI_LV.ToString();
        Get_Text("CRI골드").text = CRI_Price.ToString();

        Get_Text("CRI_DMG_LV").text = player.iAddGoldCRI_DMG_LV.ToString();
        Get_Text("CRI_DMG골드").text = CRI_Dmg_Price.ToString();

    }

    private void OnEnable()
    {
        try
        {
            player = GameObject.FindObjectOfType<Player>();
            cost = GetComponent<Cost_Controller>();
        }
        catch { }
    }
    private void Update()
    {
        Buy_Controll("Gold", Atk_Price, "공격력");
        Buy_Controll("Gold", Hp_Price, "HP");
        Buy_Controll("Gold", Atk_Spd_Price, "공격속도");
        Buy_Controll("Gold", CRI_Price, "치명타확률");
        Buy_Controll("Gold", CRI_Dmg_Price, "치명타데미지");
    }

    private Button Get_Button(string Button_Name)
    {
        Transform find = null;
        find = StatePanel.Find(Button_Name);
        if(find != null)
        {
            return find.GetComponent<Button>();
        }
        for (int i = 0; i < StatePanel.childCount; i++)
        {
            find = StatePanel.GetChild(i).Find(Button_Name);
            if(find != null) { return find.GetComponent<Button>(); }
        }
        return find.GetComponent<Button>();
    }
    private Text Get_Text(string Text_UI_Name)
    {
        Transform find = null;
        find = StatePanel.Find(Text_UI_Name);
        if (find != null)
        {
            return find.GetComponent<Text>();
        }
        for (int i = 0; i < StatePanel.childCount; i++)
        {
            find = StatePanel.GetChild(i).Find(Text_UI_Name);
            if (find != null) { return find.GetComponent<Text>(); }
        }
        return find.GetComponent<Text>();
    }

    private void Buy_Controll(string Cost_Name,int Price, string Button_Name)
    {
        if (cost.Get_Cost(Cost_Name) - Price >= 0)
        {
            Get_Button(Button_Name).interactable = true;

        }
        else
        {
            Get_Button(Button_Name).interactable = false;
        }
    }
    public void SellState_WhitGold(string CostName)
    {
        switch (CostName)
        {
            case "AD":
                player.iAddGoldAD_LV += SellStateFunction(Atk_Price, Atk_Lv_UP, "공격력 구매!");
                Atk_Price = Atk_Price + (int)(Atk_Price * 0.2f);
                Get_Text("ATK골드").text = Atk_Price.ToString();
                Get_Text("ATK_LV").text = player.iAddGoldAD_LV.ToString();
                break;
            case "HP":
                player.iAddGoldHP_LV += SellStateFunction(Hp_Price, Hp_Lv_UP, "체력 구매!");
                Hp_Price = Hp_Price + (int)(Hp_Price * 0.1f);
                Get_Text("HP골드").text = Hp_Price.ToString();
                Get_Text("HP_LV").text = player.iAddGoldHP_LV.ToString();
                break;
            case "ATK_SPD":
                player.iAddGoldATK_SPD_LV += SellStateFunction(Atk_Spd_Price, Atk_Spd_Lv_Up, "공격속도 구매");
                Atk_Spd_Price = Atk_Spd_Price + (int)(Atk_Spd_Price * 0.3f);
                Get_Text("ATK_SPD골드").text = Atk_Spd_Price.ToString();
                Get_Text("ATK_SPD_LV").text = player.iAddGoldATK_SPD_LV.ToString();
                player.SetATKSpeed();
                break;
            case "CRI":
                player.iAddGoldCRI_LV += SellStateFunction(CRI_Price, Cri_Lv_Up, "치명타확률 구매");
                CRI_Price = CRI_Price + (int)(CRI_Price * 0.8f);
                Get_Text("CRI골드").text = CRI_Price.ToString();
                Get_Text("CRI_LV").text = player.iAddGoldCRI_LV.ToString();
                break;
            case "CRI_DMG":
                player.iAddGoldCRI_DMG_LV += SellStateFunction(CRI_Dmg_Price, Cri_Dmg_Lv_Up, "치명타데미지 구매");
                CRI_Dmg_Price = CRI_Dmg_Price + (int)(CRI_Dmg_Price * 0.8f);
                Get_Text("CRI_DMG골드").text = CRI_Dmg_Price.ToString();
                Get_Text("CRI_DMG_LV").text = player.iAddGoldCRI_DMG_LV.ToString();
                break;
        }

    }

    private int SellStateFunction(int Price, int Adden, string Message)
    {
        int AddStat = 0;
        if (cost.Get_Cost("Gold") - Price >= 0)
        {
            MessageBuilder = new StringBuilder(Message);
            MessageText.text = MessageBuilder.ToString();
            cost.Cost_changer("Gold" ,-Price);
            AddStat = Adden;
        }
        else
        {
            MessageBuilder = new StringBuilder("돈 부족");
            MessageText.text = MessageBuilder.ToString();
            AddStat = 0;
        }

        StartCoroutine(FadeText());

        PlayerData.InstancePlayerDatat.Datas.iAddGoldAD_LV = player.iAddGoldAD_LV;
        PlayerData.InstancePlayerDatat.Datas.iAddGoldHP_LV = player.iAddGoldHP_LV;
        PlayerData.InstancePlayerDatat.Datas.iAddGoldATK_SPD_LV = player.iAddGoldATK_SPD_LV;
        PlayerData.InstancePlayerDatat.Datas.iAddGoldCRI_LV = player.iAddGoldCRI_LV;
        PlayerData.InstancePlayerDatat.Datas.iAddGoldCRI_DMG_LV = player.iAddGoldCRI_DMG_LV;

        PlayerData.InstancePlayerDatat.Datas.Hp_Price = Hp_Price;
        PlayerData.InstancePlayerDatat.Datas.Atk_Price = Atk_Price;
        PlayerData.InstancePlayerDatat.Datas.CRI_Price = CRI_Price;
        PlayerData.InstancePlayerDatat.Datas.CRI_Dmg_Price = CRI_Dmg_Price;

        PlayerData.InstancePlayerDatat.SavePlayerData();
        return AddStat;
    }

    IEnumerator FadeText()
    {
        MessageText.color = new Color(MessageText.color.r, MessageText.color.g, MessageText.color.b, 1f);

        while (MessageText.color.a > 0)
        {
            MessageText.color -= ChangeColor;
            yield return null;
        }
        MessageText.color = new Color(MessageText.color.r, MessageText.color.g, MessageText.color.b, 0);
    }
}
