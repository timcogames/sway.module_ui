#ifndef SWAY_UI_WIDGETS_AREATYPES_HPP
#define SWAY_UI_WIDGETS_AREATYPES_HPP

#include <sway/core.hpp>

NAMESPACE_BEGIN(sway)
NAMESPACE_BEGIN(ui)

enum class AreaType : i32_t { NONE = GLOB_IDX_INVALID, IDX_MRG = GLOB_IDX_INITIAL, IDX_BRD, IDX_PAD, IDX_CNT, Latest };

NAMESPACE_END(ui)
NAMESPACE_END(sway)

#define NUM_OF_AREAS (sway::core::detail::toBase(sway::ui::AreaType::Latest))

#endif  // SWAY_UI_WIDGETS_AREATYPES_HPP
