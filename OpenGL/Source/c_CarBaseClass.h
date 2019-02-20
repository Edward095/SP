#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H

#include "c_Entity.h"
#include "Application.h"

class c_CarBaseClass : public c_Entity
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	void updateAppearance(const char* meshPath, const char* TGApath);
	virtual void Movement(double dt);
	virtual float GetSteeringAngle();
	virtual void PadEffect(double dt);
	virtual float GetMaxAcceleration();
	virtual float GetSpeed();
	virtual float GetAcceleration();
	virtual void SetSpeed(float speed);
	virtual void SetFriction(float friction);
	virtual void SetSteering(float Steering);
	virtual void Ability(double dt) = 0;
	virtual void PowerUp(bool check) = 0;
	virtual void SetMaxSpeed(float Speed);


protected:
	float VelocityZ;
	float MaxSpeed;
	float SteeringAngle;
	float Acceleration;
	float MaxAcceleration;
	float Friction;
	float Steering;
	int Duration;


	bool Driving = false;
	bool Backwards = false;
	bool PressQ = false;
	bool Nitro = false;
	bool BoostPad = false;
	bool SlowPad = false;
	bool once;
	bool offRoad;
};


#endif

