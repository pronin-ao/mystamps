#pragma once

#include "dbmodel.h"
#include <QString>

db::Catalogue ParseCatalogue(const QString &str);

QString SerializeUserData(const db::Catalogue &db);

void ParseUserData(const QString &str, db::Catalogue &data);
