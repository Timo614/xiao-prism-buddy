#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "cJSON.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

void parseJsonDouble(const char *tag, cJSON *parent, const char *key, double *output);

void parseJsonInt(const char *tag, cJSON *parent, const char *key, int *output);

void parseJsonBool(const char *tag, cJSON *parent, const char *key, bool *output);

#ifdef __cplusplus
}
#endif
#endif