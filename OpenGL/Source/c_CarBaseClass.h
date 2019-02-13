#ifndef C_CARBASECLASS_H
#define C_CARBASECLASS_H


class c_CarBaseClass
{
public:
	c_CarBaseClass();
	~c_CarBaseClass();
	virtual void Movement(double dt) = 0;
	virtual float GetVelocityZ() = 0;


protected:
	float VelocityX;
	float VelocityZ;
	bool driving;
	bool backwards;
	float acceleration;
};


#endif

