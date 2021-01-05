#include "datamanager.h"

#include <QFile>

#include <QDebug>

#include "dbparser.h"

const QString Filename = ":/db/stamps";

QString ReadFile(){
  QString val;
  QFile file;
  file.setFileName(Filename);
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
  const auto& json = ReadFile();
  _db = ParseCatalogue(json);
  collectAllFilters();
}

void DataManager::setCatalogue(Catalogue *catalogue)
{
  _catalogue = catalogue;
  connect(catalogue, &Catalogue::sendCountriesFilter,
          this, &DataManager::applyCountriesFilter);
  connect(catalogue, &Catalogue::sendYearsFilter,
          this, &DataManager::applyYearsFilter);

  auto countries = QStringListFromSet(_countries);
  catalogue->setCountriesFilter(countries);
  catalogue->setCountries(std::move(countries));

  auto years = QStringListFromSet(_years);
  catalogue->setYearsFilter(years);
  catalogue->setYears(std::move(years));

  applyYearsFilter({});
}

void DataManager::collectAllFilters()
{
  for(const auto& [country, data]: _db){
      _countries.insert(country);
      qDebug() << QString::fromStdString(country)
               << " years: "<<data.size();
      for(const auto& [year, year_data]: data){
          _years.insert(year);
        }
    }
}

void DataManager::filterYearsForCountry()
{
  if(_country_filter.size() == _country_filter.size()){
      auto all_years = QStringListFromSet(_years);
      _catalogue->setYears(std::move(all_years));
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

void DataManager::applyCountriesFilter(const QStringList &countries) {
  QStringList stamps;
  _country_filter = countries;
  applyYearsFilter({});
  filterYearsForCountry();
}

void DataManager::applyYearsFilter(const QStringList &years) {
  QVariantList stamps;
  for(const auto& [name, country]: _db){
    const auto& country_name = name;
    if(static_cast<size_t>(_country_filter.size())== _countries.size() ||
       _country_filter.empty() ||
       _country_filter.contains(QString::fromStdString(name)))
      for(const auto& [year_val, data]: country) {
        const auto& year = year_val;
        if(years.empty() || years.contains(QString::fromStdString(year))){
          for(const auto& series: data){
              std::transform(series.second.begin(), series.second.end(),
                           std::back_inserter(stamps),
                           [&cap=series.first, &year, &name=country_name](const auto& stamp){
                  QVariant var;
                var.setValue(
                      Stamp{
                        QString::fromStdString(cap),
                        "data:image/jpg;base64,"+QString::fromStdString(stamp.image),
                        QString::fromStdString(stamp.price),
                        stamp.index,
                        QString::fromStdString(year),
                        QString::fromStdString(name),
                        QString::fromStdString(stamp.spec),
                        false /*checked*/,
                        QString::fromStdString(stamp.number)
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


