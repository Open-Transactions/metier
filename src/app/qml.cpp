// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imp.hpp"  // IWYU pragma: associated

#include <opentxs/core/ui/qt/SeedValidator.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QStringLiteral>
#include <QUrl>
#include <atomic>
#include <future>

#include "models/accountlist.hpp"
#include "models/blockchainchooser.hpp"
#include "models/seedlang.hpp"
#include "models/seedsize.hpp"
#include "models/seedtype.hpp"
#include "otwrap.hpp"
#include "qml.hpp"
#include "util/claim.hpp"

namespace metier
{
struct QmlApp final : public App::Imp, public QGuiApplication {
private:
    std::promise<void> model_promise_;

public:
    static std::unique_ptr<App> singleton_;

    App& parent_;
    std::unique_ptr<OTWrap> ot_;
    QQuickView qml_;
    QmlInterface interface_;
    std::atomic<bool> model_init_;
    std::shared_future<void> models_set_;

    auto displayBlockchainChooser() -> void final
    {
        // NOTE when the app.displayBlockchainChooser signal is received the
        // user must enable at least one blockchain Use
        // otwrap.BlockchainChooserModel to populate a control which allows the
        // user to enable a blockchain Do not allow the user to proceed until
        // the otwrap.chainsChanged signal has been received with a value
        // greater than zero. Call otwrap.checkStartupConditions() once the user
        // has selected at least one blockchain and is ready to move on.
        models_set_.get();
        interface_.doDisplayBlockchainChooser();
    }

    auto displayFirstRun() -> void final
    {
        // NOTE when the app.displayFirstRun signal is received the user must
        // choose to either create a new seed or recover an existing seed.
        //
        // If the user wants to create a new seed, call otwrap.createNewSeed()
        // then display the words for the user to write down. Once the user is
        // ready to move on, call otwrap.checkStartupConditions()
        //
        // If the user wants to import an existing seed, collect his input then
        // call otwrap.importSeed()
        interface_.doDisplayFirstRun();
    }

    auto displayMainWindow() -> void final
    {
        // NOTE once the app.displayMainWindow signal is received the startup
        // process is complete. Display the main screen.
        models_set_.get();
        interface_.doDisplayMainWindow();
    }

    auto displayNamePrompt() -> void final
    {
        // NOTE when the app.displayNamePrompt signal is received the user must
        // provide a profile name. call otwrap.createNym() with the name the
        // user provides
        interface_.doDisplayNamePrompt();
    }

    auto displayPasswordPrompt(QString, bool) -> void final
    {
        // TODO
    }

    auto confirmPassword(
        [[maybe_unused]] QString prompt,
        [[maybe_unused]] QString key) -> QString final
    {
        return "opentxs";  // TODO
    }

    auto getPassword(
        [[maybe_unused]] QString prompt,
        [[maybe_unused]] QString key) -> QString final
    {
        return "opentxs";  // TODO
    }

    auto init(int& argc, char** argv) noexcept -> void final
    {
        ot_ = std::make_unique<OTWrap>(*this, parent_, argc, argv);

        {
            auto* ot = ot_.get();
            ot->connect(ot, &OTWrap::nymReady, this, &QmlApp::nymReady);
            Ownership::Claim(ot);
            qml_.rootContext()->setContextProperty("otwrap", ot);
        }

        qml_.connect(
            qml_.engine(), &QQmlEngine::quit, this, &QCoreApplication::quit);
        qml_.setSource(QUrl("qrc:/main.qml"));

        if (qml_.status() == QQuickView::Error) { abort(); }

        qml_.setResizeMode(QQuickView::SizeRootObjectToView);
        qml_.show();
    }

    auto run() -> int final
    {
        setWindowIcon(QIcon(":/qwidget/assets/app_icon.png"));

        return exec();
    }

    auto otwrap() noexcept -> OTWrap* final { return ot_.get(); }

    QmlApp(App& parent, int& argc, char** argv) noexcept
        : QGuiApplication(argc, argv)
        , model_promise_()
        , parent_(parent)
        , ot_()
        , qml_()
        , interface_()
        , model_init_(false)
        , models_set_(model_promise_.get_future())
    {
        {
            auto* app = &interface_;
            Ownership::Claim(app);
            qml_.rootContext()->setContextProperty("app", app);
        }
    }

    ~QmlApp() final = default;

private:
    auto nymReady() noexcept -> void
    {
        const auto init = model_init_.exchange(true);

        if (init) { return; }

        model_promise_.set_value();
    }
};

auto App::Imp::factory_qml(App& parent, int& argc, char** argv) noexcept
    -> std::unique_ptr<Imp>
{
    return std::make_unique<QmlApp>(parent, argc, argv);
}

auto QmlInterface::doDisplayBlockchainChooser() -> void
{
    emit displayBlockchainChooser();
}

auto QmlInterface::doDisplayFirstRun() -> void { emit displayFirstRun(); }

auto QmlInterface::doDisplayMainWindow() -> void { emit displayMainWindow(); }

auto QmlInterface::doDisplayNamePrompt() -> void { emit displayNamePrompt(); }

QmlInterface::~QmlInterface() = default;
}  // namespace metier
