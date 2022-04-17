#include "adminproxymodel.h"

#include "adminmodel.h"

AdminProxyModel::AdminProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void AdminProxyModel::setFilterData(const QString &country,
                                    const QString &year) {
  _country = country;
  _year = year;
  invalidateFilter();
}

bool AdminProxyModel::filterAcceptsRow(int source_row,
                                       const QModelIndex &source_parent) const {
  const auto &country =
      sourceModel()->index(source_row, admin::indexes::kCountry, source_parent);
  const auto &year =
      sourceModel()->index(source_row, admin::indexes::kYear, source_parent);
  const bool countryOk =
      _country.isEmpty() ||
      sourceModel()->data(country).toString().contains(_country);
  const bool yearOk = _country.isEmpty() ||
                      sourceModel()->data(year).toString().contains(_year);
  return countryOk && yearOk;
}
