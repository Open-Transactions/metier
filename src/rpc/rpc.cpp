// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "rpc.hpp"  // IWYU pragma: associated

#include <QDir>
#include <QStandardPaths>

namespace metier
{
auto rpc_endpoint() noexcept -> std::string
{
    auto base =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    const auto folder = QDir{base.append("/otrpc/")};
    const auto absolute = folder.absolutePath();

    if (false == folder.mkpath(absolute)) {
        qFatal("Failed to create RPC socket folder");
    }

    const auto filename = std::string{METIER_APP_NAME} + ".sock";

    return std::string{"ipc://"} +
           QDir{absolute}.filePath(filename.c_str()).toStdString();
}
}  // namespace metier
