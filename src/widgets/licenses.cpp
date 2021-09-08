// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "licenses.hpp"  // IWYU pragma: associated

#include <QFont>
#include <QItemSelectionModel>
#include <QList>
#include <QListView>
#include <QPlainTextEdit>
#include <QVariant>

#include "models/licenses.hpp"
#include "ui_licenses.h"
#include "util/resizer.hpp"

namespace metier::widget
{
Licenses::Licenses(QWidget* parent)
    : QDialog(parent)
    , model_(std::make_unique<model::Licenses>(parent))
    , ui_(std::make_unique<Ui::licenseViewer>())
{
    ui_->setupUi(this);

    {
        auto& left = *ui_->componentSelector;
        left.setModel(model_.get());
        auto& selection = *left.selectionModel();
        connect(
            &selection,
            &QItemSelectionModel::selectionChanged,
            this,
            &Licenses::component_changed);
        const auto index = model_->index(0, 0);
        selection.select(index, QItemSelectionModel::Select);
        component_changed(selection.selection(), {});
        util::set_minimum_size(
            left,
            model_->suggestedWidthInCharacters() + 1,
            model_->rowCount({}),
            {3, 2});
    }

    {
        auto& right = *ui_->licenseText;
        auto font =
            QFont(QFontDatabase::systemFont(QFontDatabase::FixedFont).family());
        right.setFont(font);
        util::set_minimum_size(right, 83, 25);
    }
}

auto Licenses::component_changed(
    const QItemSelection& current,
    [[maybe_unused]] const QItemSelection& old) -> void
{
    const auto indices = current.indexes();
    auto& right = *ui_->licenseText;

    if (0 == indices.size()) {
        right.setPlainText({});
    } else {
        const auto& index = indices.first();
        auto text = model_->data(index, Qt::UserRole);
        right.setPlainText(text.toString());
    }
}

Licenses::~Licenses() = default;
}  // namespace metier::widget
