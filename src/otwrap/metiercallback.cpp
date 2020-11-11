// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <opentxs/opentxs.hpp>

#include "otwrap/metiercallback.hpp"
#include "util/scopeguard.hpp"
#include "widgets/enterpassphrase.hpp"

#include <QString>
#include <QEventLoop>

MetierCallback::MetierCallback()
    : opentxs::OTCallback()
{
    
}

auto MetierCallback::Store([[maybe_unused]] const std::string& key, const QString& pass) const -> bool
{
//    WritePasswordJob job( QLatin1String("metier") );
//    const QString qstrKey = QString::fromStdString(key);
//    job.setAutoDelete( false );
//    job.setKey( qstrKey );
//    job.setTextData( pass );
//    QEventLoop loop;
//    job.connect( &job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
//    job.start();
//    loop.exec();
//    if ( job.error() ) {
//        std::cerr << "Storing password failed: " << qPrintable(job.errorString()) << std::endl;
//        return false;
//    }
//    std::cout << "Password stored successfully" << std::endl;
    return true;
}

auto MetierCallback::Retrieve([[maybe_unused]] const std::string& key, QString& pass) const -> bool
{
    return false;
//    ReadPasswordJob job( QLatin1String("metier") );
//    const QString qstrKey = QString::fromStdString(key);
//    job.setAutoDelete( false );
//    job.setKey( qstrKey );

//    QEventLoop loop;
//    job.connect( &job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()) );
//    job.start();
//    loop.exec();

//    pass = job.textData();
//    if ( job.error() ) {
//        std::cerr << "Restoring password failed: " << qPrintable(job.errorString()) << std::endl;
//        return false;
//    }
//    std::cout << qPrintable(pass) << std::endl;
//    return true;
}

void MetierCallback::runOne(
    const char* szDisplay,
    opentxs::Secret& theOutput,
    const std::string& key) const
{
    QString qstrOutput;
    if (Retrieve(key, qstrOutput)) {
        const std::string passphrase = qstrOutput.toStdString();
        theOutput.Assign(passphrase.c_str(), passphrase.size());
    }
    else {
        const QString display = QString::fromStdString(std::string(szDisplay));

        auto dialog = std::make_unique<metier::widget::EnterPassphrase>(nullptr, display, true);
        auto postcondition = metier::ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
        if (dialog->secret(theOutput, qstrOutput)) {
            Store(key, qstrOutput);
        }
    }
}

void MetierCallback::runTwo(
    const char* szDisplay,
    opentxs::Secret& theOutput,
    const std::string& key) const
{
    const QString display = QString::fromStdString(std::string(szDisplay));
    auto dialog = std::make_unique<metier::widget::EnterPassphrase>(nullptr, display, false);
    auto postcondition = metier::ScopeGuard{[&dialog]() {
        dialog->deleteLater();
        dialog.release();
    }};
    dialog->exec();
    QString qstrOutput;
    if (dialog->secret(theOutput, qstrOutput)) {
        Store(key, qstrOutput);
    }
}
