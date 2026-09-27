#include <sway/ui/builder.hpp>
#include <sway/ui/controls/button/button.hpp>
#include <sway/ui/zindex.hpp>

namespace sway::ui {

auto Button::create(BuilderPtr_t builder, const std::string &text) -> ButtonSharedPtr_t {
  return std::make_shared<Button>(builder, text);
}

Button::Button(BuilderPtr_t builder, const std::string &text)
    : TextView(builder, text)
    , hovering_(false) {
  setMouseFilter(ois::MouseFilter::PASS);
}

Button::~Button() { std::cout << "~Button" << std::endl; }

void Button::updateState() {
  const auto oldState = hovering_;
  hovering_ = this->builder_->getWidgetUnderPointer()->getNodeIndex().equal(this->getNodeIndex());
  if (oldState == hovering_) {
    return;
  }

  if (hovering_) {
    this->setForegroundColor(COL4F_WHITE);
    this->setBackgroundColor(COL4F_GRAY2);
  } else {
    this->setForegroundColor(COL4F_BEIGE);
    this->setBackgroundColor(COL4F_GRAY1);
  }
}

void Button::update() {
  this->setAlignment(math::Alignment::CENTER);
  this->setOffset(this->getOffset().computed);
  this->setSize(this->getSize());

  // updateState();
}

void Button::repaint(PainterSharedPtr_t painter) {
  auto zindex = getZIndex(1);
  auto offset = this->getOffset().computed;

  painter->drawRect(math::rect4f_t(offset.getX(), offset.getY(), this->getSize()), this->getBackgroundColor(), zindex);

  TextView::repaint(painter);
}

}  // namespace sway::ui
