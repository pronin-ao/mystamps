#include "admin.h"
#include "ui_admin.h"

#include <QCompleter>
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QUrl>

#include "urlimagelabel.h"

#include <QDebug>

namespace {
static const QString kLink = "<a href=%2>%1<\\a>";
}

Admin::Admin(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminGui), network(this), proxyModel(this) {
  ui->setupUi(this);
  ui->update->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

  proxyModel.setSourceModel(&sourceModel);
  proxyModel.setFilterKeyColumn(0);
  ui->tableView->setModel(&proxyModel);
  ui->tableView->setSortingEnabled(true);
  ui->tableView->setItemDelegateForColumn(admin::indexes::kForcedWishlist,
                                          &wishListDelegate);
  ui->tableView->setItemDelegateForColumn(admin::indexes::kComments,
                                          &commentsDelegate);
  ui->tableView->setItemDelegateForColumn(admin::indexes::kCustomImageAction,
                                          &imageActionDelegate);
  ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

  connect(ui->apply, &QPushButton::clicked, this, &Admin::applyFilter);
  connect(&sourceModel, &AdminModel::customDataChanged, this,
          &Admin::customDataChanged);
  connect(&sourceModel, &AdminModel::imageDataChanged, this,
          &Admin::imageDataChanged);

  connect(ui->update, &QPushButton::clicked,
          [table = ui->tableView, admin = this]() {
            table->reset();
            admin->CreateItemWidgets();
          });
}

Admin::~Admin() { delete ui; }

void Admin::setDbPointerToModel(db::Catalogue *db) {
  sourceModel.setDbPointer(db);
  CreateItemWidgets();
  ui->tableView->setVisible(false);
  ui->tableView->resizeColumnsToContents();
  ui->tableView->setVisible(true);

  const auto completer = new QCompleter(sourceModel.countryList(), this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->setMaxVisibleItems(20);
  ui->country->setCompleter(completer);
}

void Admin::applyFilter() {
  const auto &country = ui->country->text();
  const auto &year = ui->year->text();
  proxyModel.setFilterData(country, year);
  CreateItemWidgets();
}

void Admin::CreateItemWidgets() {
  for (int i = 0; i < proxyModel.rowCount({}); ++i) {
    {
      const auto &cur = proxyModel.index(i, admin::indexes::kSwLink, {});
      const auto &link = proxyModel.data(cur, Qt::UserRole).toString();
      if (!ui->tableView->indexWidget(cur))
        ui->tableView->setIndexWidget(cur, new QLabel{});
      const auto label =
          qobject_cast<QLabel *>(ui->tableView->indexWidget(cur));
      label->setTextFormat(Qt::RichText);
      label->setText(kLink.arg("Stampworld link", link));
      label->setOpenExternalLinks(true);
    }
    {
      const auto &cur = proxyModel.index(i, admin::indexes::kImage, {});
      const auto &image = proxyModel.data(cur, Qt::UserRole).toString();
      if (!ui->tableView->indexWidget(cur))
        ui->tableView->setIndexWidget(cur, new UrlImageLabel{image, &network});
    }
  }
  connect(&proxyModel, &AdminModel::dataChanged,
          [&](const auto &index1, const auto &index2, const auto &) {
            for (int i = index1.row(); i <= index2.row(); ++i) {
              const auto &cur = proxyModel.index(i, admin::indexes::kImage, {});
              const auto label = qobject_cast<UrlImageLabel *>(
                  ui->tableView->indexWidget(cur));
              label->setUrl(proxyModel.data(cur, Qt::UserRole).toString());
            }
          });
}
