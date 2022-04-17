#include "adminmodel.h"

#include <QColor>
#include <QDebug>
#include <QIcon>
#include <QUrl>

namespace {
static const QModelIndex nulli = QModelIndex();
}

AdminModel::AdminModel() : QAbstractTableModel() {}

int AdminModel::columnCount(const QModelIndex &) const {
  return AdminModel::kSize;
}

int AdminModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return _hash.size();
}

QVariant AdminModel::data(const QModelIndex &index, int role) const {
  using namespace admin::indexes;
  const auto column = index.column();
  const auto &[country, year, series, number] = _hash.at(index.row());
  const auto &stamp = (*_db).at(country).at(year).at(series).at(number);
  if (role == Qt::DisplayRole) {
    const auto &it = _hash.find(index.row());
    if (it == _hash.end()) {
      qWarning() << "Wrong model index in admin: " << index.row();
      return {};
    }
    if (column == kCountry)
      return {QString::fromStdString(country)};
    if (column == kYear)
      return {QString::fromStdString(year)};
    if (column == kSeries)
      return {QString::fromStdString(series)};
    if (column == kCodeNumber)
      return {QString::fromStdString("SW " + number + "/" + stamp.code)};
    if (column == kSpec)
      return {QString::fromStdString(stamp.spec)};

    if (column == kPrice)
      return {QString::fromStdString(stamp.price)};
    if (column == kCapture)
      return {QString::fromStdString(stamp.capture)};
    if (column == kComments)
      return {QString::fromStdString(
          stamp.custom ? stamp.custom->comments.value_or("-") : "-")};
    if (column == kColor)
      return {QString::fromStdString(stamp.color)};
    if (column == kForcedWishlist)
      return stamp.custom
                 ? (stamp.custom->forced_wishlist.value_or(false) ? "+" : "-")
                 : "-";
  }

  if (role == Qt::EditRole) {
    if (column == kForcedWishlist)
      return stamp.custom ? stamp.custom->forced_wishlist.value_or(false)
                          : false;
    if (column == kComments)
      return {QString::fromStdString(
          stamp.custom ? stamp.custom->comments.value_or("") : "")};
  }
  if (role == Qt::UserRole) {
    if (column == kSwLink)
      return QString::fromStdString(stamp.sw_link);
    if (column == kImage)
      return QString::fromStdString(stamp.image);
  }
  if (role == Qt::DecorationRole) {
    if (column == kPresence)
      return stamp.owned ? QColor(Qt::green) : QColor(Qt::red);
  }

  return {};
}

QVariant AdminModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  using namespace admin::indexes;
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    if (section == kCountry)
      return "Country";
    if (section == kYear)
      return "Year";
    if (section == kSeries)
      return "Series name";
    if (section == kCodeNumber)
      return "Num\\Code";
    if (section == kSpec)
      return "Type";
    if (section == kImage)
      return "Image";
    if (section == kPrice)
      return "Nominal";
    if (section == kCapture)
      return "Capture";
    if (section == kPresence)
      return "Presence";
    if (section == kColor)
      return "Color";
    if (section == kSwLink)
      return "Stampworld link";
    if (section == kForcedWishlist)
      return "Forced wishlist";
    if (section == kComments)
      return "Comments";
  }
  return {};
}

QStringList AdminModel::countryList() const {
  QStringList res;
  res.reserve(_db->size());
  for (const auto &[country_name, _] : *_db) {
    res.append(QString::fromStdString(country_name));
  }
  return res;
}

Qt::ItemFlags AdminModel::flags(const QModelIndex &index) const {
  using namespace admin::indexes;
  const auto column = index.column();
  auto defaultFlags = QAbstractTableModel::flags(index);
  if (column == kForcedWishlist || column == kComments)
    defaultFlags |= Qt::ItemIsEditable;
  return defaultFlags;
}

bool AdminModel::setData(const QModelIndex &index, const QVariant &value,
                         int role) {
  if (role != Qt::EditRole)
    return QAbstractTableModel::setData(index, value, role);
  using namespace admin::indexes;
  const auto column = index.column();
  const auto &[country, year, series, number] = _hash.at(index.row());
  auto &stamp = (*_db).at(country).at(year).at(series).at(number);
  if (column == kForcedWishlist && value.canConvert(QMetaType::Bool)) {
    const bool new_value = value.toBool();
    if (!new_value && !stamp.custom)
      return true;
    if (new_value && !stamp.custom) {
      stamp.custom.emplace();
    }
    if (new_value)
      stamp.custom->forced_wishlist.emplace(true);
    else
      stamp.custom->forced_wishlist = std::nullopt;
    emit dataChanged(index, index, {});
    emit customDataChanged();
    return true;
  }
  if (column == kComments && value.canConvert(QMetaType::QString)) {
    const auto &new_value = value.toString();
    if (new_value.isEmpty() && !stamp.custom)
      return true;
    if (!new_value.isEmpty() && !stamp.custom) {
      stamp.custom.emplace();
    }
    if (!new_value.isEmpty())
      stamp.custom->comments.emplace(new_value.toStdString());
    else
      stamp.custom->comments = std::nullopt;
    emit dataChanged(index, index, {});
    emit customDataChanged();
    return true;
  }
  return QAbstractTableModel::setData(index, value, role);
}

void AdminModel::setDbPointer(db::Catalogue *db) {
  _db = db;
  size_t counter = 0;
  DbHash tmp;
  for (const auto &[country_name, country] : *db) {
    for (const auto &[year_val, data] : country) {
      for (const auto &[series_name, series] : data) {
        for (const auto &[id, _] : series) {
          tmp.emplace(counter++,
                      std::make_tuple(country_name, year_val, series_name, id));
        }
      }
    }
  }
  beginInsertRows(nulli, 0, counter - 1);
  _hash = std::move(tmp);
  endInsertRows();
  emit dataChanged(index(0, 0, nulli), index(_hash.size(), 20, nulli));
}
