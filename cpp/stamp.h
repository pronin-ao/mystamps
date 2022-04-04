#ifndef STAMP_H
#define STAMP_H

#include <QObject>
#include <QStringList>

class Stamp {
  Q_GADGET
  Q_PROPERTY(QString capture READ capture)
  Q_PROPERTY(QString image READ image)
  Q_PROPERTY(QString price READ price)
  Q_PROPERTY(QString year READ year)
  Q_PROPERTY(QString country READ country)
  Q_PROPERTY(QString spec READ spec)
  Q_PROPERTY(bool checked READ checked WRITE setChecked)
  Q_PROPERTY(QString id READ id)
  Q_PROPERTY(QString code READ code)
  Q_PROPERTY(QString color READ color)
  Q_PROPERTY(QString series READ series)
  Q_PROPERTY(bool owned READ owned)
  Q_PROPERTY(QStringList conditions READ conditions)
  Q_PROPERTY(QString sw_link READ sw_link)
  Q_PROPERTY(bool has_list READ has_list)
  Q_PROPERTY(QString list_note READ list_note)
  Q_PROPERTY(bool list_owned READ list_owned)
  Q_PROPERTY(QStringList list_conditions READ list_conditions)

public:
  inline auto capture() const { return _capture; }
  inline auto image() const { return _image; }
  inline auto price() const { return _price; }
  inline auto year() const { return _year; }
  inline auto country() const { return _country; }
  inline auto spec() const { return _spec; }
  inline auto checked() const { return _checked; }
  inline auto id() const { return _id; }
  inline auto code() const { return _code; }
  inline auto color() const { return _color; }
  inline auto series() const { return _series; }
  inline auto owned() const { return _owned; }
  inline auto conditions() const { return _conditions; }
  inline auto sw_link() const { return _sw_link; }
  inline auto has_list() const { return _has_list; }
  inline auto list_note() const { return _list_note; }
  inline auto list_owned() const { return _list_owned; }
  inline auto list_conditions() const { return _list_conditions; }

  void setChecked(const bool checked);

  QString _capture;
  QString _image;
  QString _price;
  QString _year;
  QString _country;
  QString _spec;
  bool _checked = false;
  QString _id;
  QString _code;
  QString _color;
  QString _series;
  bool _owned;
  QStringList _conditions;
  QString _sw_link;
  bool _has_list;
  QString _list_note;
  bool _list_owned;
  QStringList _list_conditions;
};

#endif // STAMP_H
