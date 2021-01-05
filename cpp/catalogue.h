#pragma once
#include <QObject>
#include <QStringList>
#include <qqml.h>


class Catalogue : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QStringList years  READ years WRITE setYears NOTIFY yearsChanged)
  Q_PROPERTY(QStringList countries
             READ countries WRITE setCountries NOTIFY countriesChanged)
  Q_PROPERTY(QStringList yearsFilter READ yearsFilter WRITE setYearsFilter)
  Q_PROPERTY(QStringList countriesFilter READ countriesFilter WRITE setCountriesFilter)
  Q_PROPERTY(QStringList stamps READ stamps NOTIFY stampsUpdated)

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
  void setStamps(QStringList&& stamps);

public slots:
  void setYears(QStringList&& years);
  void setYears(const QStringList& years);

  void setCountries(QStringList&& countries);
  void setCountries(const QStringList& countries);

  void setCountriesFilter(const QStringList& filter);
  void setYearsFilter(const QStringList& filter);

  void applyCountriesFilter(const QStringList& filter);
  void applyYearFilter(const QStringList& filter);


signals:
  void yearsChanged();
  void countriesChanged();
  void stampsUpdated();

  void sendCountriesFilter(const QStringList&);
  void sendYearsFilter(const QStringList&);

private:
  QStringList _years = {};
  QStringList _countries = {};

  QStringList _yearsFilter = {};
  QStringList _countriesFilter = {};

  QStringList _stamps = {};

};

