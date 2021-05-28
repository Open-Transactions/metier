// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "widgets/enterpassphrase/imp.hpp"  // IWYU pragma: associated

namespace metier::widget
{
struct Once final : public EnterPassphrase::Imp {
    auto validate() const noexcept -> bool final
    {
        return 0 < get_entry().size();
    }

    auto confirmChanged() noexcept -> void final {}
    auto passphraseChanged() noexcept -> void final
    {
        if (validate()) {
            use_provided_password();
        } else {
            use_default_password();
        }
    }

    Once(EnterPassphrase* parent, const QString& prompt) noexcept
        : Imp(parent, prompt)
    {
        enable_ok();
    }

    ~Once() final = default;
};

struct Twice final : public EnterPassphrase::Imp {
    auto validate() const noexcept -> bool final
    {
        const auto entry = get_entry();

        if (0 == entry.size()) { return true; }

        const auto confirm = get_confirm();

        return entry == confirm;
    }

    auto confirmChanged() noexcept -> void final
    {
        if (validate()) {
            use_provided_password();
            enable_ok();
        } else {
            disable_ok();
        }
    }

    auto passphraseChanged() noexcept -> void final
    {
        const auto entry = get_entry();

        if (0 == entry.size()) {
            hide_confirm();
        } else {
            show_confirm();
        }

        confirmChanged();
    }

    Twice(EnterPassphrase* parent, const QString& prompt) noexcept
        : Imp(parent, prompt)
    {
        enable_ok();
    }

    ~Twice() final = default;
};

EnterPassphrase::Imp::Imp(
    EnterPassphrase* parent,
    const QString& prompt) noexcept
    : ui_(std::make_unique<Ui::EnterPassphrase>())
    , lock_()
    , prompt_(prompt)
    , entry_()
{
    assert(ui_);

    auto& ui = *ui_;
    ui.setupUi(parent);

    if (prompt.size() > 0) { ui.displayString->setText(prompt); }

    auto* password = ui_->passphrase;
    auto* retype = ui_->retype;
    password->setEchoMode(QLineEdit::Password);
    password->setStyleSheet(QLatin1String("lineedit-password-character: 9679"));
    util::set_minimum_size(*password, 55, 1);
    retype->setEchoMode(QLineEdit::Password);
    retype->setStyleSheet(QLatin1String("lineedit-password-character: 9679"));
    hide_confirm();
}

auto EnterPassphrase::Imp::cancel() noexcept -> void { use_default_password(); }

auto EnterPassphrase::Imp::default_password() const noexcept -> const QString&
{
    static const auto password = QString{"opentxs"};

    return password;
}

auto EnterPassphrase::Imp::disable_ok() noexcept -> void
{
    auto lock = std::lock_guard<std::mutex>{lock_};
    auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
    ok->setEnabled(false);
}

auto EnterPassphrase::Imp::enable_ok() noexcept -> void
{
    auto lock = std::lock_guard<std::mutex>{lock_};
    auto* ok = ui_->buttons->button(QDialogButtonBox::Ok);
    ok->setEnabled(true);
}

auto EnterPassphrase::Imp::factory(
    EnterPassphrase* parent,
    const QString& prompt,
    Mode mode) noexcept -> std::unique_ptr<Imp>
{
    if (Mode::Once == mode) {

        return std::make_unique<Once>(parent, prompt);
    } else {

        return std::make_unique<Twice>(parent, prompt);
    }
}

auto EnterPassphrase::Imp::get_confirm() const noexcept -> QString
{
    auto lock = std::lock_guard<std::mutex>{lock_};

    return ui_->retype->text();
}

auto EnterPassphrase::Imp::get_entry() const noexcept -> QString
{
    auto lock = std::lock_guard<std::mutex>{lock_};

    return ui_->passphrase->text();
}

auto EnterPassphrase::Imp::hide_confirm() noexcept -> void
{
    auto lock = std::lock_guard<std::mutex>{lock_};
    ui_->retype->setText({});
    ui_->retype->hide();
    ui_->labelRetype->hide();
}

auto EnterPassphrase::Imp::secret() const noexcept -> QString
{
    auto lock = std::lock_guard<std::mutex>{lock_};

    if (0 == entry_.size()) {

        return default_password();
    } else {

        return entry_;
    }
}

auto EnterPassphrase::Imp::show_confirm() noexcept -> void
{
    auto lock = std::lock_guard<std::mutex>{lock_};
    ui_->retype->show();
    ui_->labelRetype->show();
}

auto EnterPassphrase::Imp::use_default_password() noexcept -> void
{
    auto lock = std::lock_guard<std::mutex>{lock_};
    entry_ = default_password();
}

auto EnterPassphrase::Imp::use_provided_password() noexcept -> void
{
    auto lock = std::lock_guard<std::mutex>{lock_};
    entry_ = ui_->passphrase->text();
}
}  // namespace metier::widget
