#include "dbmodel.h"

#include "data_constants.h"

namespace db {

ImageSources Stamp::imageSource() const {
  if (image.empty())
    return ImageSources::kNoImage;
  if (image.substr(0, codes::kBase64Image.size()) ==
      codes::kBase64Image.toStdString())
    return ImageSources::kPhoto;
  return ImageSources::kStampworld;
}

} // namespace db
