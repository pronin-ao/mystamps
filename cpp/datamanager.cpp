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

  QStringList stamps;
  for(const auto& country: _db)
    for(const auto& year: country.second)
      for(const auto& series: year.second){
          std::transform(series.second.begin(), series.second.end(),
                         std::back_inserter(stamps),
                         [](const auto& stamp){
              return "data:image/jpg;base64,"+QString::fromStdString(stamp.image);
            });
        }
  qDebug() << "Collected "<<stamps.size() <<"stamps";
  catalogue->setStamps(std::move(stamps));
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
  QStringList stamps;
  for(const auto& [name, country]: _db)
    if(_country_filter.contains(QString::fromStdString(name)))
      for(const auto& [year, data]: country)
        if(years.empty() || years.contains(QString::fromStdString(year)))
          for(const auto& series: data){
            std::transform(series.second.begin(), series.second.end(),
                         std::back_inserter(stamps),
                         [](const auto& stamp){
              return "data:image/jpg;base64,"+QString::fromStdString(stamp.image);
              });
          }
  qDebug() << "Collected "<<stamps.size() <<"stamps";
  _catalogue->setStamps(std::move(stamps));
}


