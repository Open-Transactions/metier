// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

class QWidget;

namespace Ui
{
class FirstRunDialog;
}

namespace metier::widget
{
class FirstRun final : public QDialog
{
    Q_OBJECT

Q_SIGNALS:
    void wantNew();
    void wantOld();

public:
    FirstRun(QObject* parent) noexcept;

    ~FirstRun() final;

private:
    std::unique_ptr<Ui::FirstRunDialog> ui;

    void create();
    void recover();

    FirstRun(const FirstRun&) = delete;
    FirstRun(FirstRun&&) = delete;
    FirstRun& operator=(const FirstRun&) = delete;
    FirstRun& operator=(FirstRun&&) = delete;
};
}  // namespace metier::widget
