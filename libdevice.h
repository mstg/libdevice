/**
* libdevice.h
* Copyright (C) 2015 Mustafa Gezen
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _LIBDEVICE_H_
#define _LIBDEVICE_H_

#include "MobileDevice.h"

typedef struct __device_t {
	struct am_device *device;
	struct am_device_notification *device_notification;
	const char *saved_udid;
} __device_t;

const int NULL_PATH = -1;
int install_app_on_device(__device_t *__device, const char *local_path);
__device_t *connect_to_device();
void device_free(__device_t *__device);

#endif