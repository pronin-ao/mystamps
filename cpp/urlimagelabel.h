#ifndef URLIMAGELABEL_H
#define URLIMAGELABEL_H

#include <QLabel>
#include <QNetworkAccessManager>

class UrlImageLabel : public QLabel {
  Q_OBJECT
public:
  UrlImageLabel(const QString &url, QNetworkAccessManager *network,
                QWidget *parent = nullptr);

private:
  QNetworkAccessManager *_network;
  QLabel *_bigPicture = nullptr;

protected:
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;

private slots:
  void imageLoaded();
};

#endif // URLIMAGELABEL_H
