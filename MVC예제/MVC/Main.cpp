#include "MVC_Controller.h"
#include "MVC_Model.h"
#include "MVC_VIew.h"

int main(void)
{
	MVC_Model Model;
	MVC_View View;
	MVC_Controller Controller(Model,View);

	int Input_A;
	int Input_B;

	cout << "A�� �Է� : ";
	cin >> Input_A;

	cout << "B�� �Է� : ";
	cin >> Input_B;

	Controller.User_Input(Input_A, Input_B);

	std::cout << "�����Ϸ��� �ƹ� Ű�� �Է��ϼ���" << std::endl;
	std::cin.ignore();

	return 0;
}