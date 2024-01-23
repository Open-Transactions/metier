// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QMetaObject>
#include <memory>

class QObject;

namespace metier
{
namespace common
{
class Api;
}  // namespace common
}  // namespace metier
namespace Ui
{
class BlockchainChooser;
}

class QPushButton;

namespace metier::skins::metier::widget
{
class BlockchainChooser final : public QDialog
{
    Q_OBJECT

public:
    auto Ok() noexcept -> QPushButton*;

    BlockchainChooser(QObject* parent, common::Api& ot);

    ~BlockchainChooser() final;

private:
    common::Api& ot_;
    const std::unique_ptr<Ui::BlockchainChooser> ui_;

    auto check(int enabledChains) noexcept -> void;
    auto init() noexcept -> void;
    auto model() noexcept -> void;
};
}  // namespace metier::skins::metier::widget
