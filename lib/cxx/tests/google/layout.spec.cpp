#include <sway/ui.hpp>

#undef Bool
#undef None

#include <gtest/gtest.h>

#include <memory>

NS_SHORT_SWAY()

TEST(LayoutTest, ctor_def) {
  auto builder = std::make_shared<ui::Builder>(nullptr);
  builder->setEventBus(std::make_shared<core::EventBus>());
  builder->initialize();

  auto mainLayout = ui::LinearLayout::create(builder.get());
  EXPECT_EQ(mainLayout->getOrientation(), ui::Orientation::VERT);

  builder->getRootWidget()->removeChildNode(mainLayout);
  builder->deinit();
}

TEST(LayoutTest, build) {
  auto builder = std::make_shared<ui::Builder>(nullptr);
  builder->setEventBus(std::make_shared<core::EventBus>());
  builder->initialize();

  auto mainLayout = ui::LinearLayout::create(builder.get(), ui::Orientation::VERT);
  mainLayout->setSize(800.0F, 600.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_L>(5.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_R>(5.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_T>(5.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_B>(5.0F);
  mainLayout->setSizePolicy(ui::DimensionType::ALL, ui::SizePolicyType::MATCH_PARENT);
  mainLayout->setSizeDimensions({800.0F, 600.0F});

  ASSERT_EQ(mainLayout->getSizePolicy().dimensions[0].policy, ui::SizePolicyType::MATCH_PARENT);
  ASSERT_EQ(mainLayout->getSizePolicy().dimensions[1].policy, ui::SizePolicyType::MATCH_PARENT);

  auto btn1 = ui::Button::create(builder.get(), "btn1");
  btn1->setAlignment(math::Alignment::LEFT_TOP);
  btn1->setOffset(0.0F, 0.0F);
  btn1->setSize(150.0F, 70.0F);
  btn1->setWeight(0.3F);
  btn1->setSizePolicy(ui::DimensionType::WDT, ui::SizePolicyType::MATCH_PARENT);
  btn1->setSizePolicy(ui::DimensionType::HGT, ui::SizePolicyType::WRAP_CONTENT);
  btn1->setSizeDimensions({150.0F, 70.0F});
  mainLayout->addChildNode(btn1);

  ASSERT_EQ(btn1->getSizePolicy().dimensions[0].policy, ui::SizePolicyType::MATCH_PARENT);
  ASSERT_EQ(btn1->getSizePolicy().dimensions[1].policy, ui::SizePolicyType::WRAP_CONTENT);

  mainLayout->removeChildNode(btn1);

  auto btn2 = ui::Button::create(builder.get(), "btn2");
  btn2->setAlignment(math::Alignment::LEFT_TOP);
  btn2->setOffset(0.0F, 0.0F);
  btn2->setSize(150.0F, 70.0F);
  btn2->setWeight(0.1F);
  btn2->setSizePolicy(ui::DimensionType::WDT, ui::SizePolicyType::MATCH_PARENT);
  btn2->setSizePolicy(ui::DimensionType::HGT, ui::SizePolicyType::WRAP_CONTENT);
  btn2->setSizeDimensions({150.0F, 70.0F});
  mainLayout->addChildNode(btn2);

  ASSERT_EQ(btn2->getSizePolicy().dimensions[0].policy, ui::SizePolicyType::MATCH_PARENT);
  ASSERT_EQ(btn2->getSizePolicy().dimensions[1].policy, ui::SizePolicyType::WRAP_CONTENT);
  ASSERT_EQ(std::get<0>(btn2->getSizePolicy().dimensions[0].value), 150.0F);
  ASSERT_EQ(std::get<0>(btn2->getSizePolicy().dimensions[1].value), 70.0F);

  // std::cout << "btn1: " << Representation<core::NodeIndex>::get(btn1->getNodeIndex())
  //           << " offset: " << btn1->getOffset().computed << std::endl;

  std::cout << "btn2: " << Representation<core::NodeIndex>::get(btn2->getNodeIndex())
            << " offset: " << btn2->getOffset().computed << std::endl;

  mainLayout->removeChildNode(btn2);
  // mainLayout->removeChildNode(btn1);

  builder->getRootWidget()->removeChildNode(mainLayout);
  builder->deinit();
}

TEST(LayoutTest, build_center) {
  auto builder = std::make_shared<ui::Builder>(nullptr);
  builder->setEventBus(std::make_shared<core::EventBus>());
  builder->initialize();

  auto mainLayout = ui::LinearLayout::create(builder.get());
  mainLayout->setSize(800.0F, 600.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_L>(5.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_R>(5.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_T>(5.0F);
  mainLayout->getAreaHolder().setEdge<ui::AreaType::IDX_MRG, math::RectEdge::Enum::IDX_B>(5.0F);
  mainLayout->setSizePolicy(ui::DimensionType::ALL, ui::SizePolicyType::MATCH_PARENT);
  mainLayout->setSizeDimensions({800.0F, 600.0F});

  auto btn1 = ui::Button::create(builder.get(), "btn1");
  btn1->setAlignment(math::Alignment::CENTER);
  btn1->setOffset(0.0F, 0.0F);
  btn1->setSize(150.0F, 70.0F);
  btn1->setSizeDimensions({150.0F, 70.0F});
  mainLayout->addChildNode(btn1);

  auto btn2 = ui::Button::create(builder.get(), "btn2");
  btn2->setAlignment(math::Alignment::CENTER);
  btn2->setOffset(0.0F, 0.0F);
  btn2->setSize(150.0F, 70.0F);
  btn2->setSizeDimensions({150.0F, 70.0F});
  mainLayout->addChildNode(btn2);

  std::cout << "btn1: " << Representation<core::NodeIndex>::get(btn1->getNodeIndex())
            << " offset: " << btn1->getOffset().computed << std::endl;

  std::cout << "btn2: " << Representation<core::NodeIndex>::get(btn2->getNodeIndex())
            << " offset: " << btn2->getOffset().computed << std::endl;

  mainLayout->removeChildNode(btn2);
  mainLayout->removeChildNode(btn1);

  builder->getRootWidget()->removeChildNode(mainLayout);
  builder->deinit();
}
