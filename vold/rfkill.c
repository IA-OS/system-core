/*
 * Copyright (C) 2009 The Android-x86 Open Source Project
 *
 * Author: Chih-Wei Huang <cwhuang@linux.org.tw>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "vold.h"

#include <dirent.h>
#include <errno.h>

#define DEBUG_RFKILL_BOOTSTRAP 0

#define SYSFS_CLASS_RFKILL_PATH "/sys/class/rfkill"

/*
 * Bootstrap rfkill interfaces
 */
int rfkill_bootstrap()
{
    DIR *d;
    struct dirent *de;

    if (!(d = opendir(SYSFS_CLASS_RFKILL_PATH))) {
        LOG_ERROR("Unable to open '%s' (%s)", SYSFS_CLASS_RFKILL_PATH,
                strerror(errno));
        return -errno;
    }

    while ((de = readdir(d)))
        if (de->d_name[0] != '.') {
            char path[SYSFS_PATH_MAX];
            sprintf(path, "%s/%s", SYSFS_CLASS_RFKILL_PATH, de->d_name);
#if DEBUG_RFKILL_BOOTSTRAP
            LOG_VOL("Simulate add: %s", de->d_name);
#endif
            if (simulate_add_device("rfkill", path + 4)) {
                LOG_ERROR("Simulate add device %s error: %s", path,
                        strerror(errno));
            }
        }

    closedir(d);
    return 0;
}
