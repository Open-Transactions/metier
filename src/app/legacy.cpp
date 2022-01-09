// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imp.hpp"  // IWYU pragma: associated

#include <otwrap.hpp>
#include <QApplication>
#include <QEventLoop>
#include <QIcon>
#include <QPushButton>

#include "util/focuser.hpp"
#include "util/scopeguard.hpp"
#include "widgets/blockchainchooser.hpp"
#include "widgets/enterpassphrase.hpp"
#include "widgets/firstrun.hpp"
#include "widgets/mainwindow.hpp"
#include "widgets/newseed.hpp"
#include "widgets/profilealias.hpp"
#include "widgets/recoverwallet.hpp"

namespace metier
{
struct LegacyApp final : public App::Imp, public QApplication {
    static std::unique_ptr<App> singleton_;

    struct PasswordData {
        std::mutex lock_{};
        std::promise<QString> promise_{};
        std::future<QString> future_{};
    };

    App& parent_;
    PasswordData password_;
    const QIcon icon_;
    std::atomic_bool first_run_complete_;
    std::unique_ptr<OTWrap> ot_;
    std::unique_ptr<widget::FirstRun> first_run_;
    std::unique_ptr<widget::NewSeed> new_seed_;
    std::unique_ptr<widget::RecoverWallet> recover_wallet_;
    std::unique_ptr<widget::ProfileAlias> profile_alias_;
    std::unique_ptr<widget::BlockchainChooser> blockchains_;
    std::unique_ptr<widget::MainWindow> main_window_;

    auto displayBlockchainChooser() -> void final
    {
        util::Focuser(blockchains_.get()).show();
    }

    auto displayFirstRun() -> void final
    {
        util::Focuser(first_run_.get()).show();
    }

    auto displayMainWindow() -> void final
    {
        util::Focuser(main_window_.get()).show();
    }

    auto displayNamePrompt() -> void final
    {
        util::Focuser(profile_alias_.get()).show();
    }

    auto displayNewSeed() -> void { util::Focuser(new_seed_.get()).show(); }

    auto displayRecovery() -> void
    {
        util::Focuser(recover_wallet_.get()).show();
    }

    auto confirmPassword(QString prompt, [[maybe_unused]] QString key)
        -> QString final
    {
        auto lock = std::lock_guard<std::mutex>{password_.lock_};
        password_.promise_ = {};
        password_.future_ = password_.promise_.get_future();
        parent_.needPasswordPrompt(prompt, false);

        return password_.future_.get();
    }

    auto displayPasswordPrompt(QString prompt, bool once) -> void final
    {
        using Widget = metier::widget::EnterPassphrase;
        auto dialog = std::make_unique<Widget>(
            prompt, once ? Widget::Mode::Once : Widget::Mode::Twice);
        auto postcondition = metier::ScopeGuard{[&dialog]() {
            dialog->deleteLater();
            dialog.release();
        }};
        dialog->exec();
        password_.promise_.set_value(dialog->secret());
    }

    auto getPassword(QString prompt, [[maybe_unused]] QString key)
        -> QString final
    {
        auto lock = std::lock_guard<std::mutex>{password_.lock_};
        password_.promise_ = {};
        password_.future_ = password_.promise_.get_future();
        parent_.needPasswordPrompt(prompt, true);

        return password_.future_.get();
    }

    auto init(int& argc, char** argv) noexcept -> void final
    {
        ot_ = std::make_unique<OTWrap>(*this, parent_, argc, argv);
        first_run_ = std::make_unique<widget::FirstRun>(this);
        new_seed_ = std::make_unique<widget::NewSeed>(this, *ot_);
        recover_wallet_ = std::make_unique<widget::RecoverWallet>(this, *ot_);
        profile_alias_ = std::make_unique<widget::ProfileAlias>(this);
        blockchains_ = std::make_unique<widget::BlockchainChooser>(this, *ot_);
        main_window_ = std::make_unique<widget::MainWindow>(this, *ot_);
        auto* ot = ot_.get();
        auto* first = first_run_.get();
        auto* alias = profile_alias_.get();
        auto* ok = blockchains_->Ok();
        connect(
            first,
            &widget::FirstRun::wantNew,
            this,
            &LegacyApp::displayNewSeed);
        connect(
            first,
            &widget::FirstRun::wantOld,
            this,
            &LegacyApp::displayRecovery);
        connect(alias, &widget::ProfileAlias::gotAlias, ot, &OTWrap::createNym);
        connect(ok, &QPushButton::clicked, &parent_, &App::startup);
    }

    auto run() -> int final
    {
        setWindowIcon(icon_);

        return exec();
    }

    auto otwrap() noexcept -> OTWrap* final { return ot_.get(); }

    LegacyApp(App& parent, int& argc, char** argv) noexcept
        : QApplication(argc, argv)
        , parent_(parent)
        , password_()
        , icon_(":/qwidget/assets/app_icon.png")
        , first_run_complete_(false)
        , ot_()
        , first_run_()
        , new_seed_()
        , recover_wallet_()
        , profile_alias_()
        , blockchains_()
        , main_window_()
    {
    }

    ~LegacyApp() final = default;
};

auto App::Imp::factory_widgets(App& parent, int& argc, char** argv) noexcept
    -> std::unique_ptr<Imp>
{
    return std::make_unique<LegacyApp>(parent, argc, argv);
}
}  // namespace metier
