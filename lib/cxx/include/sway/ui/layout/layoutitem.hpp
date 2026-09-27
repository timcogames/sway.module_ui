#ifndef SWAY_UI_LAYOUTITEM_HPP
#define SWAY_UI_LAYOUTITEM_HPP

#include <sway/render/updatable.hpp>
#include <sway/ui/_stdafx.hpp>
#include <sway/ui/element/_typedefs.hpp>
#include <sway/ui/element/element.hpp>
#include <sway/ui/layout/_typedefs.hpp>

namespace sway::ui {

class LayoutItem : public Element, public render::Updatable {
public:
#pragma region "Constructor(s) & Destructor"
  /** \~english @name Constructor(s) & Destructor */ /** \~russian @name Конструктор(ы) и Деструктор */
  /** @{ */

  LayoutItem();

  virtual ~LayoutItem();

  /** @} */
#pragma endregion

#pragma region "Overridden Updatable methods"

  virtual void update() {}

#pragma endregion

#pragma region "Overridden Element methods"

  void recursiveUpdateItemOffsets(const math::point2f_t offset) override;

#pragma endregion

  template <typename TYPE>
  void foreachItems(const TYPE &func) {
    for (auto i = 0; i < getNumOfChildNodes(); ++i) {
      auto node = getChildAt(i);
      if (!node.has_value()) {
        continue;
      }

      func(core::NodeUtil::cast<LayoutItem>(node.value()));
    }
  }

  MTHD_OVERRIDE(auto getBarrierType() const -> BarrierType) { return BarrierType::NONE; }

  auto getWeight() const -> f32_t { return weight_; }

  void setWeight(f32_t weight) { weight_ = weight; }

private:
  f32_t weight_{0.0F};
};

}  // namespace sway::ui

#endif  // SWAY_UI_LAYOUTITEM_HPP
