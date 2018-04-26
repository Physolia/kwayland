/****************************************************************************
Copyright 2018  Marco Martin <notmart@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#ifndef KWAYLAND_SERVER_PLASMAVIRTUALDESKTOP_H
#define KWAYLAND_SERVER_PLASMAVIRTUALDESKTOP_H

#include "global.h"
#include "resource.h"

#include <KWayland/Server/kwaylandserver_export.h>

namespace KWayland
{
namespace Server
{

class Display;

class KWAYLANDSERVER_EXPORT PlasmaVirtualDesktopManagementInterface : public Global
{
    Q_OBJECT
public:
    virtual ~PlasmaVirtualDesktopManagementInterface();

private:
    explicit PlasmaVirtualDesktopManagementInterface(Display *display, QObject *parent = nullptr);
    friend class Display;
    class Private;
};

class KWAYLANDSERVER_EXPORT PlasmaVirtualDesktopInterface : public Resource
{
    Q_OBJECT
public:
    virtual ~PlasmaVirtualDesktopInterface();

private:
    explicit PlasmaVirtualDesktopInterface(PlasmaVirtualDesktopManagementInterface *parent, wl_resource *parentResource);
    friend class PlasmaVirtualDesktopManagementInterface;

    class Private;
    Private *d_func() const;
};


}
}

#endif
