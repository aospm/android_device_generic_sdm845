#include <memory>

#include <health/utils.h>
#include <health2impl/Health.h>
#include <hidl/Status.h>
#include <log/log.h>
#include <android-base/strings.h>
#include <string>
#include <fstream>

using ::android::sp;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::health::InitHealthdConfig;
using ::android::hardware::health::V2_1::IHealth;
using ::android::hidl::base::V1_0::IBase;

using namespace std::literals;

namespace android {
namespace hardware {
namespace health {
namespace V2_1 {
namespace implementation {

// android::hardware::health::V2_1::implementation::Health implements most
// defaults. Uncomment functions that you need to override.
class HealthImpl : public Health {
  public:
    HealthImpl(std::unique_ptr<healthd_config>&& config)
        : Health(std::move(config)) {}
};

}  // namespace implementation
}  // namespace V2_1
}  // namespace health
}  // namespace hardware
}  // namespace android

extern "C" IHealth* HIDL_FETCH_IHealth(const char* instance) {
    using ::android::hardware::health::V2_1::implementation::HealthImpl;

    static const std::string POWERSUPPLY_DIR = "/sys/class/power_supply";

    if (instance != "default"sv) {
        ALOGE("Instance is not supported");
        return nullptr;
    }
    auto config = std::make_unique<healthd_config>();
    InitHealthdConfig(config.get());

    if (auto supplies = opendir(POWERSUPPLY_DIR.c_str())) {
        while (dirent *ent = readdir(supplies)) {
            if ((ent->d_type == DT_DIR && ent->d_name[0] != '.') || ent->d_type == DT_LNK) {
                std::string supplyPath = POWERSUPPLY_DIR + "/" + ent->d_name;
                ALOGI("Found %s", supplyPath.c_str());
                if (auto stream = std::ifstream(supplyPath + "/status")) {
                    if (stream.good()) {
                        config->batteryStatusPath = supplyPath.append("/status").c_str();
                        break;
                    }
                }
            }
        }
        closedir(supplies);
        ALOGI("Using %s", config->batteryStatusPath.c_str());
    } else {
        ALOGE("Failed to open %s", POWERSUPPLY_DIR.c_str());
    }

    return new HealthImpl(std::move(config));
}
