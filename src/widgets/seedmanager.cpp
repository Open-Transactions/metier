// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "seedmanager.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QPushButton>

#include "otwrap.hpp"
#include "ui_seedmanager.h"
#include "util/resizer.hpp"

namespace metier::widget
{
struct SeedManager::Imp {
    QDialog* parent_;
    std::unique_ptr<Ui::seedManager> ui_;

    Imp(QDialog* parent, OTWrap& ot) noexcept
        : parent_(parent)
        , ui_(std::make_unique<Ui::seedManager>())
    {
        ui_->setupUi(parent_);
        connect(
            ui_->buttons->button(QDialogButtonBox::Ok),
            &QPushButton::clicked,
            parent_,
            &QDialog::close);
        ui_->seedView->setModel(ot.seedManager());
        ui_->seedView->expandAll();
        util::set_minimum_size(*(ui_->seedView), 75, 16, {1, 1});
    }
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};

SeedManager::SeedManager(QWidget* parent, OTWrap& ot) noexcept
    : QDialog(parent)
    , imp_p_(std::make_unique<Imp>(this, ot))
    , imp_(*imp_p_)
{
}

SeedManager::~SeedManager() = default;
}  // namespace metier::widget
