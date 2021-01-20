#include "catalogue.h"

Catalogue::Catalogue(QObject *parent) : QObject(parent)
{
  qRegisterMetaType<Stamp>("Stamp");
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

void Catalogue::setPrices(QStringList &&prices)
{
  _prices = std::move(prices);
  emit pricesChanged();
}

void Catalogue::setPrices(const QStringList &prices)
{
  _prices = prices;
  emit pricesChanged();
}

void Catalogue::setCountriesFilter(const QStringList &filter)
{
  _countriesFilter = filter;
}

void Catalogue::setYearsFilter(const QStringList &filter)
{
  _yearsFilter = filter;
}

void Catalogue::setPriceFilter(const QStringList &filter)
{
  _priceFilter = filter;
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
  emit sendYearsFilter(_yearsFilter);
}

void Catalogue::applyPriceFilter(const QStringList &filter)
{
  _priceFilter = filter;
  emit sendPriceFilter(_priceFilter);
}

void Catalogue::applyShowMode(const QString &showMode)
{
  if(showMode != show::kAll && showMode != show::kOwned && showMode != show::kWishlist)
    return;
  _showMode = showMode;
  emit sendShowMode(showMode);
}

void Catalogue::applySort(const QString &sort_type)
{
  static const auto& price_cmp = [](const QVariant& v1, const QVariant& v2){
      const auto& s1 = v1.value<Stamp>();
      const auto& s2 = v2.value<Stamp>();
      return s1.price().toLower() < s2.price().toLower();
    };

  static const auto& series_cmp = [](const QVariant& v1, const QVariant& v2){
      const auto& s1 = v1.value<Stamp>();
      const auto& s2 = v2.value<Stamp>();
      if(s1.series() == s2.series())
        return price_cmp(v1, v2);
      return s1.series().toLower() < s2.series().toLower();
    };
  static const auto& year_cmp = [](const QVariant& v1, const QVariant& v2){
      const auto& s1 = v1.value<Stamp>();
      const auto& s2 = v2.value<Stamp>();
      if(s1.year() == s2.year())
        return series_cmp(v1, v2);
      return s1.year() < s2.year();
    };
  static const auto& country_cmp = [](const QVariant& v1, const QVariant& v2){
      const auto& s1 = v1.value<Stamp>();
      const auto& s2 = v2.value<Stamp>();
      if(s1.country() == s2.country())
        return year_cmp(v1, v2);
      return s1.country() < s2.country();
    };
  static const QHash<QString,
      std::function<bool(const QVariant&, const QVariant&)>> sorting =
    {{sorting::kPrice, price_cmp},
    {sorting::kSeries, series_cmp},
    {sorting::kYear, year_cmp},
    {sorting::kCountry, country_cmp}};
  if(!sorting.contains(sort_type))
    return;

  _sorting = sort_type;
  const auto& compare = (_order == order::kRaise)
      ? sorting[sort_type]
        : ([sort_type](const QVariant& v1, const QVariant& v2){
      return !sorting[sort_type](v1,v2);
    });
  std::sort(_stamps.begin(), _stamps.end(), compare);
  emit stampsUpdated();
  emit sortingChanged();
}

void Catalogue::applyOrder(const QString &sort_order)
{
  if(sort_order != order::kRaise && sort_order != order::kFall)
    return;
  _order = sort_order;
  applySort(_sorting);
  emit orderChanged();
}

void Catalogue::checkStamp(const Stamp &stamp)
{
  emit stampChanges(stamp);
}

void Catalogue::setStamps(QVariantList &&stamps)
{
  _stamps = std::move(stamps);
  applySort(_sorting);
}

void Stamp::setChecked(const bool checked)
{
  _checked = checked;
}
