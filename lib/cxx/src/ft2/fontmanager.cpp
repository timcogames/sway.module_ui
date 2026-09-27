#include <sway/ui/ft2/faceloader.hpp>
#include <sway/ui/ft2/fontmanager.hpp>

namespace sway::ui {

FontManager::FontManager()
    : lib_(nullptr)
    , initialized_(false) {
  initLibrary();
}

FontManager::~FontManager() { freeLibrary(); }

void FontManager::initLibrary() {
  if (initialized_) {
    return;
  }

  if (FT_Init_FreeType(&lib_) != FT_Err_Ok) {
    return;
  }

  initialized_ = true;
}

void FontManager::freeLibrary() {
  if (lib_ != nullptr) {
    FT_Done_FreeType(lib_);
    lib_ = nullptr;
  }

  initialized_ = false;
}

void FontManager::load(std::function<void()> fn, std::shared_ptr<rms::FetcherQueue> fetcherQueue,
    const std::string &name, const std::string &filepath) {
  if (!fetcherQueue) {
    return;
  }

  std::weak_ptr<FontManager> weakSelf = shared_from_this();

  auto loader = std::make_shared<FaceLoader>(filepath.c_str());
  loader->setCallback([weakSelf, fn = std::move(fn), name](rms::FetchResponse *resp) -> void {
    auto self = weakSelf.lock();
    if (!self) {
      return;
    }

    auto *response = static_cast<ObjectFetchResponse *>(resp);
    if (!response) {
      return;
    }

    auto object = response->serialize(self->lib_);
    if (!object) {
      return;
    }

    // self->cache_.insert_or_assign(std::make_pair(name, std::move(object)));
    self->cache_[name] = std::move(object);

    if (fn) {
      fn();
    }
  });

  fetcherQueue->add(loader);
}

auto FontManager::addFont(const std::string &name, lpcstr_t symbols, int size, int marginSize) -> Font::SharedPtr_t {
  auto iter = cache_.find(name);
  if (iter == cache_.end()) {
    return nullptr;
  }

  auto texAtlasSize = math::size2i_t(size, size);
  auto texAtlasMarginSize = math::size2i_t(marginSize, marginSize);

  auto font = std::make_shared<Font>(iter->second, texAtlasSize, texAtlasMarginSize);
  font->setHeight(32);
  font->create(symbols, false, true);

  fonts_.insert(std::make_pair(name, font));
  return font;
}

auto FontManager::find(const std::string &name) -> Font::SharedPtr_t {
  auto iter = fonts_.find(name);
  if (iter != fonts_.end()) {
    return iter->second;
  }

  return nullptr;
}

void FontManager::removeFont() { fonts_.clear(); }

}  // namespace sway::ui
