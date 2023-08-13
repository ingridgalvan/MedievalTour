////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _STADISTICSCLASS_H_
#define _STADISTICSCLASS_H_

class stadistics
{
public:
	stadistics();
	stadistics(const stadistics&);
	~stadistics();
	
private:
	float Deltalife, Bosslife, Mylife, Torretlife, BosslifeSPhase, MylifeLimit;

};

#endif

