// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "widgets/recoverwallet.hpp"  // IWYU pragma: associated

#include <opentxs/interface/qt/SeedValidator.hpp>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <iostream>
#include <mutex>
#include <set>
#include <vector>

#include "api/api.hpp"
#include "models/seedlang.hpp"
#include "models/seedsize.hpp"
#include "models/seedtype.hpp"
#include "ui_recoverwallet.h"
#include "util/resizer.hpp"
#include "util/scopeguard.hpp"

namespace metier::widget
{
struct RecoverWallet::Imp {
    Api& ot_;
    RecoverWallet* parent_;
    mutable std::mutex lock_;
    std::unique_ptr<Ui::RestoreSeed> ui_;
    std::set<QWidget*> labels_;
    std::vector<QLineEdit*> texts_;
    std::set<QLayout*> layouts_;

    template <typename Widget>
    static auto get_data(const Widget& control) noexcept
    {
        auto* model = control->model();

        return model
            ->data(model->index(control->currentIndex(), 0), Qt::UserRole)
            .toInt();
    }

    auto getLanguage() const noexcept { return get_data(ui_->language); }
    auto getStrength() const noexcept { return get_data(ui_->strength); }
    auto getStyle() const noexcept { return get_data(ui_->type); }

    auto cancel() -> void
    {
        parent_->hide();
        ot_.checkStartupConditions();
    }
    auto clearWords() -> void
    {
        auto* layout = ui_->wordsLayout;

        for (auto* widget : labels_) {
            layout->removeWidget(widget);
            widget->deleteLater();
        }

        for (auto* widget : texts_) {
            layout->removeWidget(widget);
            widget->deleteLater();
        }

        for (auto* box : layouts_) {
            layout->removeItem(box);
            box->deleteLater();
        }

        labels_.clear();
        texts_.clear();
        layouts_.clear();
    }
    auto createWords() -> void
    {
        const auto style = getStyle();
        const auto lang = getLanguage();
        const auto total = ot_.wordCount(style, getStrength());
        auto column{-1};
        auto row{0};

        for (auto counter{0}; counter < total; ++counter) {
            auto item = std::make_unique<QHBoxLayout>();
            auto label = std::make_unique<QLabel>();
            auto text = std::make_unique<QLineEdit>();
            auto postcondition = ScopeGuard{[&]() {
                item.release();
                label.release();
                text.release();
            }};
            labels_.emplace(label.get());
            texts_.emplace_back(text.get());
            layouts_.emplace(item.get());

            {
                const auto index = std::to_string(counter + 1) + ':';
                label->setText(index.c_str());
                label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                util::set_minimum_size(*label, 3, 1, {3, 2});
            }
            {
                text->setValidator(ot_.seedWordValidator(style, lang));
                text->setReadOnly(false);
                text->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                util::set_minimum_size(*text, ot_.longestSeedWord(), 1, {3, 2});
                connect(
                    text.get(),
                    &QLineEdit::textChanged,
                    [this, ptr = text.get()]() { validate(ptr); });
            }
            {
                item->addWidget(label.get());
                item->addWidget(text.get());
            }

            ui_->wordsLayout->addLayout(item.get(), row, ++column);

            if (2 == column) {
                ++row;
                column = -1;
            }
        }

        reflowDialog(5 + row);
    }
    auto finish() -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};
        auto words = QString{};

        assert(0 < texts_.size());

        for (auto* widget : texts_) {
            words += widget->text();
            words += ' ';
        }

        words.chop(1);
        ot_.importSeed(getStyle(), getLanguage(), words, ui_->password->text());
        parent_->hide();
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

        resetWords();
    }
    auto resetWords() -> void
    {
        {
            auto lock = std::lock_guard<std::mutex>{lock_};
            clearWords();
            createWords();
        }

        validate(nullptr);
    }
    auto validate(QLineEdit* ptr) -> void
    {
        auto lock = std::lock_guard<std::mutex>{lock_};

        if (nullptr != ptr) {
            static const auto good = [] {
                auto palette = QPalette{};
                palette.setColor(QPalette::Base, Qt::green);
                palette.setColor(QPalette::Text, Qt::black);

                return palette;
            }();
            static const auto bad = [] {
                auto palette = QPalette{};
                palette.setColor(QPalette::Base, Qt::yellow);
                palette.setColor(QPalette::Text, Qt::black);

                return palette;
            }();

            auto& text = *ptr;

            if (text.hasAcceptableInput()) {
                text.setPalette(good);
            } else {
                text.setPalette(bad);
            }
        }

        auto valid{true};

        for (auto* widget : texts_) {
            if (false == widget->hasAcceptableInput()) {
                valid = false;
                break;
            }
        }

        auto* ok = ui_->control->button(QDialogButtonBox::Ok);
        ok->setEnabled(valid);
    }

    Imp(RecoverWallet* parent, Api& ot) noexcept
        : ot_(ot)
        , parent_(parent)
        , lock_()
        , ui_(std::make_unique<Ui::RestoreSeed>())
        , labels_()
        , texts_()
        , layouts_()
    {
        texts_.reserve(24);
        ui_->setupUi(parent_);
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
            auto* strength = ui_->strength;
            connect(
                strength,
                QOverload<int>::of(&QComboBox::activated),
                [this]() -> void { resetWords(); });
        }
        {
            auto* lang = ui_->language;
            connect(
                lang,
                QOverload<int>::of(&QComboBox::activated),
                [this]() -> void { resetWords(); });
        }
        {
            auto* ok = ui_->control->button(QDialogButtonBox::Ok);
            connect(ok, &QPushButton::clicked, [this]() { finish(); });
        }
        {
            auto* cancel = ui_->control->button(QDialogButtonBox::Cancel);
            connect(
                cancel, &QPushButton::clicked, parent_, &RecoverWallet::cancel);
        }
    }
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;
};
}  // namespace metier::widget
