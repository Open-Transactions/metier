// Copyright (c) 2019-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/newseed.hpp"  // IWYU pragma: associated

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>

#include "api/api.hpp"
#include "models/seedlang.hpp"
#include "models/seedsize.hpp"
#include "models/seedtype.hpp"
#include "ui_newseed.h"
#include "util/resizer.hpp"
#include "util/scopeguard.hpp"

namespace metier::widget
{
struct NewSeed::Imp {
    Api& ot_;
    QDialog* parent_;
    std::unique_ptr<Ui::NewSeed> ui_;

    auto finish() -> void
    {
        parent_->hide();
        ot_.seedBackupFinished();
    }
    auto generateSeed() -> void
    {
        auto* top = ui_->preGenerate;
        auto* bottom = ui_->postGenerate;
        top->setHidden(true);
        const auto getData = [](const auto& control) -> auto {
            auto* model = control->model();

            return model
                ->data(model->index(control->currentIndex(), 0), Qt::UserRole)
                .toInt();
        };
        const auto words = ot_.createNewSeed(
            getData(ui_->type), getData(ui_->language), getData(ui_->strength));
        auto column{-1};
        auto row{0};
        auto counter{0};
        auto* layout = ui_->wordDisplay;

        for (const auto& word : words) {
            auto item = std::make_unique<QHBoxLayout>();
            auto label = std::make_unique<QLabel>();
            auto text = std::make_unique<QLineEdit>();
            auto postcondition = ScopeGuard{[&]() {
                item.release();
                label.release();
                text.release();
            }};

            {
                const auto index = std::to_string(++counter) + ':';
                label->setText(index.c_str());
                label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                util::set_minimum_size(*label, 3, 1, {3, 2});
            }
            {
                text->setText(word);
                text->setReadOnly(true);
                text->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                util::set_minimum_size(*text, ot_.longestSeedWord(), 1, {3, 2});
            }
            {
                item->addWidget(label.get());
                item->addWidget(text.get());
            }

            layout->addLayout(item.get(), row, ++column);

            if (2 == column) {
                ++row;
                column = -1;
            }
        }

        reflowDialog(row + 6);
        bottom->setHidden(false);
    }
    auto reflowDialog(const int lines) -> void
    {
        util::set_minimum_size(
            *parent_, 5 * ot_.longestSeedWord(), lines, {3, 2});
    }
    auto reflowOptions(const int style) -> void
    {
        {
            auto* size = ui_->strength;
            auto* model = ot_.seedSizeModel(style);
            util::set_minimum_size(
                *size, model->suggestedWidthInCharacters(), 1);
            size->setModel(model);
            size->setCurrentIndex(0);
        }
        {
            auto* lang = ui_->language;
            auto* model = ot_.seedLanguageModel(style);
            util::set_minimum_size(
                *lang, model->suggestedWidthInCharacters(), 1);
            lang->setModel(model);
            lang->setCurrentIndex(0);
        }
    }
    auto toggleAdvanced() -> void
    {
        auto* advanced = ui_->advanced;
        auto* options = ui_->options;
        const auto checked = advanced->isChecked();
        options->setVisible(advanced->isChecked());
        reflowDialog(checked ? 6 : 4);
    }

    Imp(QDialog* parent, Api& ot) noexcept
        : ot_(ot)
        , parent_(parent)
        , ui_(std::make_unique<Ui::NewSeed>())
    {
        ui_->setupUi(parent_);
        ui_->options->setHidden(true);
        ui_->postGenerate->setHidden(true);

        {
            auto* advanced = ui_->advanced;
            connect(advanced, &QCheckBox::stateChanged, [this]() {
                toggleAdvanced();
            });
        }
        {
            auto* type = ui_->type;
            auto* model = ot_.seedTypeModel();
            util::set_minimum_size(
                *type, model->suggestedWidthInCharacters(), 1);
            type->setModel(model);
            type->setCurrentIndex(0);
            reflowOptions(
                model->data(model->index(0, 0), Qt::UserRole).toInt());
            connect(
                type,
                QOverload<int>::of(&QComboBox::activated),
                [this, model](auto row) -> void {
                    reflowOptions(
                        model->data(model->index(row, 0), Qt::UserRole)
                            .toInt());
                });
        }
        {
            auto* generate = ui_->generateButtons->button(QDialogButtonBox::Ok);
            generate->setText(tr("Generate"));
            connect(
                generate, &QPushButton::clicked, [this]() { generateSeed(); });
        }

        {
            auto* done = ui_->completionButtons->button(QDialogButtonBox::Ok);
            done->setText(tr("Done"));
            connect(done, &QPushButton::clicked, [this]() { finish(); });
        }

        reflowDialog(4);
    }
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};
}  // namespace metier::widget
