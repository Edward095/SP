#include "c_Garage.h"

// +++++++++++++++++++++++++ CAR CLASS +++++++++++++++++++++++++++

c_Car::c_Car()
{
}

int c_Car::f_GetCarNum()
{
	return v_CarNum;
}

void c_Car::f_SetCarNum(int carNumber)
{
	this->v_CarNum = carNumber;
}

c_Car::~c_Car()
{
}

// +++++++++++++++++++++++++ COLOUR CLASS ++++++++++++++++++++++++

c_Colour::c_Colour()
{

}

int c_Colour::f_GetColourNum()
{
	return v_ColourNum;
}

void c_Colour::f_SetColourNum(int colour)
{
	switch (colour)
	{
	case 0:
		v_ColourNum = Blue;
		break;
	case 1:
		v_ColourNum = Red;
		break;
	case 2:
		v_ColourNum = Purple;
		break;
	case 3:
		v_ColourNum = Green;
		break;
	default:
		break;
	}
}

c_Colour::~c_Colour()
{

}

// +++++++++++++++++++++ CAR LINK LIST CLASS +++++++++++++++++++++

c_List::c_List()
{
	for (int i = 0; i < 3; i++)
	{
		v_CarList[i] = new c_Car;
		v_CarList[i]->f_SetCarNum(i);
	}
	for (int i = 0; i < 4; i++)
	{
		v_ColourList[i] = new c_Colour;
		v_ColourList[i]->f_SetColourNum(i);
	}
	v_CurColour = v_ColourList[0];
	v_CurCar = v_CarList[0];
	v_IndexCar = 0;
	v_IndexColour = 0;
}

void c_List::f_ChangeCurrentCar(char input)
{
	switch (input)
	{
	case 'O':
		v_IndexCar--;
		break;
	case 'P':
		v_IndexCar++;
		break;
	default:
		break;
	}
	v_CurCar = v_CarList[v_IndexCar];
}

void c_List::f_ChangeCurrentColour(char input)
{
	switch (input)
	{
	case 'K':
		v_IndexColour--;
		break;
	case 'L':
		v_IndexColour++;
		break;
	default:
		break;
	}
}

c_Car* c_List::f_GetCurCar()
{
	return v_CurCar;
}

c_Colour* c_List::f_GetCurColour()
{
	return v_CurColour;
}

c_List::~c_List()
{

}


// +++++++++++++++++++++++++ GARAGE CLASS ++++++++++++++++++++++++

c_Garage::c_Garage()
{
}

void c_Garage::Init()
{

}

void c_Garage::Update(double dt)
{
	if (Application::IsKeyPressed('O') && v_CarList.f_GetCurCar()->f_GetCarNum() > 0)
	{
		v_CarList.f_ChangeCurrentCar('O');
	}
	if (Application::IsKeyPressed('P') && v_CarList.f_GetCurCar()->f_GetCarNum() < 2)
	{
		v_CarList.f_ChangeCurrentCar('P');
	}
	if (Application::IsKeyPressed('K') && v_ColourList.f_GetCurColour()->f_GetColourNum < 4) 
	{
		v_ColourList.f_ChangeCurrentColour('K');
	}
	if (Application::IsKeyPressed('L') && v_ColourList.f_GetCurColour()->f_GetColourNum > 0)
	{
		v_ColourList.f_ChangeCurrentColour('L');
	}
}

void c_Garage::Render()
{
	switch (v_CarList.f_GetCurCar()->f_GetCarNum())
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
}

void c_Garage::Exit()
{

}

c_Garage::~c_Garage()
{
}


