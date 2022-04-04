#ifndef CATALOGUE_CONSTANTS_H
#define CATALOGUE_CONSTANTS_H

#include <QString>

namespace sorting {
const QString kYear = "year";
const QString kCountry = "country";
const QString kPrice = "price";
const QString kSeries = "series";
} // namespace sorting

namespace filters {
const QString kCountries = "countries";
}
namespace order {
const QString kRaise = "raise";
const QString kFall = "fall";

} // namespace order

namespace show {
const QString kAll = "all";
const QString kOwned = "owned";
const QString kWishlist = "wishlist";
} // namespace show

#endif // CATALOGUE_CONSTANTS_H
