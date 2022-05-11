// Copyright (c) 2019-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "api/api.hpp"  // IWYU pragma: associated

#include <opentxs/opentxs.hpp>
#include <QDebug>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QNetworkReply>
#include <QUrl>
#include <algorithm>
#include <utility>

#include "QR-Code-generator/cpp/QrCode.hpp"
#include "api/imp.hpp"
#include "util/convertblockchain.hpp"

namespace qr = qrcodegen;

namespace metier
{
Api::Api(QGuiApplication& parent, App& app, int& argc, char** argv)
    : QObject(&parent)
    , imp_p_(std::make_unique<Imp>(parent, app, *this, argc, argv))
    , imp_(*imp_p_)
{
    using Seed = opentxs::ui::SeedTreeQt;
    auto* seed = imp_.seedManager();
    connect(seed, &Seed::defaultSeedChanged, this, &Api::doNeedSeed);
    connect(seed, &Seed::needSeed, this, &Api::doNeedSeed);
    connect(seed, &Seed::defaultNymChanged, this, &Api::doNeedNym);
    connect(seed, &Seed::needNym, this, &Api::doNeedNym);
    connect(seed, &Seed::ready, this, &Api::doNeedNym);
}

auto Api::accountIDtoBlockchainType(const QString& id) -> int
{
    const auto accountID = imp_.api_.Factory().Identifier(id.toStdString());
    const auto [chain, owner] =
        imp_.api_.Crypto().Blockchain().LookupAccount(accountID);

    return static_cast<int>(chain);
}

auto Api::addContact(QString label, QString id) -> QString
{
    return imp_.identityManager()->getContactList()->addContact(label, id);
}

auto Api::blockchainChooserModel(const bool testnet) -> QAbstractItemModel*
{
    const auto type =
        testnet ? ot::ui::Blockchains::All : ot::ui::Blockchains::Main;

    return imp_.api_.UI().BlockchainSelectionQt(type);
}

auto Api::blockchainChooserModelQML(const bool testnet) -> QObject*
{
    return blockchainChooserModel(testnet);
}

auto Api::blockchainStatisticsModel() -> QAbstractItemModel*
{
    return imp_.api_.UI().BlockchainStatisticsQt();
}

auto Api::blockchainStatisticsModelQML() -> QObject*
{
    return blockchainStatisticsModel();
}

auto Api::blockchainTypeToAccountID(int type) -> QString
{
    return imp_.blockchainTypeToAccountID(type);
}

auto Api::chainIsDisabled(int chain) -> void { imp_.chain_is_disabled(chain); }

auto Api::chainIsEnabled(int chain) -> void { imp_.chain_is_enabled(chain); }

auto Api::checkChains(int chain) -> void { imp_.check_chains(chain); }

auto Api::checkStartupConditions() -> void
{
    switch (imp_.state_.load()) {
        case Imp::State::init:
        case Imp::State::have_seed: {
            imp_.seedManager()->check();
        } break;
        case Imp::State::have_nym: {
            auto* model =
                imp_.api_.UI().BlockchainSelectionQt(ot::ui::Blockchains::All);

            if (imp_.enableDefaultChain()) {
                const auto start = ot::Clock::now();
                constexpr auto limit = std::chrono::minutes{1};

                while (0 == model->enabledCount()) {
                    if ((ot::Clock::now() - start) > limit) {
                        qFatal("Timeout waiting for enabled blockchain");
                    }
                }

                imp_.state_.store(Imp::State::run);
                emit privateReadyForMainWindow({});
            } else if (1 > model->enabledCount()) {
                emit privateNeedBlockchain({});
            } else {
                if (false == imp_.validateBlockchains()) {
                    qFatal("Unable to initialize blockchains");
                }

                imp_.state_.store(Imp::State::run);
                emit privateReadyForMainWindow({});
            }
        } break;
        case Imp::State::run:
        default: {
            emit privateReadyForMainWindow({});
        }
    }
}

auto Api::checkAccounts() -> void { imp_.validateBlockchains(); }

auto Api::Cleanup() noexcept -> void { ot::Cleanup(); }

auto Api::createNewSeed(const int type, const int lang, const int strength)
    -> QStringList
{
    return imp_.createNewSeed(type, lang, strength);
}

auto Api::createNym(QString alias) -> void { imp_.createNym(alias); }

auto Api::doNeedNym() -> void
{
    switch (imp_.state_.load()) {
        case Imp::State::init: {
            doNeedSeed();
        } break;
        case Imp::State::have_seed: {
            const auto [nym, count] = imp_.api_.Wallet().DefaultNym();

            if (nym->empty()) {
                if (false == imp_.wait_for_seed_backup_) {
                    emit privateNeedProfileName({});
                }
            } else {
                if (false == imp_.validateNym()) {
                    qFatal("Unable to initialize identity");
                }

                imp_.state_.store(Imp::State::have_nym);
                emit checkStartupConditions();
            }
        } break;
        case Imp::State::have_nym:
        case Imp::State::run:
        default: {
        }
    }
}

auto Api::doNeedSeed() -> void
{
    switch (imp_.state_.load()) {
        case Imp::State::init: {
            const auto [seed, count] = imp_.api_.Crypto().Seed().DefaultSeed();

            if (seed.empty() && (0u == count)) {
                emit privateNeedSeed({});
            } else {
                if (false == imp_.validateSeed()) {
                    qFatal("Unable to initialize wallet seed");
                }

                imp_.state_.store(Imp::State::have_seed);
                emit checkStartupConditions();
            }
        } break;
        case Imp::State::have_seed:
        case Imp::State::have_nym:
        case Imp::State::run:
        default: {
        }
    }
}

auto Api::enabledBlockchains() -> BlockchainList
{
    return imp_.enabled_chains_.get();
}

auto Api::enabledCurrencyCount() -> int
{
    return static_cast<int>(imp_.enabled_chains_.count());
}

auto Api::getQRcodeBase64(const QString input_string) -> QString
{
    qr::QrCode qr0 = qr::QrCode::encodeText(
        input_string.toStdString().c_str(), qr::QrCode::Ecc::MEDIUM);
    QString xml_svg = QString::fromStdString(qr0.toSvgString(4));
    return xml_svg.toUtf8().toBase64();
}

auto Api::getRecoveryWords() -> QStringList { return imp_.getRecoveryWords(); }

auto Api::identityManager() noexcept -> opentxs::ui::IdentityManagerQt*
{
    return imp_.identityManager();
}

auto Api::identityManagerQML() -> QObject* { return identityManager(); }

auto Api::importSeed(int type, int lang, QString words, QString password)
    -> void
{
    imp_.importSeed(type, lang, words, password);
}

auto Api::longestBlockchainName() -> int
{
    static const auto length = [&] {
        auto out = std::size_t{0};

        for (const auto chain : ot::blockchain::DefinedChains()) {
            out = std::max(out, print(chain).size());
        }

        return out;
    }();

    return static_cast<int>(length);
}

auto Api::longestSeedWord() -> int { return imp_.longest_seed_word_; }

auto Api::openSystemBrowserLink(QString url_link) -> void
{
    QDesktopServices::openUrl(QUrl(url_link, QUrl::StrictMode));
}

auto Api::quit() -> void { QCoreApplication::exit(); }

auto Api::rescanBlockchain(int chain) -> void { imp_.rescanBlockchain(chain); }

auto Api::seedBackupFinished() -> void
{
    imp_.wait_for_seed_backup_ = false;
    emit checkStartupConditions();
}

auto Api::seedLanguageModel(const int type) -> model::SeedLanguage*
{
    return imp_.seedLanguageModel(type);
}

auto Api::seedLanguageModelQML(const int type) -> QObject*
{
    return seedLanguageModel(type);
}

auto Api::seedManager() noexcept -> opentxs::ui::SeedTreeQt*
{
    return imp_.seedManager();
}

auto Api::seedManagerQML() -> QObject* { return seedManager(); }

auto Api::seedSizeModel(const int type) -> model::SeedSize*
{
    return imp_.seedSizeModel(type);
}

auto Api::seedSizeModelQML(const int type) -> QObject*
{
    return seedSizeModel(type);
}

auto Api::seedTypeModel() -> model::SeedType* { return imp_.seed_type_.get(); }

auto Api::seedTypeModelQML() -> QObject* { return seedTypeModel(); }

auto Api::seedWordValidator(const int type, const int lang)
    -> const opentxs::ui::SeedValidator*
{
    return imp_.seedWordValidator(type, lang);
}

auto Api::seedWordValidatorQML(const int type, const int lang) -> QObject*
{
    return const_cast<opentxs::ui::SeedValidator*>(
        seedWordValidator(type, lang));
}

auto Api::validBlockchains() -> BlockchainList
{
    auto output = BlockchainList{};
    const auto& input = ot::blockchain::SupportedChains();
    std::transform(
        input.begin(),
        input.end(),
        std::back_inserter(output),
        [](const auto chain) { return util::convert(chain); });

    return output;
}

auto Api::wordCount(const int type, const int strength) -> int
{
    return imp_.wordCount(type, strength);
}

Api::~Api() = default;
}  // namespace metier
