#ifndef SWAY_UI_WIDGET_PROGRESSMODES_HPP
#define SWAY_UI_WIDGET_PROGRESSMODES_HPP

#include <sway/core.hpp>

namespace sway::ui {

// clang-format off
#define PROGRESS_MODE_LIST(ITEM) \
  ITEM(FRACTION, 1) \
  ITEM(PERCENTAGE, 2)
// clang-format on

DECLARE_ENUM_U32(ProgressMode, PROGRESS_MODE_LIST)

}  // namespace sway::ui

#endif  // SWAY_UI_WIDGET_PROGRESSMODES_HPP
