#include <sway/ui/ft2/errormacros.hpp>
#include <sway/ui/ft2/face.hpp>

NAMESPACE_BEGIN(sway)
NAMESPACE_BEGIN(ui)
NAMESPACE_BEGIN(ft2)

Face::Face(FT_Library lib, lpcstr_t filepath, u32_t idx)
    : face_(nullptr) {
  auto success = CHECK_RESULT(FT_New_Face(lib, filepath, idx, &face_));
  if (!success) {
    // Empty
  }
}

Face::Face(FT_Library lib, lpcstr_t data, u32_t numBytes, u32_t idx)
    : face_(nullptr) {
  auto success = CHECK_RESULT(FT_New_Memory_Face(lib, (FT_Byte *)data, numBytes, idx, &face_));
  if (!success) {
    // Empty
  }

  FT_Select_Charmap(face_, FT_ENCODING_UNICODE);
}

Face::~Face() {
  FT_Done_Face(face_);
  face_ = nullptr;
}

NAMESPACE_END(ft2)
NAMESPACE_END(ui)
NAMESPACE_END(sway)
