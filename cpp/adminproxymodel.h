#ifndef ADMINPROXYMODEL_H
#define ADMINPROXYMODEL_H

#include <QSortFilterProxyModel>

class AdminProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
public:
  AdminProxyModel(QObject *parent = nullptr);
  void setFilterData(const QString &country, const QString &year);

protected:
  bool filterAcceptsRow(int source_row,
                        const QModelIndex &source_parent) const override;

private:
  QString _country;
  QString _year;
};

#endif // ADMINPROXYMODEL_H
