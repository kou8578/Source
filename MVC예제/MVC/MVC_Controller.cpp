#include "MVC_Controller.h"

MVC_Controller::MVC_Controller(MVC_Model & _Model, MVC_View & _View)
	:Model(_Model) , VIew(_View)
{
}

void MVC_Controller::User_Input(int _a, int _b)
{
	//Model�� ���� �Է��� �޾� ó��
	int result = Model.Model_Activate(_a, _b);
	//Model���� ������Ʈ�� �����͸� View�� Notify
	VIew.Display_Result(result);
}
