// Copyright (c) 2019-2024 The Open-Transactions developers
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
class EnterPassphrase final : public QDialog
{
    Q_OBJECT

public:
    struct Imp;

    enum class Mode : bool {
        Once = true,
        Twice = false,
    };

    auto secret() const noexcept -> QString;

    EnterPassphrase(const QString& displayString, Mode mode) noexcept;

    ~EnterPassphrase() final;

private Q_SLOTS:
    void cancel();
    void check_one();
    void check_two();

private:
    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    EnterPassphrase(const EnterPassphrase&) = delete;
    EnterPassphrase(EnterPassphrase&&) = delete;
    EnterPassphrase& operator=(const EnterPassphrase&) = delete;
    EnterPassphrase& operator=(EnterPassphrase&&) = delete;
};
}  // namespace metier::widget
