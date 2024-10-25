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

	cout << "A값 입력 : ";
	cin >> Input_A;

	cout << "B값 입력 : ";
	cin >> Input_B;

	Controller.User_Input(Input_A, Input_B);

	std::cout << "종료하려면 아무 키나 입력하세요" << std::endl;
	std::cin.ignore();

	return 0;
}