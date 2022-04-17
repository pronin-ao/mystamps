#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QAbstractTableModel>

#include "dbmodel.h"

using DbHash =
    std::unordered_map<size_t, std::tuple<db::CountryName, db::YearVal,
                                          db::SeriesName, db::Number>>;

namespace admin::indexes {
static const int kCountry = 0;
static const int kYear = 1;
static const int kCodeNumber = 2;
static const int kPresence = 3;
static const int kForcedWishlist = 4;
static const int kImage = 5;
static const int kPrice = 6;
static const int kSwLink = 7;
static const int kColor = 8;
static const int kSpec = 9;
static const int kSeries = 10;
static const int kCapture = 11;
static const int kComments = 12;

} // namespace admin::indexes

class AdminModel : public QAbstractTableModel {
  static const int kSize = 13;
  Q_OBJECT
public:
  AdminModel();

  int columnCount(const QModelIndex &) const override;
  int rowCount(const QModelIndex &parent) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

  QStringList countryList() const;
  Qt::ItemFlags flags(const QModelIndex &index) const override;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

public slots:
  void setDbPointer(db::Catalogue *db);

private:
  DbHash _hash = {};
  db::Catalogue *_db = nullptr;

signals:
  void customDataChanged();
};

#endif // ADMINMODEL_H
