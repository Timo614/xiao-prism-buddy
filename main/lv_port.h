#pragma once

#include "prism_driver.h"
extern LGFX lcd;

/**
 * @brief Initialize related work for lvgl.
 *
 */
void lv_port_init();

/**
 * @brief Take the semaphore.
 * @note  It should be called before manipulating lvgl gui.
 *
 */
void lv_port_sem_take();

/**
 * @brief Give the semaphore.
 * @note  It should be called after manipulating lvgl gui.
 *
 */
void lv_port_sem_give();
