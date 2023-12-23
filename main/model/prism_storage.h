#ifndef STORAGE_H
#define STORAGE_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

int prism_storage_init(void);
esp_err_t prism_storage_write(char *p_key, void *p_data, size_t len);
esp_err_t prism_storage_read(char *p_key, void *p_data, size_t *p_len);

#ifdef __cplusplus
}
#endif

#endif