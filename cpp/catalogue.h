#pragma once
#include <QObject>
#include <QStringList>
#include <qqml.h>


class Stamp {
  Q_GADGET
  Q_PROPERTY(QString capture READ capture)
  Q_PROPERTY(QString image READ image)
  Q_PROPERTY(QString price READ price)
  Q_PROPERTY(size_t index READ index)
  Q_PROPERTY(QString year READ year)
  Q_PROPERTY(QString country READ country)
  Q_PROPERTY(QString spec READ spec)
  Q_PROPERTY(bool checked READ checked WRITE setChecked)
  Q_PROPERTY(QString id READ id)

public:
  inline auto capture() const {return _capture;}
  inline auto image() const {return _image;}
  inline auto price() const {return _price;}
  inline auto index() const {return _index;}
  inline auto year() const {return _year;}
  inline auto country() const {return _country;}
  inline auto spec() const {return _spec;}
  inline auto checked() const {return _checked;}
  inline auto id() const {return _id;}

  void setChecked(const bool checked);

  QString _capture;
  QString _image;
  QString _price;
  size_t _index;
  QString _year;
  QString _country;
  QString _spec;
  bool _checked = false;
  QString _id;
};

namespace sorting {
  const QString kYear = "year";
  const QString kCountry = "country";
  const QString kPrice = "price";
  const QString kSeries = "series";
}
namespace order {
  const QString kRise = "rise";
  const QString kFall = "fall";

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

  QML_ELEMENT

public:
  explicit Catalogue(QObject *parent = nullptr);
  inline auto years() const {
    return  _years;
  }
  inline auto countries() const {
    return _countries;
  }
  inline auto yearsFilter() const {
    return _yearsFilter;
  }
  inline auto countriesFilter() const {
    return _countriesFilter;
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

  void setCountriesFilter(const QStringList& filter);
  void setYearsFilter(const QStringList& filter);

  void applyCountriesFilter(const QStringList& filter);
  void applyYearFilter(const QStringList& filter);

  void applySort(const QString& sort_type);
  void applyOrder(const QString& sort_order);


signals:
  void yearsChanged();
  void countriesChanged();
  void stampsUpdated();

  void sendCountriesFilter(const QStringList&);
  void sendYearsFilter(const QStringList&);

  void dataChanged();
  void sortingChanged();
  void orderChanged();

private:
  QStringList _years = {};
  QStringList _countries = {};

  QStringList _yearsFilter = {};
  QStringList _countriesFilter = {};

  QVariantList _stamps = {};

  QString _sorting = sorting::kYear;
  QString _order= order::kRise;
};

