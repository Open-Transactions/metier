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
class OTWrap;
}  // namespace metier

namespace metier::widget
{
class ShowSeed final : public QDialog
{
    Q_OBJECT
public:
    ShowSeed(QWidget* parent, OTWrap& ot) noexcept;

    ~ShowSeed() final;

private:
    struct Imp;

    std::unique_ptr<Imp> imp_p_;
    Imp& imp_;

    ShowSeed(const ShowSeed&) = delete;
    ShowSeed(ShowSeed&&) = delete;
    ShowSeed& operator=(const ShowSeed&) = delete;
    ShowSeed& operator=(ShowSeed&&) = delete;
};
}  // namespace metier::widget
