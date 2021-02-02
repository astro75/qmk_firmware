/* Copyright 2021 L. K. Post
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

void idle_init(void);
void idle_task(void);
void idle_poke(void);

uint32_t idle_time(void);

#ifdef IDLE_AFTER
void idle_sleep_kb(void);
void idle_sleep_user(void);
void idle_wake_kb(void);
void idle_wake_user(void);
#endif
