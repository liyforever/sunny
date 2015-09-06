#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H
#include "SunnyConfig.h"
SUNNY_BEGIN
class Nocopyable
{
  Nocopyable(const Nocopyable&);
  Nocopyable& operator=(const Nocopyable&);
};

SUNNY_END
#endif // NOCOPYABLE_H
