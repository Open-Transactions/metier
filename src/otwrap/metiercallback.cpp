// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <opentxs/opentxs.hpp>
#include <QEventLoop>
#include <QString>

#include "otwrap/metiercallback.hpp"
#include "util/scopeguard.hpp"
#include "widgets/enterpassphrase.hpp"

MetierCallback::MetierCallback()
    : opentxs::OTCallback()
{
}

void MetierCallback::runOne(
    const char* szDisplay,
    opentxs::Secret& theOutput,
    [[maybe_unused]] const std::string& key) const
{
    QString qstrOutput;
    const QString display = QString::fromStdString(std::string(szDisplay));

    auto dialog = std::make_unique<metier::widget::EnterPassphrase>(
        nullptr, display, true);
    auto postcondition = metier::ScopeGuard{[&dialog]() {
        dialog->deleteLater();
        dialog.release();
    }};
    dialog->exec();
    dialog->secret(theOutput, qstrOutput);
}

void MetierCallback::runTwo(
    const char* szDisplay,
    opentxs::Secret& theOutput,
    [[maybe_unused]] const std::string& key) const
{
    const QString display = QString::fromStdString(std::string(szDisplay));
    auto dialog = std::make_unique<metier::widget::EnterPassphrase>(
        nullptr, display, false);
    auto postcondition = metier::ScopeGuard{[&dialog]() {
        dialog->deleteLater();
        dialog.release();
    }};
    dialog->exec();
    QString qstrOutput;
    dialog->secret(theOutput, qstrOutput);
}
