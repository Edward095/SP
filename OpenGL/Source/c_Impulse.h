#ifndef C_IMPULSE_H
#define C_IMPULSE_H


class c_Impulse
{
public:
	c_Impulse();
	~c_Impulse();

	void CalcInitialMomentum(float SpeedA, float SpeedB);
	void CalcFinalMomentum(float SpeedA, float SpeedB);
	void CalcImpulse();
	void CalcIniKineticEnergy(float SpeedA, float SpeedB);
	void CalcFinKineticEnergy(float SpeedA, float SpeedB);
	float CalcSpeedAfterCollision(float SpeedA, float SpeedB);

private:

	float MomentumA;
	float MomentumB;
	float Initial_Momentum;
	float Final_Momentum;
	float Initial_KE;
	float Final_KE;
	float Impulse;
	float Time;
	float Force;
};


#endif
