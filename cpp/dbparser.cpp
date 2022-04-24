#include "dbparser.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "data_constants.h"

namespace names {
const QString kCountry = "country";
const QString kSeries = "series";
const QString kId = "id";
const QString kYear = "year";
const QString kCapture = "capture";
const QString kStamps = "stamps";
const QString kImage = "image";
const QString kPrice = "price";
const QString kType = "type";
const QString kColor = "color";
const QString kOwned = "owned";
const QString kCond = "cond";

const QString kSwLink = "sw_link";
const QString kHasList = "has_list";
const QString kListOwned = "list_owned";
const QString kListCond = "list_cond";
const QString kListNote = "list_note";

const QString kChecked = "checked";

const QString kForcedWihslist = "forced_wishlist";
const QString kComments = "comments";

} // namespace names

struct Context {
  std::optional<db::Spec> spec;
  std::optional<db::CountryName> country;
};

using namespace std::string_literals;

QString GetWithDefault(const QJsonArray &array, size_t index,
                       const QString &def = "") {
  if (index >= static_cast<size_t>(array.size()))
    return def;
  return array[index].toString();
}

db::Series ParseSeries(const QJsonObject &json, const Context &context) {
  db::Series res;
  const auto &stamps_it = json.find(names::kStamps);
  if (stamps_it == json.end())
    throw std::runtime_error("Series has no stamps");
  if (!stamps_it->isObject())
    throw std::runtime_error("Stamps in not an object");

  const auto &sw_link =
      json.value(names::kSwLink).toString("").remove(-2, 2).toStdString();
  const bool has_list = json.value(names::kHasList).toBool(false);
  std::optional<bool> list_owned{};
  std::optional<db::Conditions> list_conditions{};
  if (has_list) {
    list_owned = json.value(names::kListOwned).toBool(false);
    if (json.contains(names::kListCond)) {
      const auto &cond_json = json.value(names::kListCond).toArray({});
      db::Conditions cond{};
      cond.reserve(cond_json.size());
      std::transform(
          cond_json.begin(), cond_json.end(), std::back_inserter(cond),
          [](const auto &val) { return val.toString().toStdString(); });
      list_conditions = std::move(cond);
    }
  }

  const auto &stamps = stamps_it->toObject();
  res.reserve(stamps.size());
  for (auto it = stamps.begin(); it != stamps.end(); ++it) {
    const auto &num = it.key().toStdString();
    const auto &stamp_v = it.value();
    if (!stamp_v.isObject())
      throw std::runtime_error("Stamp is not an object");
    const auto &stamp = stamp_v.toObject();
    const auto &cond_json = stamp.value(names::kCond).toArray({});
    db::Conditions cond{};
    cond.reserve(cond_json.size());
    std::transform(
        cond_json.begin(), cond_json.end(), std::back_inserter(cond),
        [](const auto &val) { return val.toString().toStdString(); });
    db::ListNote list_note{};
    if (stamp.contains(names::kListNote)) {
      list_note = stamp[names::kListNote].toString().toStdString();
    }
    db::Stamp new_stamp{stamp.value(names::kImage).toString("").toStdString(),
                        context.spec.value_or(""),
                        stamp.value(names::kPrice).toString("").toStdString(),
                        stamp.value(names::kType).toString("").toStdString(),
                        stamp.value(names::kCapture).toString("").toStdString(),
                        stamp.value(names::kColor).toString("").toStdString(),
                        stamp.value(names::kOwned).toBool(false),
                        std::move(cond),
                        sw_link,
                        has_list,
                        list_note,
                        list_owned,
                        list_conditions,
                        db::UserParams{}};
    res.emplace(num, std::move(new_stamp));
  }
  return res;
}

