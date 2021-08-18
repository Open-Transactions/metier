// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <boost/program_options.hpp>

#include "app/imp.hpp"

namespace po = boost::program_options;

namespace metier
{
auto App::Imp::factory(App& parent, int& argc, char** argv) noexcept -> std::unique_ptr<Imp>
{
    static constexpr auto advanced{"advanced"};
    [[maybe_unused]] auto use_advanced{false};
    const auto options = [] {
        const auto text = std::string{METIER_APP_NAME} + " options";
        auto out = po::options_description{text};
        out.add_options()(
            advanced,
            po::value<bool>()->implicit_value("true"),
            "Use advanced gui");

        return out;
    }();
    auto variables = po::variables_map{};

    try {
        auto parsed = po::command_line_parser(argc, argv)
                          .options(options)
                          .allow_unregistered()
                          .run();
        po::store(parsed, variables);
        po::notify(variables);
    } catch (po::error& e) {
    }

    for (const auto& [name, value] : variables) {
        if (name == advanced) {
            try {
                use_advanced = value.as<bool>();
            } catch (...) {
            }
        }
    }

#if METIER_QML_INTERFACE
    if (use_advanced) {

        return factory_widgets(parent, argc, argv);
    } else {

        return factory_qml(parent, argc, argv);
    }
#else
    return factory_widgets(parent, argc, argv);
#endif
}
}  // namespace metier
