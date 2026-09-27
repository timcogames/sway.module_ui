#ifndef SWAY_UI_LAYOUT_HPP
#define SWAY_UI_LAYOUT_HPP

#include <sway/ui/_stdafx.hpp>
#include <sway/ui/layout/_typedefs.hpp>
#include <sway/ui/orientable.hpp>
#include <sway/ui/orientations.hpp>
#include <sway/ui/widget/widget.hpp>

namespace sway::ui {

class Layout : public LayoutItem, public Orientable {
public:
#pragma region "Static methods"

  /**
   * \~russian
   * @brief Вычисляет размер ячейки макета.
   * @param[in] elem Указатель на элемент макета.
   */
  static auto calculatesAutoCellSize(ElementPtr_t elem) -> math::size2f_t;

#pragma endregion

#pragma region "Constructor(s) & Destructor"
  /** \~english @name Constructor(s) & Destructor */ /** \~russian @name Конструктор(ы) и Деструктор */
  /** @{ */

  Layout(BuilderPtr_t builder, Orientation orien);

  virtual ~Layout() = default;

  /** @} */
#pragma endregion

  MTHD_OVERRIDE(auto getBarrierType() const -> BarrierType) { return BarrierType::LAYOUT; }

  virtual void setAdjacentChildOffsets() = 0;

  auto handleItemAdded(const core::EventTypedefs::UniquePtr_t &evt) -> bool;

  auto handleItemRemoved(const core::EventTypedefs::UniquePtr_t &evt) -> bool;

  /**
   * \~english
   * @brief Requests one of the child elements of this view to measure itself.
   * @param[in] child Child element to measure.
   *
   * \~russian
   * @brief Запрашивает одного из дочерних элементов этого представления измерить себя.
   * @param[in] child Дочерний элемент для измерения.
   */
  void measureChild(ElementSharedPtr_t child) {}
};

}  // namespace sway::ui

#endif  // SWAY_UI_LAYOUT_HPP
