#ifndef ADMIN_H
#define ADMIN_H

#include <QNetworkAccessManager>
#include <QWidget>

#include "adminmodel.h"
#include "adminproxymodel.h"
#include "delegates.h"

namespace Ui {
class AdminGui;
}

class Admin : public QWidget {
  Q_OBJECT

public:
  explicit Admin(QWidget *parent = nullptr);
  ~Admin();

public slots:
  void setDbPointerToModel(db::Catalogue *db);

  void applyFilter();

private:
  Ui::AdminGui *ui;
  AdminModel sourceModel;
  QNetworkAccessManager network;
  AdminProxyModel proxyModel;
  WishlistDelegate wishListDelegate;
  CommenstDelegate commentsDelegate;

  void CreateItemWidgets();

signals:
  void dataChanged();
};

#endif // ADMIN_H
