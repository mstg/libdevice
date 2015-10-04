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

#define BasebandStatus "BasebandStatus"
#define BluetoothAddress "BluetoothAddress"
#define DeviceName "DeviceName"
#define FirmwareVersion "FirmwareVersion"
#define IntegratedCircuitCardIdentity "IntegratedCircuitCardIdentity"
#define InternationalMobileEquipmentIdentity "InternationalMobileEquipmentIdentity"
#define MLBSerialNumber "MLBSerialNumber"
#define PhoneNumber "PhoneNumber"
#define ProductType "ProductType"
#define SerialNumber "SerialNumber"
#define SIMStatus "SIMStatus"
#define TimeZone "TimeZone"
#define UniqueDeviceID "UniqueDeviceID"
#define WiFiAddress "WiFiAddress"

typedef struct __device_t {
	struct am_device *device;
	struct am_device_notification *device_notification;
	const char *saved_udid;
} __device_t;

const int NULL_ARG = -1;
__device_t *connect_to_device();
void device_free(__device_t *__device);
int install_app_on_device(__device_t *__device, const char *local_path);
int remove_app_from_device(__device_t *__device, const char *bundle_identifier);
const void *get_device_property(__device_t *__device, const char *property);

#endif