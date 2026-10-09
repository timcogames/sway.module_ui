#include <sway/ui/builder.hpp>
#include <sway/ui/widget/progress/progressbar.hpp>
#include <sway/ui/zindex.hpp>

#include <algorithm>

namespace sway::ui {

ProgressBar::ProgressBar(BuilderPtr_t builder)
    : Widget(builder)
    , mode_(ProgressMode::Enum::PERCENTAGE)
    , current_(0.0F)
    , total_(1.0F) {}

void ProgressBar::update() {
  std::string text = "";
  switch (mode_) {
    case ProgressMode::Enum::FRACTION: {
      std::ostringstream fractionString;
      fractionString << current_ << "/" << total_;
      text += fractionString.str();
      break;
    }
    case ProgressMode::Enum::PERCENTAGE: {
      std::ostringstream percentageString;
      percentageString << (f32_t)current_ * 100 / total_ << "%";
      text += percentageString.str();
      break;
    }
    case ProgressMode::Enum::INITIAL:
    default:
      break;
  }

  std::cout << text << std::endl;
}

void ProgressBar::repaint(PainterSharedPtr_t painter) {
  const auto offset = this->getOffset().computed;

  auto rectWithMargin = math::rect4f_t(offset.getX(), offset.getY(), this->getSize());
  rectWithMargin.reduce(math::Rect<f32_t>(1.5F, 1.5F, math::size2f_t(1.5F, 1.5F)));

  auto barLen = rectWithMargin.getL() + rectWithMargin.getW() * current_;

  painter->drawRect(
      rectWithMargin.getL(), rectWithMargin.getT(), barLen, rectWithMargin.getB(), COL4F_BEIGE, getZIndex(1));
  painter->drawRect(math::rect4f_t(offset.getX(), offset.getY(), this->getSize()), COL4F_GRAY1, getZIndex(2));
}

void ProgressBar::setProgress(f32_t val) {
  if (current_ == val) {
    return;
  }

  current_ = std::clamp(val, 0.0F, total_);
}

void ProgressBar::addProgress(f32_t val) { this->setProgress(current_ + val); }

}  // namespace sway::ui
