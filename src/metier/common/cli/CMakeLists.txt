# Copyright (c) 2019-2024 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

find_package(
  Boost
  1.82.0
  REQUIRED
  COMPONENTS json program_options
  OPTIONAL_COMPONENTS container date_time
)

metier_add_private_header_target(metier-cli-common "endpoint.hpp")
metier_add_private_header_target(metier-cli-common "parser.hpp")
metier_add_private_header_target(metier-cli-common "processor.hpp")
metier_add_source_file_target(metier-cli-common "parser.cpp")
metier_add_source_file_target(metier-cli-common "processor.cpp")
metier_link_external_target(metier-cli-common Boost::json)
metier_link_external_target(metier-cli-common Boost::program_options)
metier_link_external_target(metier-cli-common opentxs::libopentxs)
metier_link_external_target(metier-cli-common "${METIER_ZMQ_TARGET}")
