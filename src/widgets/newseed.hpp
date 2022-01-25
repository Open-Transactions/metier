// Copyright (c) 2019-2022 The Open-Transactions developers
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
class Api;
}  // namespace metier

namespace metier::widget
{
class NewSeed final : public QDialog
{
    Q_OBJECT
public:
    NewSeed(QObject* parent, Api& ot) noexcept;

    ~NewSeed() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    NewSeed(const NewSeed&) = delete;
    NewSeed(NewSeed&&) = delete;
    NewSeed& operator=(const NewSeed&) = delete;
    NewSeed& operator=(NewSeed&&) = delete;
};
}  // namespace metier::widget
