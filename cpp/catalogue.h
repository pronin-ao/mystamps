#pragma once
#include <QObject>
#include <QStringList>
#include <qqml.h>


class Stamp {
  Q_GADGET
  Q_PROPERTY(QString capture READ capture)
  Q_PROPERTY(QString image READ image)
  Q_PROPERTY(QString price READ price)
  Q_PROPERTY(QString year READ year)
  Q_PROPERTY(QString country READ country)
  Q_PROPERTY(QString spec READ spec)
  Q_PROPERTY(bool checked READ checked WRITE setChecked)
  Q_PROPERTY(QString id READ id)
  Q_PROPERTY(QString code READ code)
  Q_PROPERTY(QString color READ color)
  Q_PROPERTY(QString series READ series)
  Q_PROPERTY(bool owned READ owned)
  Q_PROPERTY(QStringList conditions READ conditions)

public:
  inline auto capture() const {return _capture;}
  inline auto image() const {return _image;}
  inline auto price() const {return _price;}
  inline auto year() const {return _year;}
  inline auto country() const {return _country;}
  inline auto spec() const {return _spec;}
  inline auto checked() const {return _checked;}
  inline auto id() const {return _id;}
  inline auto code() const {return _code;}
  inline auto color() const {return _color;}
  inline auto series() const {return _series;}
  inline auto owned() const {return _owned;}
  inline auto conditions() const {return _conditions;}

  void setChecked(const bool checked);

  QString _capture;
  QString _image;
  QString _price;
  QString _year;
  QString _country;
  QString _spec;
  bool _checked = false;
  QString _id;
  QString _code;
  QString _color;
  QString _series;
  bool _owned;
  QStringList _conditions;
};

namespace sorting {
  const QString kYear = "year";
  const QString kCountry = "country";
  const QString kPrice = "price";
  const QString kSeries = "series";
}
namespace order {
  const QString kRaise = "raise";
  const QString kFall = "fall";

}

namespace show {
  const QString kAll = "all";
  const QString kOwned = "owned";
  const QString kWishlist = "wishlist";
}


class Catalogue : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QStringList years  READ years WRITE setYears NOTIFY yearsChanged)
  Q_PROPERTY(QStringList countries
             READ countries WRITE setCountries NOTIFY countriesChanged)
  Q_PROPERTY(QStringList yearsFilter READ yearsFilter WRITE setYearsFilter)
  Q_PROPERTY(QStringList countriesFilter READ countriesFilter WRITE setCountriesFilter)
  Q_PROPERTY(QVariantList stamps READ stamps NOTIFY stampsUpdated)
  Q_PROPERTY(QString sorting READ sorting NOTIFY sortingChanged)
  Q_PROPERTY(QString order READ order NOTIFY orderChanged)
  Q_PROPERTY(QStringList priceFilter READ priceFilter WRITE setPriceFilter)
  Q_PROPERTY(QStringList prices
             READ prices WRITE setPrices NOTIFY pricesChanged)
  Q_PROPERTY(QString showMode READ showMode WRITE applyShowMode)

  QML_ELEMENT

public:
  explicit Catalogue(QObject *parent = nullptr);
  inline auto years() const {
    return  _years;
  }
  inline auto countries() const {
    return _countries;
  }
  inline auto prices() const {
    return _prices;
  }
  inline auto yearsFilter() const {
    return _yearsFilter;
  }
  inline auto countriesFilter() const {
    return _countriesFilter;
  }
  inline auto priceFilter() const  {
    return _priceFilter;
  }
  inline auto showMode() const  {
    return _showMode;
  }
  inline auto stamps() const {
    return _stamps;
  }
  inline auto sorting() const{
    return _sorting;
  }
  inline auto order() const {
    return _order;
  }
  void setStamps(QVariantList&& stamps);

public slots:
  void setYears(QStringList&& years);
  void setYears(const QStringList& years);

  void setCountries(QStringList&& countries);
  void setCountries(const QStringList& countries);

  void setPrices(QStringList&& prices);
  void setPrices(const QStringList& prices);

  void setCountriesFilter(const QStringList& filter);
  void setYearsFilter(const QStringList& filter);
  void setPriceFilter(const QStringList& filter);

  void applyCountriesFilter(const QStringList& filter);
  void applyYearFilter(const QStringList& filter);
  void applyPriceFilter(const QStringList& filter);
  void applyShowMode(const QString& showMode);

  void applySort(const QString& sort_type);
  void applyOrder(const QString& sort_order);

  void checkStamp(const Stamp& stamp);


signals:
  void yearsChanged();
  void countriesChanged();
  void pricesChanged();

  void stampsUpdated();

  void sendCountriesFilter(const QStringList&);
  void sendYearsFilter(const QStringList&);
  void sendPriceFilter(const QStringList&);
  void sendShowMode(const QString&);

  void dataChanged();
  void sortingChanged();
  void orderChanged();

  void stampChanges(const Stamp&);

private:
  QStringList _years = {};
  QStringList _countries = {};
  QStringList _prices = {};

  QStringList _yearsFilter = {};
  QStringList _countriesFilter = {};
  QStringList _priceFilter = {};
  QString _showMode = show::kAll;

  QVariantList _stamps = {};

  QString _sorting = sorting::kYear;
  QString _order= order::kRaise;
};

