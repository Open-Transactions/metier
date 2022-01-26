// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

class QWidget;

namespace metier
{
class Api;
}  // namespace metier

namespace Ui
{
class BlockchainChooser;
}

class QPushButton;

namespace metier::widget
{
class BlockchainChooser final : public QDialog
{
    Q_OBJECT

public:
    auto Ok() noexcept -> QPushButton*;

    BlockchainChooser(QObject* parent, Api& ot);

    ~BlockchainChooser() final;

private:
    Api& ot_;
    const std::unique_ptr<Ui::BlockchainChooser> ui_;

    auto check(int enabledChains) noexcept -> void;
    auto init() noexcept -> void;
};
}  // namespace metier::widget
