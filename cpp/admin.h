#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>

namespace Ui {
class Admin;
}

class Admin : public QWidget {
  Q_OBJECT

public:
  explicit Admin(QWidget *parent = nullptr);
  ~Admin();

private:
  Ui::Admin *ui;
};

#endif // ADMIN_H
