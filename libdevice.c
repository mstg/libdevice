/**
* libdevice.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libdevice.h"

static bool device_connected;
static struct am_device *__tmp_device;

void _device_connect(struct am_device *__device) {
	AMDeviceConnect(__device);
	AMDeviceIsPaired(__device);
	AMDeviceValidatePairing(__device);
	AMDeviceStartSession(__device);
}

static void device_connected_callback(struct am_device_notification_callback_info *cb_info, int arg) {
	if (cb_info->msg == ADNCI_MSG_CONNECTED) {
		device_connected = true;
		__tmp_device = cb_info->dev;
	} else {
		device_connected = false;
	}
}

__device_t *_device_create_object() {
	return (__device_t*)malloc(sizeof(__device_t));
}

__device_t *connect_to_device(int *success) {
	struct __device_t *tmp_device = _device_create_object();
	device_connected = false;

	AMDeviceNotificationSubscribe(&device_connected_callback, 0, 0, 0, &tmp_device->device_notification);

	dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
		if (device_connected) {
			*success = 1;
			tmp_device->saved_udid = CFStringGetCStringPtr(AMDeviceCopyDeviceIdentifier(__tmp_device), 0);
			tmp_device->device = __tmp_device;
			_device_connect(tmp_device->device);
		} else {
			*success = 0;
		}
		CFRunLoopStop(CFRunLoopGetCurrent());
	});
	CFRunLoopRun();

	return tmp_device;
}

void device_free(__device_t *__device) {
	if (__device) {
		if (__device->device_notification) {
			AMDeviceNotificationUnsubscribe(__device->device_notification);
			AMDeviceRelease(__device->device);
		}
		free(__device);
	}
}

int same_device(__device_t *__device) {
	const char *current_udid = CFStringGetCStringPtr(AMDeviceCopyDeviceIdentifier(__device->device), 0);

	if (strcmp(__device->saved_udid, current_udid) == 0) {
		return 0;
	} else {
		return -1;
	}

	return 0;
}

int install_app_on_device(__device_t *__device, const char *local_path) {
	if (!local_path) {
		return NULL_ARG;
	}

	int __same = same_device(__device);
	if (__same != 0) {
		return -1;
	}

	CFStringRef _path_string = CFStringCreateWithCString(NULL, local_path, 0);
	CFURLRef _path_url = CFURLCreateWithFileSystemPath(NULL, _path_string, 0, true);
	const void *_opt_keys[] = {CFStringCreateWithCString(NULL, "PackageType", 0)};
	const void *_opt_values[] = {CFStringCreateWithCString(NULL, "Developer", 0)};

	CFDictionaryRef opt = CFDictionaryCreate(NULL, _opt_keys, _opt_values, (signed long)1, NULL, NULL);

	AMDeviceSecureTransferPath(0, __device->device, _path_url, opt, NULL, 0);

	int installed_ = AMDeviceSecureInstallApplication(0, __device->device, _path_url, opt, NULL, 0);

	CFRelease(opt);
	CFRelease(_path_string);
	CFRelease(_path_url);
	CFRelease(_opt_keys[0]);
	CFRelease(_opt_values[0]);

	return installed_;
}

int remove_app_from_device(__device_t *__device, const char *bundle_identifier) {
	if (!bundle_identifier) {
		return NULL_ARG;
	}

	int __same = same_device(__device);
	if (__same != 0) {
		return -1;
	}

	CFStringRef bundleid = CFStringCreateWithCString(NULL, bundle_identifier, 0);
	int uninstalled_ = AMDeviceSecureUninstallApplication(0, __device->device, bundleid, 0, NULL, 0);

	CFRelease(bundleid);

	return uninstalled_;
}

const void *get_device_property(__device_t *__device, const char *property) {
	CFStringRef prop_ref = CFStringCreateWithCString(NULL, property, 0);
	const void *prop = AMDeviceCopyValue(__device->device, 0, prop_ref);

	CFRelease(prop_ref);
	const char *ret_val = CFStringGetCStringPtr(prop, 0);
	CFRelease(prop);

	return ret_val;
}