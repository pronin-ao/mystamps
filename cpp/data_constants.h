#ifndef DATA_CONSTANTS_H
#define DATA_CONSTANTS_H

#include <QByteArray>
#include <QString>

namespace paths {
#if defined(Q_OS_ANDROID)
const QString dbStampworldFile = "/sdcard/stamps/stamp_base.json";
const QString dbCustomFile = "/sdcard/stamps/custom_stamp_base.json";
const QString UserDataFile = "/sdcard/stamps//user_data.json";
const QString CustomDataFile = "/sdcard/stamps/custom_data.json";
const QString ImageDataFile = "/sdcard/stamps/image_data.json";
const QString kLocation = "/sdcard/stamps/";
#elif defined(Q_OS_LINUX)
const QString dbStampworldFile = "../stamp_base.json";
const QString dbCustomFile = "../custom_stamp_base.json";
const QString UserDataFile = "../user_data.json";
const QString CustomDataFile = "../custom_data.json";
const QString ImageDataFile = "../image_data.json";
const QString kLocation = "./";
#endif
} // namespace paths

namespace codes {
const QByteArray kBase64Image = "data:image/jpg;base64,";
}

#endif // DATA_CONSTANTS_H
