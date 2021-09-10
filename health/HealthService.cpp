/*
 * Copyright (C) 2018 The Android Open Source Project
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
#define LOG_TAG "android.hardware.health@2.0-service.sdm845"
#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <health2/Health.h>
#include <health2/service.h>
#include <healthd/healthd.h>
#include <hidl/HidlTransportSupport.h>

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "BatteryRechargingControl.h"

namespace {

using android::hardware::health::V2_0::DiskStats;
using android::hardware::health::V2_0::StorageAttribute;
using android::hardware::health::V2_0::StorageInfo;
using ::device::generic::sdm845::health::BatteryRechargingControl;

static BatteryRechargingControl battRechargingControl;

const std::string kDiskStatsFile{"/sys/block/sda/stat"};

std::ifstream assert_open(const std::string &path) {
    std::ifstream stream(path);
    if (!stream.is_open()) {
        LOG(FATAL) << "Cannot read " << path;
    }
    return stream;
}

template <typename T>
void read_value_from_file(const std::string &path, T *field) {
    auto stream = assert_open(path);
    stream.unsetf(std::ios_base::basefield);
    stream >> *field;
}

}  // anonymous namespace

void healthd_board_init(struct healthd_config *config) {
    using ::device::generic::sdm845::health::kChargerStatus;

    config->batteryStatusPath = kChargerStatus.c_str();
}

int healthd_board_battery_update(struct android::BatteryProperties *props) {
    battRechargingControl.updateBatteryProperties(props);
    return 0;
}

void get_storage_info(std::vector<StorageInfo> &vec_storage_info) {
    vec_storage_info.resize(1);
    StorageInfo *storage_info = &vec_storage_info[0];
    return;
}

void get_disk_stats(std::vector<DiskStats> &vec_stats) {
    vec_stats.resize(1);
    DiskStats *stats = &vec_stats[0];

    auto stream = assert_open(kDiskStatsFile);
    // Regular diskstats entries
    stream >> stats->reads >> stats->readMerges >> stats->readSectors >> stats->readTicks >>
        stats->writes >> stats->writeMerges >> stats->writeSectors >> stats->writeTicks >>
        stats->ioInFlight >> stats->ioTicks >> stats->ioInQueue;
    return;
}

int main(void) {
    return health_service_main();
}
