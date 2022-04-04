#include <QApplication>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QSslSocket>
#include <QtGlobal>
#include <QtWebView>

#include "catalogue.h"
#include "datamanager.h"

#if defined(Q_OS_LINUX)

#endif

int main(int argc, char *argv[]) {
  qputenv("ANDROID_OPENSSL_SUFFIX", "_1_1");
  qInfo() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();

  QApplication a(argc, argv);

  QtWebView::initialize();

  try {
    DataManager db{};
    QQmlApplicationEngine engine(":/main.qml");
    qDebug() << "qml app started";
    const auto &roots = engine.rootObjects();
    if (roots.empty() || roots.at(0)->children().empty())
      throw std::runtime_error("Empty QML root object");
    Catalogue *catalogue =
        roots.at(0)->children().at(0)->findChild<Catalogue *>();
    if (catalogue == nullptr)
      throw std::runtime_error("Null qml catalogue pointer");
    qDebug() << "about to set";

    // catalogue --> db
    QObject::connect(catalogue, &Catalogue::sendCountriesFilter, &db,
                     &DataManager::applyCountriesFilter);
    QObject::connect(catalogue, &Catalogue::sendYearsFilter, &db,
                     &DataManager::applyYearsFilter);
    QObject::connect(catalogue, &Catalogue::sendPriceFilter, &db,
                     &DataManager::applyPriceFilter);
    QObject::connect(catalogue, &Catalogue::stampChanges, &db,
                     &DataManager::registerStampChecked);
    QObject::connect(catalogue, &Catalogue::sendShowMode, &db,
                     &DataManager::applyShowMode);

    void (Catalogue::*setCountries_rval)(QSharedPointer<QStringList>) =
        &Catalogue::setCountries;
    void (Catalogue::*setYears_rval)(QSharedPointer<QStringList>) =
        &Catalogue::setYears;
    void (Catalogue::*setPrices_rval)(QSharedPointer<QStringList>) =
        &Catalogue::setPrices;

    // db --> catalogue
    QObject::connect(&db, &DataManager::sendCountriesFilter, catalogue,
                     &Catalogue::setCountriesFilter);
    QObject::connect(&db, &DataManager::sendCountries, catalogue,
                     setCountries_rval);
    QObject::connect(&db, &DataManager::sendYearsFilter, catalogue,
                     &Catalogue::setYearsFilter);
    QObject::connect(&db, &DataManager::sendYears, catalogue, setYears_rval);
    QObject::connect(&db, &DataManager::sendPricesFilter, catalogue,
                     &Catalogue::setPriceFilter);
    QObject::connect(&db, &DataManager::sendPrices, catalogue, setPrices_rval);

    // set data to catalogue (once)
    QObject::connect(&db, &DataManager::sendStamps, catalogue,
                     &Catalogue::setStamps);

    db.initiateDataForQml();
    return a.exec();
  } catch (const std::exception &ex) {
    QMessageBox::critical(nullptr, "Error loading DB",
                          QString::fromStdString(ex.what()));
    return 0;
  }
}
