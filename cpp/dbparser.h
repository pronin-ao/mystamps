#pragma once

#include "dbmodel.h"
#include <QString>

db::Catalogue ParseStampworldCatalogue(const QString &str);

QString SerializeUserData(const db::Catalogue &db);
void ParseUserData(const QString &str, db::Catalogue &db);

QString SerializeCustomData(const db::Catalogue &db);
void ParseCustomData(const QString &str, db::Catalogue &db);
