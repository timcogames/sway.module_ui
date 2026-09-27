#ifndef SWAY_UI_FT2_GLYPH_HPP
#define SWAY_UI_FT2_GLYPH_HPP

#include <sway/ui/_stdafx.hpp>
#include <sway/ui/ft2/errormacros.hpp>
#include <sway/ui/ft2/face.hpp>

namespace sway::ui {

/**
 * @ingroup ft2
 * @{
 */

struct FontGlyphId {
  u32_t idx;
  u32_t code;

  FontGlyphId(FT_Face face, u32_t charcode)
      : idx(FT_Get_Char_Index(face, charcode))
      , code(charcode) {}
};

class FontGlyph {
public:
#pragma region "Constructor(s) & Destructor"
  /** \~english @name Constructor(s) & Destructor */ /** \~russian @name Конструктор(ы) и Деструктор */
  /** @{ */

  FontGlyph(std::shared_ptr<Face> face)
      : face_(face) {}

  ~FontGlyph() = default;

  /** @} */
#pragma endregion

  static auto load(FT_Face face, FontGlyphId sym) -> std::optional<FT_GlyphSlot> {
    bool success = CHECK_RESULT(FT_Load_Glyph(face, sym.idx, FT_LOAD_DEFAULT));
    if (!success) {
      return std::nullopt;
    }

    return std::make_optional<FT_GlyphSlot>(face->glyph);
  }

private:
  std::shared_ptr<Face> face_;
};

/** @} */  // ingroup ft2

}  // namespace sway::ui

#endif  // SWAY_UI_FT2_GLYPH_HPP
