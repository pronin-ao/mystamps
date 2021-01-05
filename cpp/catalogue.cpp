#include "catalogue.h"

Catalogue::Catalogue(QObject *parent) : QObject(parent)
{

}

void Catalogue::setYears(QStringList &&years)
{
  _years = std::move(years);
  emit yearsChanged();

}

void Catalogue::setYears(const QStringList& years)
{
  _years = years;
  emit yearsChanged();
}

void Catalogue::setCountries(QStringList &&countries)
{
  _countries = std::move(countries);
  emit countriesChanged();
}

void Catalogue::setCountries(const QStringList &countries)
{
  _countries = countries;
  emit countriesChanged();
}

void Catalogue::setCountriesFilter(const QStringList &filter)
{
  _countriesFilter = filter;
}

void Catalogue::setYearsFilter(const QStringList &filter)
{
  _yearsFilter = filter;
}

void Catalogue::applyCountriesFilter(const QStringList &filter)
{
  qDebug() << "Apply countries: "<<filter;
  _countriesFilter = filter;
  emit sendCountriesFilter(_countriesFilter);
}

void Catalogue::applyYearFilter(const QStringList &filter)
{
  _yearsFilter = filter;
}

void Catalogue::setStamps(QStringList &&stamps)
{
  _stamps = std::move(stamps);
  emit stampsUpdated();
}