db::Country ParseSpec(const QJsonValue &json, const Context &context) {
  db::Country res;
  if (!json.isArray())
    throw std::runtime_error(context.spec.value_or("-") + " of country "s +
                             context.country.value_or("-") +
                             " is not an array");
  const auto &series = json.toArray();
  for (const auto &ser : series) {
    if (!ser.isObject())
      throw std::runtime_error("Seria is not an object");
    const auto &object = ser.toObject();
    const auto &year_it = object.find(names::kYear);
    if (year_it == object.end())
      throw std::runtime_error("Seria has no year");
    const db::YearVal year = year_it->toString().toStdString();
    auto seria = ParseSeries(object, context);
    const auto &name_it = object.find(names::kCapture);
    if (name_it == object.end())
      throw std::runtime_error("Seria has no name");
    const db::SeriesName &name = name_it->toString().toStdString();
    if (!res.count(year))
      res[year] = {};
    if (!res[year].count(name))
      res[year].emplace(name, seria);
    else {
      // qDebug() << "Seria "<< name_it->toString() << " duplicated";
      res[year][name].merge(seria);
      if (!seria.empty()) {
        for (auto &stamp : seria) {
          if (stamp.second.owned)
            res[year][name][stamp.first] = stamp.second;
        }
      }
    }
  }
  return res;
}

void AppendTo(db::Country &global, const db::Country &spec) {
  for (const auto &[key, val] : spec) {
    if (global.count(key))
      global[key].merge(db::Year{val});
    else
      global.emplace(key, val);
  }
}

db::Country ParseCountry(const QJsonValue &json, const Context &context) {
  db::Country res;
  if (!json.isObject())
    throw std::runtime_error("Country "s + context.country.value_or("-") +
                             " is not an object");
  const auto &country = json.toObject();
  for (auto it = country.begin(); it != country.end(); ++it) {
    const auto &spec = it.key().toStdString();
    auto new_context = context;
    new_context.spec = spec;
    const auto &part = ParseSpec(*it, new_context);
    AppendTo(res, part);
  }
  return res;
}

db::Catalogue ParseStampworldCatalogue(const QString &str) {
  QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
  if (!json.isObject())
    throw std::runtime_error{"Global value is not an object"};
  const auto &root = json.object();
  db::Catalogue res;
  for (auto it = root.begin(); it != root.end(); ++it) {
    qInfo() << "Loading " << it.key();
    Context context{};
    context.country = it.key().toStdString();
    res.emplace(it.key().toStdString(), ParseCountry(*it, context));
  }

  return res;
}
///////////////////////////////////////////////////////////////////////////////
QJsonDocument AddParamsToJson(const db::Catalogue &db) {
  QJsonArray res;
  for (const auto &[country, country_data] : db)
    for (const auto &[year, year_data] : country_data)
      for (const auto &[series, series_data] : year_data)
        for (const auto &[id, stamp_data] : series_data) {
          QJsonObject stamp;
          stamp[names::kCountry] = QString::fromStdString(country);
          stamp[names::kYear] = QString::fromStdString(year);
          stamp[names::kSeries] = QString::fromStdString(series);
          stamp[names::kId] = QString::fromStdString(id);
          stamp[names::kChecked] = stamp_data.user.checked;
          res.append(stamp);
        }
  return QJsonDocument(res);
}

QString SerializeUserData(const db::Catalogue &db) {
  const auto &json = AddParamsToJson(db);
  return QString::fromUtf8(json.toJson());
}

