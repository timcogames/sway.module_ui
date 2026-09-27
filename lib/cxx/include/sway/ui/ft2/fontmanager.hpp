#ifndef SWAY_UI_FT2_FONTMANAGER_HPP
#define SWAY_UI_FT2_FONTMANAGER_HPP

#include <sway/ui/_stdafx.hpp>
#include <sway/ui/ft2/face.hpp>
#include <sway/ui/ft2/font.hpp>

namespace sway::ui {

/**
 * @ingroup ft2
 * @{
 */

class FontManager : public std::enable_shared_from_this<FontManager> {
public:
  using Ptr_t = FontManager *;
  using SharedPtr_t = std::shared_ptr<FontManager>;

#pragma region "Constructor(s) & Destructor"
  /** \~english @name Constructor(s) & Destructor */ /** \~russian @name Конструктор(ы) и Деструктор */
  /** @{ */

  FontManager();

  FontManager(const FontManager &) = delete;

  ~FontManager();

  /** @} */
#pragma endregion

  void initLibrary();

  void freeLibrary();

  void load(std::function<void()> fn, std::shared_ptr<rms::FetcherQueue> fetcherQueue, const std::string &name,
      const std::string &filepath);

  auto addFont(const std::string &name, lpcstr_t symbols, int size, int marginSize) -> Font::SharedPtr_t;

  auto find(const std::string &name) -> Font::SharedPtr_t;

  void removeFont();

  FontManager &operator=(const FontManager &) = delete;

private:
  FT_Library lib_;
  bool initialized_;

  std::unordered_map<std::string, std::shared_ptr<Face>> cache_;
  std::unordered_map<std::string, Font::SharedPtr_t> fonts_;
};

/** @} */  // ingroup ft2

}  // namespace sway::ui

#endif  // SWAY_UI_FT2_FONTMANAGER_HPP
