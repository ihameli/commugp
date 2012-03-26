#ifndef _MUTUAL_INFORMATION_H_
#define _MUTUAL_INFORMATION_H_

#include <list>
#include <map>

double mutual_information(std::map<int, std::list<int> > partition_1, std::map<int, std::list<int> > partition_2);

#endif

