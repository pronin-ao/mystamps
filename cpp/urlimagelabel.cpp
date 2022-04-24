#include "urlimagelabel.h"

#include <QNetworkReply>

UrlImageLabel::UrlImageLabel(const QString &url, QNetworkAccessManager *network,
                             QWidget *parent)
    : QLabel(parent), _network(network) {
  setUrl(url);
}

void UrlImageLabel::setUrl(const QString &url) {
  if (_url == url) {
    qDebug() << url;
    return;
  }
  _url = url;
  if (_response) {
    _response->close();
    _response->disconnect(this);
  }
  _response = _network->get(QNetworkRequest(QUrl(url)));
  connect(_response, &QNetworkReply::finished, this,
          &UrlImageLabel::imageLoaded);
}

void UrlImageLabel::enterEvent(QEvent *) {
  if (!_bigPicture && !this->pixmap(Qt::ReturnByValue).isNull()) {
    _bigPicture = new QLabel();
    _bigPicture->setPixmap(this->pixmap(Qt::ReturnByValue));
    _bigPicture->show();
  }
}

void UrlImageLabel::leaveEvent(QEvent *) {
  if (_bigPicture) {
    _bigPicture->hide();
    _bigPicture->deleteLater();
    _bigPicture = nullptr;
  }
}

void UrlImageLabel::imageLoaded() {
  if (_response == sender())
    _response = nullptr;
  QByteArray jpegData = qobject_cast<QNetworkReply *>(sender())->readAll();
  sender()->deleteLater();
  QPixmap pixmap;
  pixmap.loadFromData(jpegData);
  setPixmap(pixmap);
}
