// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <opentxs/opentxs.hpp>
#include <QDialog>
#include <memory>

class QWidget;

namespace metier::widget
{
class EnterPassphrase : public QDialog
{
    Q_OBJECT

public:
    EnterPassphrase(
        QWidget* parent,
        const QString& displayString,
        bool runOneOrTwo = true);
    ~EnterPassphrase() override;

    auto secret(opentxs::Secret& theSecret, QString& qstrSecret) -> bool;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    EnterPassphrase(const EnterPassphrase&) = delete;
    EnterPassphrase(EnterPassphrase&&) = delete;
    EnterPassphrase& operator=(const EnterPassphrase&) = delete;
    EnterPassphrase& operator=(EnterPassphrase&&) = delete;
};
}  // namespace metier::widget
