#include <sway/ui/ft2/faceloader.hpp>

namespace sway::ui {

FaceLoader::FaceLoader(const std::string &url)
    : rms::Fetcher(url) {}

void FaceLoader::fetch() {
#if EMSCRIPTEN_PLATFORM
  // thread_ = std::thread([this]() -> void {
  auto callback = [this](const u8_t *data, u32_t numBytes) {
    std::fprintf(stderr, "[FaceLoader] callback ENTER, data=%p, numBytes=%u\n", (void *)data, numBytes);
    std::fflush(stderr);

    response_ = std::make_unique<ObjectFetchResponse>(data, numBytes);

    // std::fprintf(stderr, "[FaceLoader] response_=%p, numBytes=%u\n", (void *)response_, fetch->numBytes);
    // std::fflush(stderr);

    fetching_.store(false);

    // std::fprintf(stderr, "[FaceLoader] response_=%p, numBytes=%u\n", (void *)response_, fetch->numBytes);
    // std::fflush(stderr);
  };

  rms::RemoteFile::fetch(getUrl().c_str(), callback);
  // });
#endif
}

FaceLoader::~FaceLoader() {
  // if (thread_.joinable()) {
  //   thread_.join();
  // }
}

}  // namespace sway::ui
