#pragma once

#include<set>

#include <QObject>

#include "dbmodel.h"
#include "catalogue.h"



class DataManager : public QObject
{
  Q_OBJECT
public:
  explicit DataManager(QObject *parent = nullptr);

  void setCatalogue(Catalogue* catalogue);

public slots:
  void applyCountriesFilter(const QStringList& filter);
  void applyYearsFilter(const QStringList& filter);
  void applyPriceFilter(const QStringList& filter);
  void applyShowMode(const QString& showMode);

  void registerStampChecked(const Stamp& stamp);

 private:
  db::Catalogue _db;

  void collectAllFilters();
  void filterYearsForCountry();
  void filterPriceForYears();

  void saveAddToFile();

  std::set<std::string> _countries;
  std::set<std::string> _years;
  std::set<std::string> _prices;

  QStringList _country_filter;
  QStringList _year_filter;
  QStringList _price_filter;
  QString _showMode = show::kAll;

  Catalogue* _catalogue;

signals:

};

