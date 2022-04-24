#include "delegates.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>

#include "adminmodel.h"

WishlistDelegate::WishlistDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *WishlistDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &) const {
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

CommentsDelegate::CommentsDelegate(QObject *parent) : QItemDelegate(parent) {}

QWidget *CommentsDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &) const {
  return new QLineEdit(parent);
}

void CommentsDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const {
  const auto &comments = index.model()->data(index, Qt::EditRole).toString();
  auto lineedit = qobject_cast<QLineEdit *>(editor);
  lineedit->setText(comments);
}

void CommentsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const {
  auto lineedit = qobject_cast<QLineEdit *>(editor);
  const auto &comment = lineedit->text() == "-" ? "" : lineedit->text();
  model->setData(index, comment, Qt::EditRole);
}

void CommentsDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &) const {
  editor->setGeometry(option.rect);
}

ImageActionDelegate::ImageActionDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *ImageActionDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &,
                                           const QModelIndex &index) const {
  const auto &action = index.model()->data(index, Qt::UserRole).toString();
  if (!action.isEmpty()) {
    auto button = new QPushButton(parent);
    button->setMinimumWidth(100);
    return button;
  }
  return nullptr;
}

void ImageActionDelegate::setEditorData(QWidget *editor,
                                        const QModelIndex &index) const {
  const auto &action = index.model()->data(index, Qt::UserRole).toString();
  if (!action.isEmpty()) {
    auto button = qobject_cast<QPushButton *>(editor);
    button->setText(action);
  }
}

void ImageActionDelegate::setModelData(QWidget *, QAbstractItemModel *model,
                                       const QModelIndex &index) const {
  const auto &new_image = index.model()->data(index, Qt::EditRole).toString();
  model->setData(index, new_image, Qt::EditRole);
}

void ImageActionDelegate::updateEditorGeometry(
    QWidget *editor, const QStyleOptionViewItem &option,
    const QModelIndex &) const {
  editor->setGeometry(option.rect);
}
