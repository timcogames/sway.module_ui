#ifndef SWAY_UI_ELEMENT_HPP
#define SWAY_UI_ELEMENT_HPP

#include <sway/core/container/nodeindexchain.hpp>
#include <sway/ui/_stdafx.hpp>
#include <sway/ui/area/areaholder.hpp>
#include <sway/ui/barriertypes.hpp>
#include <sway/ui/element/_typedefs.hpp>
#include <sway/ui/element/elementoffset.hpp>
#include <sway/ui/element/elementpositions.hpp>
#include <sway/ui/transform/sizepolicy.hpp>

namespace sway::ui {

class Measurable {
public:
  virtual ~Measurable() = default;

  void setSizePolicy(DimensionType dim, SizePolicyType policy) { sizePolicy_.setResizePolicy(dim, policy); }

  auto getSizePolicy() const -> SizePolicy { return sizePolicy_; }

  void setSizeDimensions(const math::size2f_t &size) {
    std::get<0>(sizePolicy_.dimensions[0].value) = size.getW();
    std::get<0>(sizePolicy_.dimensions[1].value) = size.getH();
  }

  auto getDimensions() const -> math::size2f_t {
    return math::size2f_t(std::get<0>(sizePolicy_.dimensions[0].value), std::get<0>(sizePolicy_.dimensions[1].value));
  }

private:
  SizePolicy sizePolicy_;
};

/** @defgroup views Views */

/**
 * @ingroup views
 * @brief Fundamental UI building block
 * @verbatim
    * [Size & Position](#size--position)
  @endverbatim
 */
class Element : public Measurable, public core::Node, public core::Visibleable {
  DECLARE_EVENT(EVT_POINTER_ENTER, PointerEnter)
  DECLARE_EVENT(EVT_POINTER_LEAVE, PointerLeave)
  DECLARE_EVENT(EVT_MOUSE_CLICKED, MouseClicked)

public:
#pragma region "Constructor(s) & Destructor"
  /** \~english @name Constructor(s) & Destructor */ /** \~russian @name Конструктор(ы) и Деструктор */
  /** @{ */

  Element();

  virtual ~Element();

  /** @} */
#pragma endregion  // region "Constructor(s) & Destructor"

#pragma region "Pure virtual methods"

  virtual void recursiveUpdateItemOffsets(const math::point2f_t offset) = 0;

  virtual auto getBarrierType() const -> BarrierType = 0;

#pragma endregion  // region "Pure virtual methods"

#pragma region "Size & Position"
  /** \~english @name Size & Position */ /** \~russian @name Размер и Позиция */
  /** @{ */

#pragma region "Getters/Setters Position"

  void setPosition(ElementPosition pos);

  [[nodiscard]] auto getPosition() const -> ElementPosition;

#pragma endregion  // region "Getters/Setters Position"

#pragma region "Getters/Setters Offset"

  /**
   * \~english
   * @brief Sets the original position.
   * @param[in] pnt New original position.
   *
   * \~russian
   * @brief Устанавливает оригинальное положение.
   * @param[in] pnt Новое оригинальное положение.
   *
   * \~
   * @sa setOffset(f32_t, f32_t)
   */
  void setOffset(const math::point2f_t &pnt);

  /**
   * \~english
   * @brief Sets the original position.
   * @param[in] x Value of X coordinate.
   * @param[in] y Value of Y coordinate.
   *
   * \~russian
   * @brief Устанавливает оригинальное положение.
   * @param[in] x Значение координаты по оси X.
   * @param[in] y Значение координаты по оси Y.
   *
   * \~
   * @sa setOffset(const math::point2f_t &)
   */
  void setOffset(f32_t x, f32_t y);

  auto getOffset() -> ElementOffset &;

#pragma endregion  // region "Getters/Setters Offset"

#pragma region "Getters/Setters Size"

  void setSize(const math::size2f_t &size);

  void setSize(f32_t wdt, f32_t hgt);

  [[nodiscard]] auto getSize() const -> math::size2f_t;

#pragma endregion  // region "Getters/Setters Size"

  /** @} */
#pragma endregion  // region "Size & Position"

  void updateOffset();

  [[nodiscard]] auto getAreaHolder() const -> AreaHolder;

  [[nodiscard]] auto getInnerSize() const -> math::size2f_t;

  [[nodiscard]] auto getOuterSize() const -> math::size2f_t;

  [[nodiscard]] auto getOuterSizeWithMargin() const -> math::size2f_t;

  auto getChildAtPoint(const math::point2f_t &point) -> ElementPtr_t;

  void setEventFilter(core::EventHandlerTypedefs::Ptr_t handler) { eventFilter_ = handler; }

  void setMouseFilter(ois::MouseFilter filter) { mouseFilter_ = filter; }

  [[nodiscard]] auto getMouseFilter() const -> ois::MouseFilter { return mouseFilter_; }

  void onCursorPointerEnter();

  void onCursorPointerLeave();

  void onMouseClick(u32_t state);

protected:
  math::Alignment alignment_;

private:
  /** @cond INTERNAL */

  AreaHolder holder_;

  ElementPosition position_;
  ElementOffset offset_;

  core::EventHandlerTypedefs::Ptr_t eventFilter_;
  ois::MouseFilter mouseFilter_;
  bool containsPointer_;

  /** @endcond */
};

}  // namespace sway::ui

#endif  // SWAY_UI_ELEMENT_HPP
