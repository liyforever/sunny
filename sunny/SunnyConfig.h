#ifndef SUNNYCONFIG_H
#define SUNNYCONFIG_H
#define SUNNY_BEGIN namespace sunny {
#define SUNNY_END   }

#define SUNNY_DISABLE_COPY(Class) \
    Class(const Class &);\
    Class &operator=(const Class &);
#endif // SUNNYCONFIG_H
