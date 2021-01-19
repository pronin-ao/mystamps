#pragma once

#include "dbmodel.h"
#include <QString>

db::Catalogue ParseCatalogue(const QString &str);


QString SerializeAddData(const db::Catalogue& db);

void ParseAddData(const QString& str, db::Catalogue& data);
