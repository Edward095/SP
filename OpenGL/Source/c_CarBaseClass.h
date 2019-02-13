#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H


class c_CarBaseClass
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	virtual void Movement(double dt) = 0;
	virtual float GetPosX() = 0;
	virtual float GetPosY() = 0;
	virtual float GetPosZ() = 0;

protected:
	float VelocityZ;
	float NewPosX;
	float NewPosY;
	float NewPosZ;
	float SteeringAngle;
	float Acceleration;

	bool Driving;
	bool Backwards;

	
};


#endif

