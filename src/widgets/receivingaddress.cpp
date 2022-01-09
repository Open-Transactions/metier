// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "receivingaddress.hpp"  // IWYU pragma: associated

#include <QAction>  // IWYU pragma: keep
#include <QApplication>
#include <QClipboard>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QToolButton>
#include <string>

#include "qrtoolbutton.hpp"
#include "ui_receivingaddress.h"
#include "util/resizer.hpp"

namespace metier::widget
{
ReceivingAddress::ReceivingAddress(QWidget* parent, QString address)
    : QDialog(parent)
    , ui_(std::make_unique<Ui::receivingAddress>())
{
    ui_->setupUi(this);
    installEventFilter(this);
    auto* text = ui_->addressText;
    auto* image = ui_->qrCode;
    auto* button = ui_->copyAddress;
    util::set_minimum_size(*text, address.size(), 1);
    text->setText(address);
    image->setString(address);
    connect(
        button, &QToolButton::triggered, this, &ReceivingAddress::copy_text);
    connect(image, &QToolButton::triggered, this, &ReceivingAddress::copy_text);
}

auto ReceivingAddress::eventFilter(QObject* obj, QEvent* event) -> bool
{
    if (event->type() == QEvent::KeyPress) {
        auto* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Escape) {
            close();

            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}

auto ReceivingAddress::copy_text() -> void
{
    auto* clipboard = QApplication::clipboard();

    if (nullptr == clipboard) { return; }

    clipboard->setText(ui_->addressText->text());
    QMessageBox::information(
        this,
        tr(QApplication::applicationName().toStdString().c_str()),
        QString("%1:<br/>%2")
            .arg(tr("Copied receiving address to the clipboard"))
            .arg(ui_->addressText->text()));
}

ReceivingAddress::~ReceivingAddress() = default;
}  // namespace metier::widget
