#ifndef SWAY_UI_FT2_FACELOADER_HPP
#define SWAY_UI_FT2_FACELOADER_HPP

#include <sway/ui/_stdafx.hpp>
#include <sway/ui/ft2/face.hpp>

namespace sway::ui {

/**
 * @ingroup ft2
 * @{
 */

// struct ObjectFetchResponse : public rms::FetchResponse {
//   ObjectFetchResponse(lpcstr_t data, u32_t numBytes)
//       : rms::FetchResponse(data, numBytes) {}

//   auto serialize(FT_Library lib) -> std::shared_ptr<Face> {
//     return std::make_shared<Face>(lib, this->data, this->numBytes, 0);
//   }
// };

struct ObjectFetchResponse : public rms::FetchResponse {
  std::vector<u8_t> ownedData_;

  ObjectFetchResponse(lpcstr_t data, u32_t numBytes)
      : rms::FetchResponse(data, numBytes)
      , ownedData_(reinterpret_cast<const u8_t *>(data), reinterpret_cast<const u8_t *>(data) + numBytes) {
    this->data = reinterpret_cast<lpcstr_t>(ownedData_.data());
  }

  auto serialize(FT_Library lib) -> std::shared_ptr<Face> {
    return std::make_shared<Face>(lib, this->data, this->numBytes, 0);
  }
};

class FaceLoader : public rms::Fetcher {
public:
  FaceLoader(const std::string &url);

  virtual ~FaceLoader();

  MTHD_OVERRIDE(void fetch());

private:
  // std::shared_ptr<Face> faceResponse_;
};

/** @} */  // ingroup ft2

}  // namespace sway::ui

#endif  // SWAY_UI_FT2_FACELOADER_HPP
