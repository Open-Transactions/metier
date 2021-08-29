// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "accountstatus.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QPushButton>

#include "otwrap.hpp"
#include "ui_accountstatus.h"
#include "util/resizer.hpp"

namespace metier::widget
{
struct AccountStatus::Imp {
    QDialog* parent_;
    std::unique_ptr<Ui::accountStatus> ui_;

    Imp(QDialog* parent, OTWrap& ot, int chain) noexcept
        : parent_(parent)
        , ui_(std::make_unique<Ui::accountStatus>())
    {
        ui_->setupUi(parent_);
        connect(
            ui_->buttons->button(QDialogButtonBox::Ok),
            &QPushButton::clicked,
            parent_,
            &QDialog::close);
        ui_->accountView->setModel(ot.accountStatusModel(chain));
        ui_->accountView->expandAll();
        util::set_minimum_size(*(ui_->accountView), 75, 16, {1, 1});
    }
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};

AccountStatus::AccountStatus(QWidget* parent, OTWrap& ot, int chain) noexcept
    : QDialog(parent)
    , imp_p_(std::make_unique<Imp>(this, ot, chain))
    , imp_(*imp_p_)
{
}

AccountStatus::~AccountStatus() = default;
}  // namespace metier::widget
