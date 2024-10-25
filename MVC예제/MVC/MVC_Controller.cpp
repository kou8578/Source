#include "MVC_Controller.h"

MVC_Controller::MVC_Controller(MVC_Model & _Model, MVC_View & _View)
	:Model(_Model) , VIew(_View)
{
}

void MVC_Controller::User_Input(int _a, int _b)
{
	//Model이 유저 입력을 받아 처리
	int result = Model.Model_Activate(_a, _b);
	//Model에서 업데이트된 데이터를 View에 Notify
	VIew.Display_Result(result);
}
