#ifndef CRYPTOCURRENCY_H
#define CRYPTOCURRENCY_H

#include "config.h"
#include "view_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct view_cryptocurrency_data* get_cryptocurrency_data();
int prism_cryptocurrency_init(void);

#ifdef __cplusplus
}
#endif

#endif