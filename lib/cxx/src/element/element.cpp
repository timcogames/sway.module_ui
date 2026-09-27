#include <sway/ui/element/element.hpp>
#include <sway/ui/widget/widgetevent.hpp>

namespace sway::ui {

Element::Element()
    : mouseFilter_(ois::MouseFilter::STOP)
    , position_(ElementPosition::RELATIVE)
    , containsPointer_(false)
    , eventFilter_(nullptr)
    , alignment_(math::Alignment::LEFT_TOP) {}

Element::~Element() { std::cout << "~Element" << std::endl; }

void Element::setPosition(ElementPosition pos) { position_ = pos; }

auto Element::getPosition() const -> ElementPosition { return position_; }

void Element::setSize(const math::size2f_t &size) {
  this->getAreaHolder().getArea<AreaType::IDX_CNT>().value()->setSize(size);
}

void Element::setSize(f32_t wdt, f32_t hgt) { setSize(math::size2f_t(wdt, hgt)); }

auto Element::getSize() const -> math::size2f_t {
  return this->getAreaHolder().getArea<AreaType::IDX_CNT>().value()->getSize();
}

void Element::setOffset(const math::point2f_t &pnt) {
  offset_.original = pnt;
  offset_.markAsDirty();
}

void Element::setOffset(f32_t x, f32_t y) { setOffset(math::point2f_t(x, y)); }

auto Element::getOffset() -> ElementOffset & { return offset_; }

void Element::updateOffset() {
  if (!offset_.dirty) {
    return;
  }

  if (position_ == ElementPosition::RELATIVE) {
    auto parentOpt = this->getParentNode();
    if (!parentOpt.has_value()) {
      offset_.computed = offset_.original;
    } else {
      auto parent = core::NodeUtil::cast<Element>(parentOpt);
      auto parentAreaHolder = parent->getAreaHolder();
      auto parentAreaPosition = parentAreaHolder.getPosition<ui::AreaType::IDX_CNT>();
      auto parentContentSize = parentAreaHolder.getContentSize();

      auto x = 0.0F, y = 0.0F;
      auto alignmentBase = core::toBase<math::Alignment>(alignment_);
      auto childContentSize = this->getAreaHolder().getContentSize();

      if (alignmentBase & math::ConvFromXAlign<math::HorzAlign::CENTER>()) {
        x = ((parentContentSize.getW() / parent->getNumOfChildNodes()) - childContentSize.getW()) / 2;
      } else if (alignmentBase & math::ConvFromXAlign<math::HorzAlign::RIGHT>()) {
        x = ((parentContentSize.getW() / parent->getNumOfChildNodes()) - childContentSize.getW());
      }

      if (alignmentBase & math::ConvFromXAlign<math::VertAlign::CENTER>()) {
        y = ((parentContentSize.getH() / parent->getNumOfChildNodes()) - childContentSize.getH()) / 2;
      } else if (alignmentBase & math::ConvFromXAlign<math::VertAlign::BOTTOM>()) {
        y = ((parentContentSize.getH() / parent->getNumOfChildNodes()) - childContentSize.getH());
      }

      offset_.computed = offset_.original;
      offset_.computed = math::point2f_t(offset_.computed.getX() + x, offset_.computed.getY() + y);
    }
  } else if (position_ == ElementPosition::ABSOLUTE || position_ == ElementPosition::FIXED) {
    // auto parentInnerSize = parent->getInnerSize();
    offset_.computed = offset_.original;
  } else {
    offset_.computed = offset_.original;
  }

  offset_.dirty = false;
}

auto Element::getAreaHolder() const -> AreaHolder { return holder_; }

auto Element::getInnerSize() const -> math::size2f_t { return holder_.getSize<AreaType::IDX_PAD>(); }

auto Element::getOuterSize() const -> math::size2f_t { return holder_.getSize<AreaType::IDX_BRD>(); }

auto Element::getOuterSizeWithMargin() const -> math::size2f_t { return holder_.getSize<AreaType::IDX_MRG>(); }

auto Element::getChildAtPoint(const math::point2f_t &pnt) -> ElementPtr_t {
  for (auto node : this->getChildNodes()) {
    auto child = std::static_pointer_cast<Element>(node);
    if (!child->isVisible()) {
      break;
    }

    const auto childPos = child->getOffset().computed;
    auto childRect = math::rect4f_t(childPos.getX(), childPos.getY(), child->getOuterSize());

    // std::cout << childRect << " / " << pnt << std::endl;

    if (auto *const element = child->getChildAtPoint(pnt)) {
      return element;
    } else if (childRect.contains(pnt) && child->getMouseFilter() != ois::MouseFilter::IGNORE) {
      return child.get();
    }
  }

  return nullptr;
}

void Element::onCursorPointerEnter() {
  containsPointer_ = true;
  // this->update();

  auto *evtdata = new PointerEnterEventData();

  emit(EVT_POINTER_ENTER, std::make_unique<PointerEnterEvent>(0, evtdata),
      [&](core::EventHandlerTypedefs::Ptr_t) { return true; });
}

void Element::onCursorPointerLeave() {
  containsPointer_ = false;
  // this->update();

  auto *evtdata = new PointerLeaveEventData();
  emit(EVT_POINTER_LEAVE, std::make_unique<PointerLeaveEvent>(0, evtdata),
      [&](core::EventHandlerTypedefs::Ptr_t) { return true; });
}

void Element::onMouseClick(u32_t state) {
  // this->update();

  if (eventFilter_ != nullptr) {
    // eventFilter_->handle(this, event);
  }

  auto *evtdata = new MouseClickEventData();
  evtdata->nodeidx = this->getNodeIndex();
  evtdata->state = state;

  emit(EVT_MOUSE_CLICKED, std::make_unique<MouseClickedEvent>(0, std::move(evtdata)),
      [&](core::EventHandlerTypedefs::Ptr_t) { return true; });
}

}  // namespace sway::ui
