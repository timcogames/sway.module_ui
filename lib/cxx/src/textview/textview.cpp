#include <sway/ui/builder.hpp>
#include <sway/ui/controls/textview/textview.hpp>
#include <sway/ui/zindex.hpp>

namespace sway::ui {

TextView::TextView(BuilderPtr_t builder, const std::string &text)
    : Widget(builder)
    , text_(text)
    , font_("")
    , fontSize_(12) {
  setMouseFilter(ois::MouseFilter::IGNORE);

#ifdef UI_USE_GMOCK
  // TODO
#else
  resize();
#endif
}

TextView::~TextView() { std::cout << "~TextView" << std::endl; }

void TextView::resize() {
  auto fnt = builder_->getPainter()->getDefaultFont();
  auto wdt = 0.0F;
  auto hgt = 0.0F;

  for (auto i = 0; i < text_.size(); ++i) {
    auto charInfo = fnt->getCharInfo(text_[i]);
    if (!charInfo.has_value()) {
      return;
    }

    auto charSize = charInfo.value().size;
    auto charOffset = charInfo.value().bitmapGlyphOffset;

    wdt += charOffset.getX() + static_cast<f32_t>(charSize.getW());
    hgt = std::max(hgt, static_cast<f32_t>(charSize.getH()));
  }

  this->setSize(wdt, hgt);
}

void TextView::update() {}

void TextView::repaint(PainterSharedPtr_t painter) {
  auto zindex = getZIndex(2);
  auto offset = this->getOffset().computed;

  painter->drawText(
      math::rect4f_t(offset.getX(), offset.getY(), this->getSize()), this->getForegroundColor(), text_.c_str(), zindex);
}

void TextView::setText(const std::string &text) { text_ = text; }

}  // namespace sway::ui
