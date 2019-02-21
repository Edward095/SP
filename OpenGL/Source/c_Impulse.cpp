#include "c_Impulse.h"
#include "MyMath.h"


c_Impulse::c_Impulse()
{
	MomentumA = 0;
	MomentumB = 0;
	Final_Momentum = 0;
	Initial_Momentum = 0;
	Impulse = 0;
	Time = 3;
	Force = 0;
}

c_Impulse::~c_Impulse()
{

}

float c_Impulse::CalcSpeedAfterCollision(float SpeedA, float SpeedB)
{
	Initial_KE = (1 / 2)(SpeedA * SpeedA)(7 / 10) + (1 / 2)(SpeedB * SpeedB)(13 / 10);
	SpeedB = sqrtf((Initial_KE - ((7.f / 20.f) * (SpeedA * SpeedA))) * (20.f / 13.f));
		
	return SpeedB;
}

void c_Impulse::CalcInitialMomentum(float SpeedA, float SpeedB)
{
	MomentumA = SpeedA;
	MomentumB = SpeedB;
	Initial_Momentum = MomentumA + MomentumB;
}

void c_Impulse::CalcFinalMomentum(float SpeedA, float SpeedB)
{
	MomentumA = SpeedA;
	MomentumB = SpeedB;
	Final_Momentum = MomentumA + MomentumB;
}

void c_Impulse::CalcImpulse()
{
	Impulse = Final_Momentum - Initial_Momentum;
}




//Momentum, mass x velocity
//Impulse = Final Momentum - Initial Momentum 
//impulse = force x time 
//find the time that affects the car,
//force, f = ma then find acceleration, since m = 1, just dont care about mass

//Ke after collision = 100
//