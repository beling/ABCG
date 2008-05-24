#ifndef ENVIROMENT_H_
#define ENVIROMENT_H_

struct Enviroment
{
	const double entropy;
	
	const double gravity_force;
	
	Enviroment(const double entropy, const double gravity_force) : entropy(entropy), gravity_force(gravity_force) {};
};

#endif /*ENVIROMENT_H_*/
