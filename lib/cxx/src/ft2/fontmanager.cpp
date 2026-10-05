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
  std::fprintf(stderr, "[load] this=%p, name='%s'\n", (void *)this, name.c_str());
  std::fflush(stderr);

  if (!fetcherQueue) {
    return;
  }

  std::weak_ptr<FontManager> weakSelf = shared_from_this();

  auto loader = std::make_shared<FaceLoader>(filepath.c_str());
  loader->setCallback([weakSelf, fn = std::move(fn), name](rms::FetchResponse *resp) -> void {
    std::fprintf(stderr, "[load callback] ENTER for '%s'\n", name.c_str());
    std::fflush(stderr);

    auto self = weakSelf.lock();
    std::fprintf(stderr, "[callback] self=%p, cache_=%p, size=%zu\n", (void *)self.get(), (void *)&self->cache_,
        self->cache_.size());
    std::fflush(stderr);

    if (!self) {
      std::fprintf(stderr, "[load callback] weakSelf.lock() FAILED — FontManager destroyed\n");
      std::fflush(stderr);
      return;
    }

    std::fprintf(stderr, "[load callback] lock OK, serializing\n");
    std::fflush(stderr);

    auto *response = static_cast<ObjectFetchResponse *>(resp);
    if (!response) {
      std::fprintf(stderr, "[load callback] response is nullptr\n");
      std::fflush(stderr);
      return;
    }

    auto object = response->serialize(self->lib_);
    std::fprintf(stderr, "[load callback] serialized, object=%p\n", (void *)object.get());
    std::fflush(stderr);
    if (!object) {
      std::fprintf(stderr, "[load callback] object is nullptr\n");
      std::fflush(stderr);
      return;
    }

    // self->cache_.insert_or_assign(std::make_pair(name, std::move(object)));
    std::lock_guard<std::mutex> lock(self->cacheMutex_);
    self->cache_[name] = std::move(object);
    std::fprintf(stderr, "[load callback] cache_ updated, size=%zu\n", self->cache_.size());
    std::fflush(stderr);

    if (fn) {
      std::fprintf(stderr, "[load callback] calling fn()\n");
      std::fflush(stderr);
      fn();
    }
  });

  fetcherQueue->add(loader);
}

auto FontManager::addFont(const std::string &name, lpcstr_t symbols, int size, int marginSize) -> Font::SharedPtr_t {
  std::fprintf(stderr, "[addFont] this=%p, cache size=%zu, name='%s'\n", (void *)this, cache_.size(), name.c_str());
  std::fflush(stderr);

  std::fprintf(stderr, "[addFont] ENTER, cache size=%zu, name='%s'\n", cache_.size(), name.c_str());
  std::fflush(stderr);

  std::lock_guard<std::mutex> lock(cacheMutex_);
  auto iter = cache_.find(name);
  if (iter == cache_.end()) {
    std::fprintf(stderr, "[addFont] cache MISS for '%s'\n", name.c_str());
    std::fflush(stderr);
    return nullptr;
  }

  std::fprintf(stderr, "[addFont] cache HIT for '%s'\n", name.c_str());
  std::fflush(stderr);

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
