// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imp.hpp"  // IWYU pragma: associated

#include <otwrap.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringLiteral>
#include <QUrl>

namespace metier
{
struct QmlApp final : public App::Imp, public QGuiApplication {
    static std::unique_ptr<App> singleton_;

    App& parent_;
    std::unique_ptr<OTWrap> ot_;
    QQmlApplicationEngine qml_;

    auto displayBlockchainChooser() -> void final
    {
        // TODO
    }

    auto displayFirstRun() -> void final
    {
        // TODO
    }

    auto displayMainWindow() -> void final
    {
        // TODO
    }

    auto displayNamePrompt() -> void final
    {
        // TODO
    }

    auto run() -> int final { return exec(); }

    auto otwrap() noexcept -> OTWrap* final { return ot_.get(); }

    QmlApp(App& parent, int& argc, char** argv) noexcept
        : QGuiApplication(argc, argv)
        , parent_(parent)
        , ot_(std::make_unique<OTWrap>(*this))
        , qml_()
    {
        {
            auto* ot = ot_.get();
            QQmlEngine::setObjectOwnership(ot, QQmlEngine::CppOwnership);
            qml_.rootContext()->setContextProperty("otwrap", ot);
        }
        {
            auto* app = &parent_;
            QQmlEngine::setObjectOwnership(app, QQmlEngine::CppOwnership);
            qml_.rootContext()->setContextProperty("app", app);
        }

        qml_.load(QUrl(QStringLiteral("qrc:/main.qml")));
    }

    ~QmlApp() final = default;
};

auto App::Imp::factory(App& parent, int& argc, char** argv) noexcept
    -> std::unique_ptr<Imp>
{
    return std::make_unique<QmlApp>(parent, argc, argv);
}
}  // namespace metier
