// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <QObject>
#include <QSize>
#include <QString>
#include <QToolButton>
#include <cstdint>
#include <memory>
#include <vector>

class QImage;
class QPaintEvent;
class QPainter;
class QWidget;

namespace qrcodegen
{
class QrCode;
}  // namespace qrcodegen

namespace Ui
{
class QrToolButton;
}  // namespace Ui

namespace metier::widget
{
class QrToolButton final : public QToolButton
{
    Q_OBJECT

public:
    void setString(QString value);
    int getQRWidth() const;
    bool asImage(QImage& output, int size);

    QrToolButton(QWidget* parent = nullptr);

    ~QrToolButton() final;

private:
    using Bytes = std::vector<std::uint8_t>;

    std::unique_ptr<Ui::QrToolButton> ui_;
    QString value_;
    std::unique_ptr<qrcodegen::QrCode> qr_;

    static auto bytes(const QString& in) noexcept -> Bytes;

    auto draw(QPainter& painter, int width, int height) -> void;
    auto minimumSizeHint() const -> QSize final;
    auto paintEvent(QPaintEvent*) -> void final;
    auto sizeHint() const -> QSize final;

    QrToolButton(const QrToolButton&) = delete;
    QrToolButton(QrToolButton&&) = delete;
    QrToolButton& operator=(const QrToolButton&) = delete;
    QrToolButton& operator=(QrToolButton&&) = delete;
};
}  // namespace metier::widget
