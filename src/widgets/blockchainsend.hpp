// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QDialog>
#include <QObject>
#include <QString>
#include <memory>

namespace opentxs
{
namespace ui
{
class AccountActivityQt;
}  // namespace ui
}  // namespace opentxs

class QWidget;

namespace metier::widget
{
class BlockchainSend final : public QDialog
{
    Q_OBJECT

public:
    using Model = opentxs::ui::AccountActivityQt;

    BlockchainSend(QWidget* parent, Model* model);
    ~BlockchainSend() final;

private Q_SLOTS:
    void finished();
    void updateAddress(const QString&);
    void updateAmount(int);
    void updateSendResult(int, int, QString);
    void verify();

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
