#ifndef SWAY_UI_ELEMENT_EVENTHUBS_INPUTEVEN_HPP
#define SWAY_UI_ELEMENT_EVENTHUBS_INPUTEVEN_HPP

#include <sway/core.hpp>
#include <sway/core/events/v2/event.hpp>
#include <sway/ui/element/event/_typedefs.hpp>

namespace sway::ui {

class InputEvent : public core::v2::Event {
protected:
  explicit InputEvent(u32_t type, const core::v2::EventContext &context, u32_t modifiers)
      : core::v2::Event(context, /*core::v2::EventData()*/ NULL)
      , modifiers_(modifiers) {}

public:
  virtual ~InputEvent() = default;

  [[nodiscard]] auto getModifiers() const -> u32_t { return modifiers_; }

private:
  u32_t modifiers_;
};

}  // namespace sway::ui

#endif  // SWAY_UI_ELEMENT_EVENTHUBS_INPUTEVEN_HPP