void ParseUserData(const QString &str, db::Catalogue &db) {
  QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
  if (json.isEmpty())
    return;
  if (!json.isArray())
    throw std::runtime_error{"User data value is not an array"};
  const auto &array = json.array();
  for (const auto &stamp : array) {
    try {
      const auto &country = stamp[names::kCountry].toString().toStdString();
      const auto &year = stamp[names::kYear].toString().toStdString();
      const auto &series = stamp[names::kSeries].toString().toStdString();
      const auto &id = stamp[names::kId].toString().toStdString();
      db.at(country).at(year).at(series).at(id).user =
          db::UserParams{stamp[names::kChecked].toBool()};
    } catch (const std::out_of_range &err) {
      // qDebug() << "Skipping a stamp "<<stamp;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
QString SerializeCustomData(const db::Catalogue &db) {
  QJsonArray res;
  for (const auto &[country, country_data] : db)
    for (const auto &[year, year_data] : country_data)
      for (const auto &[series, series_data] : year_data)
        for (const auto &[id, stamp_data] : series_data) {
          if (!stamp_data.custom || !stamp_data.custom.has_value())
            continue;
          const auto &data = *stamp_data.custom;
          QJsonObject stamp;
          stamp[names::kCountry] = QString::fromStdString(country);
          stamp[names::kYear] = QString::fromStdString(year);
          stamp[names::kSeries] = QString::fromStdString(series);
          stamp[names::kId] = QString::fromStdString(id);
          if (stamp_data.custom->forced_wishlist)
            stamp[names::kForcedWihslist] =
                stamp_data.custom->forced_wishlist.value();
          if (stamp_data.custom->comments)
            stamp[names::kComments] =
                QString::fromStdString(stamp_data.custom->comments.value());
          res.append(stamp);
        }
  return QString::fromUtf8(QJsonDocument(res).toJson());
}

void ParseCustomData(const QString &str, db::Catalogue &db) {
  QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
  if (json.isEmpty())
    return;
  if (!json.isArray())
    throw std::runtime_error{"Custom data value is not an array"};
  const auto &array = json.array();
  for (const auto &stamp : array) {
    try {
      const auto &country = stamp[names::kCountry].toString().toStdString();
      const auto &year = stamp[names::kYear].toString().toStdString();
      const auto &series = stamp[names::kSeries].toString().toStdString();
      const auto &id = stamp[names::kId].toString().toStdString();
      const auto &stamp_obj = stamp.toObject();
      auto &data = db.at(country).at(year).at(series).at(id).custom;
      data.emplace();
      if (stamp_obj.contains(names::kForcedWihslist))
        data->forced_wishlist = stamp[names::kForcedWihslist].toBool();
      if (stamp_obj.contains(names::kComments))
        data->comments = stamp[names::kComments].toString().toStdString();
    } catch (const std::out_of_range &err) {
      // qDebug() << "Skipping a stamp "<<stamp;
    }
  }
}

QString SerializeImagesData(const db::Catalogue &db) {
  QJsonArray res;
  for (const auto &[country, country_data] : db)
    for (const auto &[year, year_data] : country_data)
      for (const auto &[series, series_data] : year_data)
        for (const auto &[id, stamp_data] : series_data) {
          if (stamp_data.imageSource() != db::ImageSources::kPhoto)
            continue;
          const auto &data = *stamp_data.custom;
          QJsonObject stamp;
          stamp[names::kCountry] = QString::fromStdString(country);
          stamp[names::kYear] = QString::fromStdString(year);
          stamp[names::kSeries] = QString::fromStdString(series);
          stamp[names::kId] = QString::fromStdString(id);
          stamp[names::kImage] = QString::fromStdString(stamp_data.image);
          res.append(stamp);
        }
  return QString::fromUtf8(QJsonDocument(res).toJson());
}

void ParseImagesData(const QString &str, db::Catalogue &db) {
  QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
  if (json.isEmpty())
    return;
  if (!json.isArray())
    throw std::runtime_error{"Image data value is not an array"};
  const auto &array = json.array();
  for (const auto &stamp : array) {
    try {
      const auto &country = stamp[names::kCountry].toString().toStdString();
      const auto &year = stamp[names::kYear].toString().toStdString();
      const auto &series = stamp[names::kSeries].toString().toStdString();
      const auto &id = stamp[names::kId].toString().toStdString();
      auto &stamp_data = db.at(country).at(year).at(series).at(id);
      if (!stamp_data.image.empty())
        stamp_data.image_params.fallback_image = stamp_data.image;
      stamp_data.image = stamp[names::kImage].toString().toStdString();
    } catch (const std::out_of_range &err) {
      qDebug() << "Skipping a stamp " << stamp;
    }
  }
}
