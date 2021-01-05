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

  qDebug() << "in size: "<<_countries.size();
  QStringList countries;
  countries.reserve(_countries.size());
  std::transform(_countries.begin(), _countries.end(),
                 std::back_inserter(countries),
                 [](const auto& std_str){
      return QString::fromStdString(std_str);
    });
  qDebug() << "out size: "<<countries.size();
  catalogue->setCountriesFilter(countries);
  catalogue->setCountries(std::move(countries));

  QStringList years;
  qDebug() << "in size "<<_years.size();
  years.reserve(_years.size());
  std::transform(_years.begin(), _years.end(),
                 std::back_inserter(years),
                 [](const auto& std_str){
      return QString::fromStdString(std_str);});
  qDebug() << "out size "<<years.size();
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
  qDebug() << "Filters built";
}

void DataManager::applyCountriesFilter(const QStringList &filter)
{
  QStringList stamps;
  for(const auto& [name, country]: _db)
    if(filter.contains(QString::fromStdString(name)))
      for(const auto& year: country)
        for(const auto& series: year.second){
          std::transform(series.second.begin(), series.second.end(),
                         std::back_inserter(stamps),
                         [](const auto& stamp){
              return "data:image/jpg;base64,"+QString::fromStdString(stamp.image);
            });
        }
  qDebug() << "Collected "<<stamps.size() <<"stamps";
  _catalogue->setStamps(std::move(stamps));
}

