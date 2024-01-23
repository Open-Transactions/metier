// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

class QEvent;
class QWidget;

namespace Ui
{
class receivingAddress;
}

namespace metier::skins::metier::widget
{
class ReceivingAddress final : public QDialog
{
    Q_OBJECT

public:
    void dialog();

    ReceivingAddress(QWidget* parent, QString address);
    ~ReceivingAddress() final;

private:
    std::unique_ptr<Ui::receivingAddress> ui_;

    auto copy_text() -> void;
    auto eventFilter(QObject* obj, QEvent* event) -> bool final;
};
}  // namespace metier::skins::metier::widget
