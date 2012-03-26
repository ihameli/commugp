#include "log.h"
#include "types.h"

bool bestNeighbourOrder(const bestNeighbourPtr& bn1, const bestNeighbourPtr& bn2) { 
	if (bn1->num*bn2->den != bn2->num*bn1->den)
		return (bn1->num*bn2->den > bn2->num*bn1->den);
	else
		if (bn1->C1 != bn2->C1)
			return (bn1->C1 > bn2->C1);
		return (bn1->C2 > bn2->C2);
}

double max_from_2(double a, double b) {
	if (a>b)
		return a;
	return b;
}

double pi = 4.0 * atan(1.0);

Log logGrowthProcess("log/growth_process.log");

