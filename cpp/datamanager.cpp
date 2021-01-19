#include "datamanager.h"

#include <QFile>

#include <QDebug>


#include "dbparser.h"

#if defined(Q_OS_ANDROID)
const QString dbFilename = "/sdcard/stamps/stamp_base.json";
const QString Datafile = "/sdcard/stamps/mystamps-data.json";
#elif defined(Q_OS_LINUX)
const QString dbFilename = "../cpp/res/stamp_base.json";
const QString Datafile = "../cpp/res/mystamps-data.json";
#endif



QString ReadFile(const QString& filename){
  QString val;
  QFile file;
  file.setFileName(filename);
  qDebug() << "Reading "<<filename;
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  val = file.readAll();
  file.close();
  return val;
}

QStringList QStringListFromSet(const std::set<std::string>& set) {
  QStringList res;
  res.reserve(set.size());
  std::transform(set.begin(), set.end(),
                 std::back_inserter(res),
                 [](const auto& std_str){
      return QString::fromStdString(std_str);
    });
  return res;
}


DataManager::DataManager(QObject *parent) : QObject(parent)
{
  const auto& json = ReadFile(dbFilename);
  _db = ParseCatalogue(json);
  collectAllFilters();
  const auto& add_json = ReadFile(Datafile);
  ParseAddData(add_json, _db);
}

void DataManager::setCatalogue(Catalogue *catalogue)
{
  _catalogue = catalogue;
  connect(catalogue, &Catalogue::sendCountriesFilter,
          this, &DataManager::applyCountriesFilter);
  connect(catalogue, &Catalogue::sendYearsFilter,
          this, &DataManager::applyYearsFilter);
  connect(catalogue, &Catalogue::sendPriceFilter,
          this, &DataManager::applyPriceFilter);
  connect(catalogue, &Catalogue::stampChanges,
          this, &DataManager::registerStampChecked);

  auto countries = QStringListFromSet(_countries);
  catalogue->setCountriesFilter(countries);
  catalogue->setCountries(std::move(countries));

  auto years = QStringListFromSet(_years);
  catalogue->setYearsFilter(years);
  catalogue->setYears(std::move(years));

  auto prices = QStringListFromSet(_prices);
  catalogue->setPriceFilter(prices);
  catalogue->setPrices(std::move(prices));

  applyPriceFilter({});
}

void DataManager::collectAllFilters()
{
  for(const auto& [country, data]: _db){
      _countries.insert(country);
      qDebug() << QString::fromStdString(country)
               << " years: "<<data.size();
      for(const auto& [year, year_data]: data){
          _years.insert(year);
          for(const auto& seriees: year_data){
              for(const auto& [_, stamp]: seriees.second){
                  _prices.insert(stamp.price);
                }
            }
        }
    }
}

void DataManager::filterYearsForCountry()
{
  if(static_cast<size_t>(_country_filter.size()) == _countries.size()){
      auto all_years = QStringListFromSet(_years);
      _catalogue->setYears(std::move(all_years));
      return;
    }
  std::set<std::string> years;
  for(const auto& [country, data]: _db){
     if(_country_filter.contains(QString::fromStdString(country)))
      for(const auto& [year, year_data]: data){
          years.insert(year);
        }
    }
  qDebug() << _years.size() << " and "<< years.size();
  _catalogue->setYears(QStringListFromSet(years));
}

void DataManager::filterPriceForYears()
{
  if(static_cast<size_t>(_country_filter.size()) == _countries.size() ||
     static_cast<size_t>(_year_filter.size()) == _years.size()){
      auto all_prices = QStringListFromSet(_prices);
      _catalogue->setPrices(std::move(all_prices));
    }
  std::set<std::string> prices;
  for(const auto& [country, data]: _db){
     if(_country_filter.contains(QString::fromStdString(country)))
      for(const auto& [year, year_data]: data){
          for(const auto& series: year_data){
              for(const auto& [_, stamp]: series.second)
                prices.insert(stamp.price);
            }
        }
    }
  _catalogue->setPrices(QStringListFromSet(prices));
}

void DataManager::saveAddToFile()
{
  const auto& new_data = SerializeAddData(_db);
  QFile file;
  file.setFileName(Datafile);
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

void DataManager::applyPriceFilter(const QStringList &filter)
{
  _price_filter = filter;
  QVariantList stamps;
  for(const auto& [name, country]: _db){
    const auto& country_name = name;
    if(static_cast<size_t>(_country_filter.size())== _countries.size() ||
       _country_filter.empty() ||
       _country_filter.contains(QString::fromStdString(name)))
      for(const auto& [year_val, data]: country) {
        const auto& year = year_val;
        if(_year_filter.empty() ||
           static_cast<size_t>(_year_filter.size()) == _years.size() ||
           _year_filter.contains(QString::fromStdString(year))){
          for(const auto& series: data){
              decltype(series.second) filtered;
              std::copy_if(series.second.begin(), series.second.end(),
                           std::inserter(filtered, filtered.begin()),
                           [&prices = this->_price_filter,
                           &all_prices = this->_prices](const auto& stamp){
                  if(prices.empty() ||
                     static_cast<size_t>(prices.size()) == all_prices.size() ||
                     prices.contains(QString::fromStdString(stamp.second.price)))
                    return true;
                  return false;
                });
              std::transform(filtered.begin(), filtered.end(),
                           std::back_inserter(stamps),
                           [&cap=series.first, &year, &name=country_name](const auto& stamp){
                  QVariant var;
                var.setValue(
                      Stamp{
                        QString::fromStdString(stamp.second.capture),
                        QString::fromStdString(stamp.second.image),
                        QString::fromStdString(stamp.second.price),
                        QString::fromStdString(year),
                        QString::fromStdString(name),
                        QString::fromStdString(stamp.second.spec),
                        stamp.second.add.checked,
                        QString::fromStdString(stamp.first),
                        QString::fromStdString(stamp.second.code),
                        QString::fromStdString(stamp.second.color),
                        QString::fromStdString(cap)
                      });
                return var;
                });
            }
          }
        }
  }
  qDebug() << "Collected "<<stamps.size() <<"stamps";
  _catalogue->setStamps(std::move(stamps));
}

void DataManager::registerStampChecked(const Stamp &st)
{
  _db[st._country.toStdString()][st._year.toStdString()][st._series.toStdString()]
      [st._id.toStdString()].add.checked = st._checked;
  saveAddToFile();
}


