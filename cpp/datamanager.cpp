#include "datamanager.h"

#include <QBuffer>
#include <QFile>

#include <QDebug>

#include "data_constants.h"
#include "dbparser.h"

QString ReadFile(const QString &filename) {
  QString val;
  QFile file;
  file.setFileName(filename);
  qDebug() << "Reading " << filename;
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  val = file.readAll();
  file.close();
  return val;
}

QStringList QStringListFromSet(const std::set<std::string> &set) {
  QStringList res;
  res.reserve(set.size());
  std::transform(
      set.begin(), set.end(), std::back_inserter(res),
      [](const auto &std_str) { return QString::fromStdString(std_str); });
  return res;
}

DataManager::DataManager(QObject *parent) : QObject(parent) {
  const auto &json = ReadFile(paths::dbFilename);
  _db = ParseStampworldCatalogue(json);
  collectAllFilters();
  const auto &user_json = ReadFile(paths::UserDataFile);
  ParseUserData(user_json, _db);
  const auto &custom_json = ReadFile(paths::CustomDataFile);
  ParseCustomData(custom_json, _db);
  const auto &image_json = ReadFile(paths::ImageDataFile);
  ParseImagesData(image_json, _db);
  emit sendDbPointer(&_db);
}

void DataManager::initiateDataForQml() {
  auto countries = QStringListFromSet(_countries);
  emit sendCountriesFilter(countries);
  emit sendCountries(QSharedPointer<QStringList>::create(std::move(countries)));

  auto years = QStringListFromSet(_years);
  emit sendYearsFilter(years);
  emit sendYears(QSharedPointer<QStringList>::create(std::move(years)));

  auto prices = QStringListFromSet(_prices);
  emit sendPricesFilter(prices);
  emit sendPrices(QSharedPointer<QStringList>::create(std::move(prices)));

  applyPriceFilter({});
}

void DataManager::initiateDataForAdmin() { emit sendDbPointer(&_db); }

void DataManager::collectAllFilters() {
  for (const auto &[country, data] : _db) {
    _countries.insert(country);
    qDebug() << QString::fromStdString(country) << " years: " << data.size();
    for (const auto &[year, year_data] : data) {
      _years.insert(year);
      for (const auto &series : year_data) {
        for (const auto &[_, stamp] : series.second) {
          _prices.insert(stamp.price);
        }
      }
    }
  }
}

void DataManager::filterYearsForCountry() {
  if (static_cast<size_t>(_country_filter.size()) == _countries.size()) {
    auto all_years = QStringListFromSet(_years);
    emit sendYears(QSharedPointer<QStringList>::create(std::move(all_years)));
    return;
  }
  std::set<std::string> years;
  for (const auto &[country, data] : _db) {
    if (_country_filter.contains(QString::fromStdString(country)))
      for (const auto &[year, year_data] : data) {
        years.insert(year);
      }
  }
  qDebug() << _years.size() << " and " << years.size();
  emit sendYears(
      QSharedPointer<QStringList>::create(QStringListFromSet(years)));
}

void DataManager::filterPriceForYears() {
  if (static_cast<size_t>(_country_filter.size()) == _countries.size() ||
      static_cast<size_t>(_year_filter.size()) == _years.size()) {
    auto all_prices = QStringListFromSet(_prices);
    emit sendPrices(QSharedPointer<QStringList>::create(std::move(all_prices)));
  }
  std::set<std::string> prices;
  for (const auto &[country, data] : _db) {
    if (_country_filter.contains(QString::fromStdString(country)))
      for (const auto &[year, year_data] : data) {
        for (const auto &series : year_data) {
          for (const auto &[_, stamp] : series.second)
            prices.insert(stamp.price);
        }
      }
  }
  emit sendPrices(
      QSharedPointer<QStringList>::create(QStringListFromSet(prices)));
}

void DataManager::saveUserToFile() const {
  const auto &new_data = SerializeUserData(_db);
  QFile file;
  file.setFileName(paths::UserDataFile);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  file.write(new_data.toUtf8());
  file.close();
}

void DataManager::saveImagesToFile() const {
  const auto &new_data = SerializeImagesData(_db);
  QFile file;
  file.setFileName(paths::ImageDataFile);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  file.write(new_data.toUtf8());
  file.close();
}

void DataManager::applyCountriesFilter(const QStringList &countries) {
  QStringList stamps;
  _country_filter = countries;
  applyYearsFilter({});
  filterYearsForCountry();
}

void DataManager::applyYearsFilter(const QStringList &years) {
  _year_filter = years;
  applyPriceFilter({});
  filterPriceForYears();
}

