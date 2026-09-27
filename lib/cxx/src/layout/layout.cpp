#include <sway/ui/builder.hpp>
#include <sway/ui/layout/specs/linearlayout.hpp>

namespace sway::ui {

auto Layout::calculatesAutoCellSize(ElementPtr_t elem) -> math::size2f_t {
  return elem->getAreaHolder().getContentSize() / elem->getNumOfChildNodes();
}

Layout::Layout(BuilderPtr_t builder, Orientation orien)
    : LayoutItem()
    , Orientable(orien) {
  setMouseFilter(ois::MouseFilter::PASS);

  subscribe(this, "NodeAdded", EVENT_HANDLER(Layout, handleItemAdded));
  subscribe(this, "NodeRemoved", EVENT_HANDLER(Layout, handleItemRemoved));
}

auto Layout::handleItemAdded(const core::EventTypedefs::UniquePtr_t &evt) -> bool {
  auto *nodeEventData = static_cast<core::NodeEventData *>(evt->getData());

  setAdjacentChildOffsets();
  recursiveUpdateItemOffsets(math::point2f_zero);

  // auto prevElement = (ElementSharedPtr_t) nullptr;

  // auto prevNodeIndex = NodeChainExtension::getPrevItem(nodeEventData->nodeidx);
  // if (!prevNodeIndex.has_value()) {
  //   std::clog << "[UI Element::handleAddNode]: prevNodeIndex has no value" << std::endl;
  // } else {
  //   prevElement = NodeExtension::getChild<Element>(this, prevNodeIndex);
  // }

  // recursiveUpdate(prevElement, currElement);
  return true;
}

auto Layout::handleItemRemoved(const core::EventTypedefs::UniquePtr_t &evt) -> bool {
  auto *nodeEventData = static_cast<core::NodeEventData *>(evt->getData());
  Node::getChild<LayoutItem>(this, nodeEventData->nodeidx).reset();
  std::cout << "Layout::handleItemRemoved" << std::endl;
  return true;
}

}  // namespace sway::ui
