// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "firstrun.hpp"  // IWYU pragma: associated

#include <QPushButton>

#include "ui_firstrun.h"

namespace metier::widget
{
FirstRun::FirstRun(QObject* parent) noexcept
    : QDialog(nullptr)
    , ui(std::make_unique<Ui::FirstRunDialog>())
{
    moveToThread(parent->thread());
    ui->setupUi(this);
    connect(ui->create, &QPushButton::clicked, this, &FirstRun::create);
    connect(ui->restore, &QPushButton::clicked, this, &FirstRun::recover);
    connect(this, &FirstRun::wantNew, this, &QDialog::hide);
    connect(this, &FirstRun::wantOld, this, &QDialog::hide);
}

auto FirstRun::create() -> void { emit wantNew(); }

auto FirstRun::recover() -> void { emit wantOld(); }

FirstRun::~FirstRun() = default;
}  // namespace metier::widget
