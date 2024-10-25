#pragma once
#include "MVC_Model.h"
#include "MVC_VIew.h"
class MVC_Controller
{
public:
	MVC_Controller(MVC_Model &_Model, MVC_View &_View);
	
	void User_Input(int _a, int _b);

private:
	MVC_Model Model;
	MVC_View VIew;
};