#include "dbparser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace names {
  const QString kYear = "year";
  const QString kCapture = "capture";
  const QString kStamps = "stamps";
  const QString kImage = "image";
  const QString kPrice = "price";
  const QString kType = "type";
  const QString kColor = "color";

}

struct Context {
  std::optional<db::Spec> spec;
  std::optional<db::CountryName> country;
};

using namespace std::string_literals;


QString GetWithDefault(const QJsonArray& array, size_t index,
                       const QString& def = "") {
  if(index >= static_cast<size_t>(array.size()))
    return def;
  return array[index].toString();
}

db::Series ParseSeries(const QJsonObject& json, const Context& context) {
  db::Series res;
  const auto& stamps_it = json.find(names::kStamps);
  if(stamps_it == json.end())
    throw std::runtime_error("Series has no stamps");
  if(!stamps_it->isObject())
    throw std::runtime_error("Stamps in not an object");

  const auto& stamps = stamps_it->toObject();
  res.reserve(stamps.size());
  for(auto it = stamps.begin(); it != stamps.end(); ++it){
      const auto& num = it.key().toStdString();
      const auto& stamp_v = it.value();
      if(!stamp_v.isObject())
        throw std::runtime_error("Stamp is not an object");
      const auto& stamp = stamp_v.toObject();
      db::Stamp new_stamp{
        stamp.value(names::kImage).toString("").toStdString(),
        context.spec.value_or(""),
        stamp.value(names::kPrice).toString("").toStdString(),
        stamp.value(names::kType).toString("").toStdString(),
        stamp.value(names::kCapture).toString("").toStdString(),
        stamp.value(names::kColor).toString("").toStdString(),
        db::AddParams{}
      };
      res.emplace(num, std::move(new_stamp));
    }
  return res;
}

db::Country ParseSpec(const QJsonValue& json, const Context& context){
  db::Country res;
  if(!json.isArray())
    throw std::runtime_error(context.spec.value_or("-")
                             +" of country "s+
                             context.country.value_or("-")+" is not an array");
  const auto& series = json.toArray();
  for(const auto& ser: series){
      if(!ser.isObject())
        throw std::runtime_error("Seria is not an object");
      const auto& object = ser.toObject();
      const auto& year_it = object.find(names::kYear);
      if(year_it == object.end())
        throw std::runtime_error("Seria has no year");
      const db::YearVal year = year_it->toString().toStdString();
      const auto& seria = ParseSeries(object, context);
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

db::Country ParseCountry(const QJsonValue& json, const Context& context){
  db::Country res;
  if(!json.isObject())
    throw std::runtime_error("Country "s+ context.country.value_or("-")
                             +" is not an object");
  const auto& country = json.toObject();
  for(auto it = country.begin(); it != country.end(); ++it){
      const auto& spec = it.key().toStdString();
      auto new_context = context;
      new_context.spec = spec;
      const auto& part = ParseSpec(*it, new_context);
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
      Context context{};
      context.country = it.key().toStdString();
      res.emplace(it.key().toStdString(),
                  ParseCountry(*it, context));
    }

  return res;
}
