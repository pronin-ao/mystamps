#pragma once

#include <set>

#include <QObject>

#include "catalogue_constants.h"
#include "dbmodel.h"
#include "stamp.h"

class DataManager : public QObject {
  Q_OBJECT
public:
  explicit DataManager(QObject *parent = nullptr);

  void initiateDataForQml();
  void initiateDataForAdmin();

public slots:
  void applyCountriesFilter(const QStringList &filter);
  void applyYearsFilter(const QStringList &filter);
  void applyPriceFilter(const QStringList &filter);
  void applyShowMode(const QString &showMode);

  void registerStampChecked(const Stamp &stamp);

  void saveCustomData() const;

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

signals:
  void sendCountriesFilter(const QStringList &);
  void sendCountries(QSharedPointer<QStringList>);
  void sendYearsFilter(const QStringList &);
  void sendYears(QSharedPointer<QStringList>);
  void sendPricesFilter(const QStringList &);
  void sendPrices(QSharedPointer<QStringList>);
  void sendStamps(QSharedPointer<QVariantList>);
  void sendDbPointer(db::Catalogue *db);
};
