/*
 * Copyright (C) 2015-2022 Département de l'Instruction Publique (DIP-SEM)
 *
 * Copyright (C) 2013 Open Education Foundation
 *
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour
 * l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */




#include "UBColorPicker.h"

#include <QtGui>

#include "core/memcheck.h"

#include <QPainter>

#include <cmath>

UBColorPicker::UBColorPicker(QWidget* parent)
    : QFrame(parent),
      mRadius(50) // Set an initial radius for the circular palette
{
    mColors.append(Qt::red);
    mColors.append(Qt::green);
    mColors.append(Qt::blue);
    mColors.append(Qt::yellow);
    mColors.append(Qt::magenta);
    mColors.append(Qt::cyan);
    mSelectedColorIndex = 0;
}

UBColorPicker::UBColorPicker(QWidget* parent, int radius, const QList<QColor>& colors, int pSelectedColorIndex)
    : QFrame(parent),
      mColors(colors),
      mSelectedColorIndex(pSelectedColorIndex),
      mRadius(radius)
{
    // Initialize with the provided colors and radius
}

UBColorPicker::~UBColorPicker()
{
    // NOOP
}

void UBColorPicker::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    int centerX = width() / 2;
    int centerY = height() / 2;

    if (mSelectedColorIndex < mColors.size())
    {
        painter.setRenderHint(QPainter::Antialiasing);

        // Calculate the angle between each color section
        float angle = 2 * M_PI / mColors.size();

        for (int i = 0; i < mColors.size(); ++i)
        {
            float startAngle = i * angle;
            float endAngle = (i + 1) * angle;

            // Draw a sector for each color
            painter.setBrush(mColors.at(i));
            painter.drawPie(centerX - mRadius, centerY - mRadius, 2 * mRadius, 2 * mRadius,
                            qRound(startAngle * 180.0 / M_PI), qRound(angle * 180.0 / M_PI));
        }
    }
}

void UBColorPicker::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // Calculate the angle corresponding to the mouse click position
        int centerX = width() / 2;
        int centerY = height() / 2;
        double dx = event->x() - centerX;
        double dy = event->y() - centerY;
        double angle = atan2(dy, dx);

        if (angle < 0)
            angle += 2 * M_PI;

        // Determine the selected color based on the angle
        int colorIndex = static_cast<int>(angle / (2 * M_PI) * mColors.size());

        mSelectedColorIndex = colorIndex;
        repaint();

        emit colorSelected(mColors.at(mSelectedColorIndex));
    }
}
