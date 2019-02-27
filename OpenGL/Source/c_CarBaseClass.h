#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H

#include "c_Entity.h"
#include "Application.h"
#include "c_Sound.h"
class c_CarBaseClass : public c_Entity
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	void updateAppearance(const char* meshPath, const char* TGApath);
	virtual void Movement(double dt);
	virtual void PadEffect(double dt);
	bool onCooldown();
	float GetSteeringAngle();
	float GetMaxAcceleration();
	float GetSpeed();
	float GetAcceleration();
	void SetSpeed(float speed);
	void SetFriction(float friction);
	void SetSteering(float Steering);
	void SetMaxSpeed(float Speed);
	void SetSteeringAngle(float angle);
	void PSpeed(float speed);
	float GetSpedoSpeed();
	bool getStopTime();
	void setSlowed(bool slowedorNot);
	virtual void resetVar() = 0;
	virtual void Ability(double dt) = 0;
	virtual void isOffRoad() = 0;

protected:
	float VelocityZ;
	float MaxSpeed;
	float SteeringAngle;
	float Acceleration;
	float MaxAcceleration;
	float Friction;
	float Steering;
	float elapsedTime;
	float coolDown;
	float abilityDuration;
	float SpedoVeloZ;
	int padDuration;

	bool Driving;
	bool Backwards;
	bool PressQ;
	bool BoostPad;
	bool SlowPad;

	bool offRoad;
	bool Collided;

	bool stopTime;
	bool slowed;
};


#endif

