// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

namespace metier
{
namespace model
{
class AccountActivity;
}  // namespace model

}  // namespace metier

class QWidget;

namespace metier::widget
{
class BlockchainSend final : public QDialog
{
    Q_OBJECT

public:
    BlockchainSend(QWidget* parent, model::AccountActivity* model);
    ~BlockchainSend() final;

private slots:
    void updateAddress(const QString&);
    void updateAmount(int);

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    BlockchainSend(const BlockchainSend&) = delete;
    BlockchainSend(BlockchainSend&&) = delete;
    BlockchainSend& operator=(const BlockchainSend&) = delete;
    BlockchainSend& operator=(BlockchainSend&&) = delete;
};
}  // namespace metier::widget
