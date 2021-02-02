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

#include "idle.h"
#include "timer.h"

uint32_t last_activity;
bool sleeping;

void idle_init(void) {
    last_activity = timer_read32();
#ifdef IDLE_AFTER
    sleeping = false;
#endif
}

void idle_task(void) {
#ifdef IDLE_AFTER
    if (!sleeping && idle_time() > IDLE_AFTER) {
        sleeping = true;
        idle_sleep_kb();
        idle_sleep_user();
    }
#endif
}

void idle_poke(void) {
    last_activity = timer_read32();
#ifdef IDLE_AFTER
    if (sleeping) {
        sleeping = false;
        idle_wake_kb();
        idle_wake_user();
    }
#endif
}

uint32_t idle_time(void) {
    return timer_elapsed32(last_activity);
}

#ifdef IDLE_AFTER
__attribute__ ((weak))
void idle_sleep_kb(void) {
}

__attribute__ ((weak))
void idle_sleep_user(void) {
}

__attribute__ ((weak))
void idle_wake_kb(void) {
}

__attribute__ ((weak))
void idle_wake_user(void) {
}
#endif
