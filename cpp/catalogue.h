#pragma once
#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <qqml.h>

#include "catalogue_constants.h"
#include "stamp.h"

class Catalogue : public QObject {
  Q_OBJECT
  Q_PROPERTY(QStringList years READ years WRITE setYears NOTIFY yearsChanged)
  Q_PROPERTY(QStringList countries READ countries WRITE setCountries NOTIFY
                 countriesChanged)
  Q_PROPERTY(QStringList yearsFilter READ yearsFilter WRITE setYearsFilter)
  Q_PROPERTY(QStringList countriesFilter READ countriesFilter WRITE
                 setCountriesFilter NOTIFY countriesFilterChanged)
  Q_PROPERTY(QVariantList stamps READ stamps NOTIFY stampsUpdated)
  Q_PROPERTY(QString sorting READ sorting NOTIFY sortingChanged)
  Q_PROPERTY(QString order READ order NOTIFY orderChanged)
  Q_PROPERTY(QStringList priceFilter READ priceFilter WRITE setPriceFilter)
  Q_PROPERTY(
      QStringList prices READ prices WRITE setPrices NOTIFY pricesChanged)
  Q_PROPERTY(QString showMode READ showMode WRITE applyShowMode)
  Q_PROPERTY(QString location READ location)

  QML_ELEMENT

public:
  explicit Catalogue(QObject *parent = nullptr);
  inline auto years() const { return *_years; }
  inline auto countries() const { return *_countries; }
  inline auto prices() const { return *_prices; }
  inline auto yearsFilter() const { return _yearsFilter; }
  inline auto countriesFilter() const { return _countriesFilter; }
  inline auto priceFilter() const { return _priceFilter; }
  inline auto showMode() const { return _showMode; }
  inline auto stamps() const { return *_stamps; }
  inline auto sorting() const { return _sorting; }
  inline auto order() const { return _order; }
  inline auto location() const { return _location; }

public slots:
  void setStamps(QSharedPointer<QVariantList> stamps);

  void setYears(QSharedPointer<QStringList> years);
  void setYears(const QStringList &years);

  void setCountries(QSharedPointer<QStringList> countries);
  void setCountries(const QStringList &countries);

  void setPrices(QSharedPointer<QStringList> prices);
  void setPrices(const QStringList &prices);

  void setCountriesFilter(const QStringList &filter);
  void setYearsFilter(const QStringList &filter);
  void setPriceFilter(const QStringList &filter);

  void applyCountriesFilter(const QStringList &filter);
  void applyYearFilter(const QStringList &filter);
  void applyPriceFilter(const QStringList &filter);
  void applyShowMode(const QString &showMode);

  void applySort(const QString &sort_type);
  void applyOrder(const QString &sort_order);

  void checkStamp(const Stamp &stamp);

  void filter(QString text, QString filterType);

  void getImage(const Stamp &stamp, const QString &image);

signals:
  void yearsChanged();
  void countriesChanged();
  void pricesChanged();

  void stampsUpdated();

  void sendCountriesFilter(const QStringList &);
  void sendYearsFilter(const QStringList &);
  void sendPriceFilter(const QStringList &);
  void sendShowMode(const QString &);

  void dataChanged();
  void sortingChanged();
  void orderChanged();

  void stampChanges(const Stamp &);
  void saveImageForStamp(const Stamp &, const QString &);

  void getFilter(const QStringList &list);
  void countriesFilterChanged();

private:
  QSharedPointer<QStringList> _years = QSharedPointer<QStringList>::create();
  QSharedPointer<QStringList> _countries =
      QSharedPointer<QStringList>::create();
  QSharedPointer<QStringList> _prices = QSharedPointer<QStringList>::create();

  QString _location;

  QStringList _fullYears = {};
  QStringList _fullCountries = {};
  QStringList _fullPrices = {};

  QStringList _yearsFilter = {};
  QStringList _countriesFilter = {};
  QStringList _priceFilter = {};
  QString _showMode = show::kAll;

  QSharedPointer<QVariantList> _stamps =
      QSharedPointer<QVariantList>::create(QVariantList{});

  QString _sorting = sorting::kYear;
  QString _order = order::kRaise;
};