void DataManager::applyPriceFilter(const QStringList &filter) {
  _price_filter = filter;
  QVariantList stamps;
  for (const auto &[name, country] : _db) {
    const auto &country_name = name;
    if (static_cast<size_t>(_country_filter.size()) == _countries.size() ||
        _country_filter.empty() ||
        _country_filter.contains(QString::fromStdString(name)))
      for (const auto &[year_val, data] : country) {
        const auto &year = year_val;
        if (_year_filter.empty() ||
            static_cast<size_t>(_year_filter.size()) == _years.size() ||
            _year_filter.contains(QString::fromStdString(year))) {
          for (const auto &series : data) {
            decltype(series.second) filtered;
            std::copy_if(
                series.second.begin(), series.second.end(),
                std::inserter(filtered, filtered.begin()),
                [&prices = this->_price_filter, &all_prices = this->_prices,
                 &showMode = this->_showMode](const auto &stamp) {
                  if (prices.empty() ||
                      static_cast<size_t>(prices.size()) == all_prices.size() ||
                      prices.contains(
                          QString::fromStdString(stamp.second.price))) {
                    if (showMode == show::kAll ||
                        (showMode == show::kOwned && stamp.second.owned) ||
                        (showMode == show::kWishlist && !stamp.second.owned))
                      return true;
                  }
                  return false;
                });
            std::transform(
                filtered.begin(), filtered.end(), std::back_inserter(stamps),
                [&cap = series.first, &year,
                 &name = country_name](const auto &stamp) {
                  QVariant var;
                  QStringList conds{};
                  conds.reserve(stamp.second.condition.size());
                  std::transform(stamp.second.condition.begin(),
                                 stamp.second.condition.end(),
                                 std::back_inserter(conds),
                                 [](const auto &str) {
                                   return QString::fromStdString(str);
                                 });

                  QStringList list_conds{};
                  if (stamp.second.list_conditions) {
                    conds.reserve(stamp.second.list_conditions->size());
                    std::transform(stamp.second.list_conditions->begin(),
                                   stamp.second.list_conditions->end(),
                                   std::back_inserter(list_conds),
                                   [](const auto &str) {
                                     return QString::fromStdString(str);
                                   });
                  }
                  var.setValue(Stamp{
                      QString::fromStdString(stamp.second.capture),
                      QString::fromStdString(stamp.second.image),
                      QString::fromStdString(stamp.second.price),
                      QString::fromStdString(year),
                      QString::fromStdString(name),
                      QString::fromStdString(stamp.second.spec),
                      stamp.second.user.checked,
                      QString::fromStdString(stamp.first),
                      QString::fromStdString(stamp.second.code),
                      QString::fromStdString(stamp.second.color),
                      QString::fromStdString(cap),
                      (stamp.second.owned &&
                       (!stamp.second.custom ||
                        !stamp.second.custom->forced_wishlist.value_or(false))),
                      std::move(conds),
                      QString::fromStdString(stamp.second.sw_link),
                      stamp.second.has_list,
                      QString::fromStdString(
                          stamp.second.list_note.value_or("")),
                      stamp.second.list_owned.value_or(false), list_conds,
                      QString::fromStdString(
                          stamp.second.custom
                              ? stamp.second.custom->comments.value_or("")
                              : "")});
                  return var;
                });
          }
        }
      }
  }
  qDebug() << "Collected " << stamps.size() << "stamps";
  emit sendStamps(QSharedPointer<QVariantList>::create(std::move(stamps)));
}

void DataManager::applyShowMode(const QString &showMode) {
  _showMode = showMode;
  applyPriceFilter(_price_filter);
}

void DataManager::registerStampChecked(const Stamp &st) {
  _db.at(st._country.toStdString())
      .at(st._year.toStdString())
      .at(st._series.toStdString())
      .at(st._id.toStdString())
      .user.checked = st._checked;
  saveUserToFile();
}

void DataManager::saveImageForStamp(const Stamp &st,
                                    const QString &image_file) {
  QFile file(image_file);
  file.open(QIODevice::ReadOnly);
  QByteArray image = file.readAll();
  if (file.exists())
    while (!file.remove())
      ;
  qDebug() << st._country << st._year << st._series << st._id;
  auto &stamp = _db.at(st._country.toStdString())
                    .at(st._year.toStdString())
                    .at(st._series.toStdString())
                    .at(st._id.toStdString());
  if (!stamp.image.empty())
    stamp.image_params.fallback_image = stamp.image;
  stamp.image = (codes::kBase64Image + image.toBase64()).toStdString();
  applyCountriesFilter(_country_filter);
  saveImagesToFile();
}

void DataManager::saveCustomData() const {
  const auto &new_data = SerializeCustomData(_db);
  QFile file;
  file.setFileName(paths::CustomDataFile);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  file.write(new_data.toUtf8());
  file.close();
}

void DataManager::saveImageData() const { saveImagesToFile(); }
