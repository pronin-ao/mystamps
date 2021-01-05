#pragma once

#include<set>

#include <QObject>

#include "dbmodel.h"
#include "catalogue.h"


class DataManager : public QObject
{
  Q_OBJECT
public:
  explicit DataManager(QObject *parent = nullptr);

  void setCatalogue(Catalogue* catalogue);

public slots:
  void applyCountriesFilter(const QStringList& filter);

 private:
  db::Catalogue _db;

  void collectAllFilters();

  std::set<std::string> _countries;
  std::set<std::string> _years;

  Catalogue* _catalogue;

signals:

};

