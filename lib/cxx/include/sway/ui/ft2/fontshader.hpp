#ifndef SWAY_UI_FT2_FONTSHADER_HPP
#define SWAY_UI_FT2_FONTSHADER_HPP

#include <sway/ui/_stdafx.hpp>
#include <sway/ui/typography.hpp>

namespace sway::ui {

/**
 * @ingroup ft2
 * @{
 */

class FontShader {
public:
  using Ptr_t = FontShader *;
  using SharedPtr_t = std::shared_ptr<FontShader>;

#pragma region "Constructor(s) & Destructor"
  /** \~english @name Constructor(s) & Destructor */ /** \~russian @name Конструктор(ы) и Деструктор */
  /** @{ */

  FontShader() {}

  ~FontShader() = default;

  /** @} */
#pragma endregion

  static void setColor(render::EffectTypedefs::Ptr_t effect, const math::col4f_t &col) {
    if (!effect) {
      return;
    }

    auto shaderProgram = effect->getShaderProgram();
    if (!shaderProgram) {
      return;
    }

    shaderProgram->setUniformCol4f("text_color", col);
  }

  static void setLayer(render::EffectTypedefs::Ptr_t effect, i32_t zindex) {
    if (!effect) {
      return;
    }

    auto shaderProgram = effect->getShaderProgram();
    if (!shaderProgram) {
      return;
    }

    shaderProgram->setUniform1f("zindex", zindex == 0 ? -0.999F : (f32_t)(zindex - 100) / 100);
  }

private:
};

/** @} */  // ingroup ft2

}  // namespace sway::ui

#endif  // SWAY_UI_FT2_FONTSHADER_HPP
