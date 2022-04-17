#include "delegates.h"

#include <QCheckBox>
#include <QLineEdit>

#include "adminmodel.h"

WishlistDelegate::WishlistDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *WishlistDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem & /*option*/,
                                        const QModelIndex & /*index*/) const {
  return new QCheckBox(parent);
}

void WishlistDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const {

  const bool wishlisted = index.model()->data(index, Qt::EditRole).toBool();
  auto checkbox = qobject_cast<QCheckBox *>(editor);
  checkbox->setChecked(wishlisted);
}

void WishlistDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const {

  auto checkbox = qobject_cast<QCheckBox *>(editor);
  model->setData(index, checkbox->isChecked(), Qt::EditRole);
}

void WishlistDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &) const {
  editor->setGeometry(option.rect);
}

CommenstDelegate::CommenstDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *CommenstDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
  return new QLineEdit(parent);
}

void CommenstDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const {
  const auto &comments = index.model()->data(index, Qt::EditRole).toString();
  auto lineedit = qobject_cast<QLineEdit *>(editor);
  lineedit->setText(comments);
}

void CommenstDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const {
  auto lineedit = qobject_cast<QLineEdit *>(editor);
  model->setData(index, lineedit->text(), Qt::EditRole);
}

void CommenstDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
  editor->setGeometry(option.rect);
}
