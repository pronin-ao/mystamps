#include "dbparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace names {
  const QString kYear = "year";
  const QString kCapture = "capture";
  const QString kStamps = "stamps";
  const QString kImages = "images";

}


QString GetWithDefault(const QJsonArray& array, size_t index,
                       const QString& def = "") {
  if(index >= static_cast<size_t>(array.size()))
    return def;
  return array[index].toString();
}

db::Series ParseSeries(const QJsonObject& json, const db::Spec& spec) {
  db::Series res;
  const auto& stamps_it = json.find(names::kStamps);
  if(stamps_it == json.end())
    throw std::runtime_error("Series has no stamps");
  const auto& images_it = json.find(names::kImages);
  if(images_it == json.end())
    throw std::runtime_error("Series has no images");
  if(!stamps_it->isArray())
    throw std::runtime_error("Stamps in not an array");
  if(!images_it->isArray())
    throw std::runtime_error("Images is not an array");

  const auto& stamps = stamps_it->toArray();
  const auto& images = images_it->toArray();
  if(images.size() > stamps.size())
    qWarning() << "We have more images then stamps, some images will be dummy";

  size_t i = 0;
  res.reserve(images.size());
  for(const auto& image: images){
      const auto& stamp = GetWithDefault(stamps, i);
      db::Stamp new_stamp{
        i,
        image.toString().toStdString(),
        spec,
        stamp.toStdString(),
        "###"
      };
      res.emplace_back(std::move(new_stamp));
      ++i;
    }
  return res;
}

db::Country ParseSpec(const QJsonValue& json, const db::Spec& spec){
  db::Country res;
  if(!json.isArray())
    throw std::runtime_error(spec+" of country is not an array");
  const auto& series = json.toArray();
  for(const auto& ser: series){
      if(!ser.isObject())
        throw std::runtime_error("Seria is not an object");
      const auto& object = ser.toObject();
      const auto& year_it = object.find(names::kYear);
      if(year_it == object.end())
        throw std::runtime_error("Seria has no year");
      const db::YearVal year = year_it->toString().toStdString();
      const auto& seria = ParseSeries(object, spec);
      const auto& name_it = object.find(names::kCapture);
      if(name_it == object.end())
        throw std::runtime_error("Seria has no name");
      const db::SeriesName& name = name_it->toString().toStdString();
      if(!res.count(year))
        res[year] = {};
      res[year].emplace(name, seria);
    }
  return res;
}

void AppendTo(db::Country& global, const db::Country& spec) {
  for(const auto& [key, val] : spec){
      if(global.count(key))
        global[key].merge(db::Year{val});
      else
        global.emplace(key,val);
    }
}

db::Country ParseCountry(const QJsonValue& json){
  db::Country res;
  if(!json.isObject())
    throw std::runtime_error("Country is not an object");
  const auto& country = json.toObject();
  for(auto it = country.begin(); it != country.end(); ++it){
      const auto& spec = it.key().toStdString();
      const auto& part = ParseSpec(*it, spec);
      AppendTo(res, part);
    }
  return res;
}

db::Catalogue ParseCatalogue(const QString &str)
{
  QJsonDocument json = QJsonDocument::fromJson(str.toUtf8());
  if(!json.isObject())
    throw std::runtime_error{"Global value is not an object"};
  const auto& root = json.object();
  db::Catalogue res;
  for(auto it = root.begin(); it != root.end(); ++it){
      qInfo() << "Loading "<<it.key();
      res.emplace(it.key().toStdString(),
                  ParseCountry(*it));
    }

  return res;
}
