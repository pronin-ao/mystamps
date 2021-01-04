#include "datamanager.h"

#include <QFile>

#include <QDebug>

#include "dbparser.h"

const QString Filename = ":/db/stamps";

QString ReadFile(){
  QString val;
  QFile file;
  file.setFileName(Filename);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  val = file.readAll();
  file.close();
  return val;
}


DataManager::DataManager(QObject *parent) : QObject(parent)
{
  const auto& json = ReadFile();
  _size = json.size();
  _db = ParseCatalogue(json);
  _size = _db.size();

}

