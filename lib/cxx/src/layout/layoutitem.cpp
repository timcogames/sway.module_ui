#include <sway/core/container/nodeutil.hpp>
#include <sway/ui/layout/layoutitem.hpp>

namespace sway::ui {

LayoutItem::LayoutItem() {}

LayoutItem::~LayoutItem() { std::cout << "~LayoutItem" << std::endl; }

void LayoutItem::recursiveUpdateItemOffsets(const math::point2f_t parentOffset) {
  auto elementOffset = math::point2f_t(
      parentOffset.getX() + getOffset().original.getX(), parentOffset.getY() + getOffset().original.getY());

  foreachItems([&](LayoutItemSharedPtr_t item) {
    item->getOffset().computed = math::point2f_t(
        elementOffset.getX() /* + item->getMargin().getX() */, elementOffset.getY() /* + item->getMargin().getY() */);
    item->updateOffset();
    item->recursiveUpdateItemOffsets(parentOffset);
  });
}

}  // namespace sway::ui
