#include <sway/ois/events/inputevent.hpp>
#include <sway/ois/events/keyeventhandler.hpp>
#include <sway/ois/events/mouseevent.hpp>
#include <sway/ois/web/emsmouse.hpp>
#include <sway/ui/builder.hpp>

NAMESPACE_BEGIN(sway)
NAMESPACE_BEGIN(ui)

struct WidgetEventHandler : public core::evts::EventHandler {
  WidgetEventHandler(Builder::Ptr_t builder)
      : core::evts::EventHandler()
      , builder_(builder) {}

  ~WidgetEventHandler() override = default;

  // clang-format off
  MTHD_OVERRIDE(auto invoke(const std::unique_ptr<core::foundation::Event> &evt) -> bool) final {  // clang-format on
    if (ois::InputEventUtil::isMouseEvent(evt)) {
      auto mouseEvtData = evt->getConcreteData<ois::MouseEventData>();
      if (evt->type() == core::detail::toBase(ois::InputActionType::MOUSE_MOVED)) {
        builder_->setCursorPoint(mouseEvtData.point);
      }

      auto cursor = builder_->getCursor();
      auto target = builder_->getRootWidget()->getChildAtPoint(cursor.pnt);

      builder_->updateWidgetUnderPointer(target);

      if (evt->type() == core::detail::toBase(ois::InputActionType::MOUSE_BUTTON)) {
        if (mouseEvtData.btnCode == core::detail::toBase(ois::MouseButtonCode::LMB)) {
          builder_->handleMouseClick();
        }
      }
    }

    return true;
  }

private:
  Builder::Ptr_t builder_;
};

Builder::Builder(core::foundation::Context *context, Painter::SharedPtr_t painter)
    : core::foundation::Object(context)
    , currWidgetUnderPointer_(nullptr)
    , painter_(painter) {
  root_ = std::make_shared<widget::Widget>(this);
}

void Builder::initialize(std::shared_ptr<ft2::Font> font, std::shared_ptr<render::MaterialManager> materialMngr,
    std::shared_ptr<rms::ImageResourceManager> imgResMngr, std::shared_ptr<rms::GLSLResourceManager> glslResMngr) {
  auto ctx = this->getContext();
  auto renderSubsystem = ctx->getSubsystem<render::RenderSubsystem>("RenderSubsystem").value();

  painter_->initialize(font, renderSubsystem, materialMngr, imgResMngr, glslResMngr);

  subscriber_ = evtbus_->subscribe(new WidgetEventHandler(this));

  // root_->setAbsolutePosition(0, 0);
  // root_->setSize(800, 600);
}

void Builder::deinit() {
  evtbus_->unsubscribe(subscriber_);
  currWidgetUnderPointer_ = nullptr;
}

void Builder::update() { root_->paintEvent(painter_); }

void Builder::updateWidgetUnderPointer(widget::Widget::Ptr_t target) {
  if (currWidgetUnderPointer_ == target) {
    return;
  }

  auto prevWidgetUnderPointer = currWidgetUnderPointer_;
  currWidgetUnderPointer_ = target;

  if (prevWidgetUnderPointer) {
    prevWidgetUnderPointer->onCursorPointerLeave();
  }

  if (currWidgetUnderPointer_) {
    currWidgetUnderPointer_->onCursorPointerEnter();
  }
}

void Builder::handleMouseClick() {
  if (currWidgetUnderPointer_) {
    currWidgetUnderPointer_->onMouseClick();
  }
}

NAMESPACE_END(ui)
NAMESPACE_END(sway)
