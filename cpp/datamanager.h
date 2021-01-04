#pragma once

#include <QObject>

#include "dbmodel.h"


class DataManager : public QObject
{
  Q_OBJECT
public:
  explicit DataManager(QObject *parent = nullptr);

  inline uint size() { return _size;}

 private:
  uint _size;
  db::Catalogue _db;

signals:

};

