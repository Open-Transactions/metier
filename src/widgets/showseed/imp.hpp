// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/showseed.hpp"  // IWYU pragma: associated

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <sstream>

#include "api/api.hpp"
#include "models/seedlang.hpp"
#include "models/seedsize.hpp"
#include "models/seedtype.hpp"
#include "ui_showseed.h"
#include "util/resizer.hpp"
#include "util/scopeguard.hpp"

namespace metier::widget
{
struct ShowSeed::Imp {
    Api& ot_;
    QDialog* parent_;
    std::unique_ptr<Ui::ShowSeed> ui_;

    auto reflowDialog(const int lines) -> void
    {
        util::set_minimum_size(
            *parent_, 5 * ot_.longestSeedWord(), lines, {3, 2});
    }

    Imp(QDialog* parent, Api& ot) noexcept
        : ot_(ot)
        , parent_(parent)
        , ui_(std::make_unique<Ui::ShowSeed>())
    {
        ui_->setupUi(parent_);
        connect(
            ui_->completionButtons->button(QDialogButtonBox::Ok),
            &QPushButton::clicked,
            parent_,
            &QDialog::close);
        auto text = std::stringstream{};
        auto counter{-1};

        for (const auto& word : ot_.getRecoveryWords()) {
            text << word.toStdString();
            const auto col = (++counter) % 3;

            switch (col) {
                case 2: {
                    text << '\n';
                } break;
                case 0:
                case 1:
                default: {
                    text << ' ';
                }
            }
        }

        auto* viewer = ui_->words;
        viewer->setAlignment(Qt::AlignCenter);
        viewer->setText(text.str().c_str());
        reflowDialog(3);
    }
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};
}  // namespace metier::widget
