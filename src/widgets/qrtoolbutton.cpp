// Copyright (c) 2019-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "qrtoolbutton.hpp"  // IWYU pragma: associated

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include <string>

#include "QR-Code-generator/cpp/QrCode.hpp"
#include "ui_qrtoolbutton.h"

namespace qr = qrcodegen;

constexpr auto quality_{qr::QrCode::Ecc::MEDIUM};

namespace metier::widget
{
QrToolButton::QrToolButton(QWidget* parent)
    : QToolButton(parent)
    , ui_(std::make_unique<Ui::QrToolButton>())
    , value_()
    , qr_(std::make_unique<qr::QrCode>(
          qr::QrCode::encodeBinary(bytes(value_), quality_)))
{
    ui_->setupUi(this);
    update();
}

auto QrToolButton::asImage(QImage& output, int size) -> bool
{
    if (size == 0) { return false; }

    QImage image(size, size, QImage::Format_Mono);
    QPainter painter(&image);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, size, size);

    if (qr_) { draw(painter, size, size); }

    output = image;

    return true;
}

auto QrToolButton::bytes(const QString& in) noexcept -> Bytes
{
    const auto data = in.toStdString();
    const auto* p = reinterpret_cast<const std::uint8_t*>(data.data());

    return {p, p + data.size()};
}

auto QrToolButton::draw(QPainter& painter, int width, int height) -> void
{
    painter.setBrush(QColor{Qt::black});
    const auto qr_width = qr_->getSize() > 0 ? qr_->getSize() : 1;
    const auto scale_qr_width = static_cast<double>(qr_width);
    const auto scale_width = static_cast<double>(width);
    const auto scale_height = static_cast<double>(height);
    const auto double_scale_x = scale_width / scale_qr_width;
    const auto double_scale_y = scale_height / scale_qr_width;

    for (int y = 0; y < qr_width; y++) {
        const double scale_y = y;

        for (int x = 0; x < qr_width; x++) {
            const double scale_x = x;

            if (qr_->getModule(x, y)) {
                QRectF r(
                    scale_x * double_scale_x,
                    scale_y * double_scale_y,
                    double_scale_x,
                    double_scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
}

auto QrToolButton::getQRWidth() const -> int
{
    if (qr_) { return qr_->getSize(); }

    return 1;
}

auto QrToolButton::paintEvent(QPaintEvent*) -> void
{
    auto p = QStylePainter{this};
    auto sotb = QStyleOptionToolButton{};
    sotb.initFrom(this);
    p.drawComplexControl(QStyle::CC_ToolButton, sotb);
    auto background = QColor{Qt::white};
    auto& painter = static_cast<QPainter&>(p);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());

    if (qr_) { draw(painter, width(), height()); }
}

auto QrToolButton::setString(QString value) -> void
{
    value_ = value;
    qr_ = std::make_unique<qr::QrCode>(
        qr::QrCode::encodeBinary(bytes(value_), quality_));
    update();
}

auto QrToolButton::sizeHint() const -> QSize
{
    auto output = QSize{};

    if (qr_) {
        const auto width{qr_->getSize() > 0 ? qr_->getSize() : 1};
        output = QSize{width * 4, width * 4};
    } else {
        output = QSize{148, 148};
    }

    return output;
}

auto QrToolButton::minimumSizeHint() const -> QSize
{
    auto output = QSize{};

    if (qr_) {
        auto width = qr_->getSize() > 0 ? qr_->getSize() : 1;
        output = QSize{width, width};
    } else {
        output = QSize{148, 148};
    }

    return output;
}

QrToolButton::~QrToolButton() = default;
}  // namespace metier::widget
