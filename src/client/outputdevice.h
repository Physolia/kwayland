/********************************************************************
Copyright 2013  Martin Gräßlin <mgraesslin@kde.org>

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
*********************************************************************/
#ifndef WAYLAND_OUTPUTDEVICE_H
#define WAYLAND_OUTPUTDEVICE_H

#include <QObject>
#include <QPointer>
#include <QSize>

#include <KWayland/Client/kwaylandclient_export.h>

struct org_kde_kwin_outputdevice;
class QPoint;
class QRect;

namespace KWayland
{
namespace Client
{

class EventQueue;

/**
 * @short Wrapper for the org_kde_kwin_outputdevice interface.
 *
 * This class provides a convenient wrapper for the org_kde_kwin_outputdevice interface.
 * Its main purpose is to hold the information about one OutputDevice.
 *
 * To use this class one needs to interact with the Registry. There are two
 * possible ways to create an OutputDevice interface:
 * @code
 * OutputDevice *c = registry->createOutputDevice(name, version);
 * @endcode
 *
 * This creates the OutputDevice and sets it up directly. As an alternative this
 * can also be done in a more low level way:
 * @code
 * OutputDevice *c = new OutputDevice;
 * c->setup(registry->bindOutputDevice(name, version));
 * @endcode
 *
 * The OutputDevice can be used as a drop-in replacement for any org_kde_kwin_outputdevice
 * pointer as it provides matching cast operators.
 *
 * Please note that all properties of OutputDevice are not valid until the
 * changed signal has been emitted. The wayland server is pushing the
 * information in an async way to the OutputDevice instance. By emitting changed
 * the OutputDevice indicates that all relevant information is available.
 *
 * @see Registry
 **/
class KWAYLANDCLIENT_EXPORT OutputDevice : public QObject
{
    Q_OBJECT
public:
    struct Edid {
        QString eisaId;
        QString monitorName;
        QString serialNumber;
        QSize physicalSize;
        QString data;
    };
    enum class SubPixel {
        Unknown,
        None,
        HorizontalRGB,
        HorizontalBGR,
        VerticalRGB,
        VerticalBGR
    };
    enum class Transform {
        Normal,
        Rotated90,
        Rotated180,
        Rotated270,
        Flipped,
        Flipped90,
        Flipped180,
        Flipped270
    };
    struct Mode {
        enum class Flag {
            None = 0,
            Current = 1 << 0,
            Preferred = 1 << 1
        };
        Q_DECLARE_FLAGS(Flags, Flag)
        /**
         * The size of this Mode in pixel space.
         **/
        QSize size;
        /**
         * The refresh rate in mHz of this Mode.
         **/
        int refreshRate = 0;
        /**
         * The flags of this Mode, that is whether it's the
         * Current and/or Preferred Mode of the OutputDevice.
         **/
        Flags flags = Flag::None;
        /**
         * The OutputDevice to which this Mode belongs.
         **/
        QPointer<OutputDevice> output;

        bool operator==(const Mode &m) const;
    };
    explicit OutputDevice(QObject *parent = nullptr);
    virtual ~OutputDevice();

    /**
     * Setup this Compositor to manage the @p output.
     * When using Registry::createOutputDevice there is no need to call this
     * method.
     **/
    void setup(org_kde_kwin_outputdevice *output);

    /**
     * @returns @c true if managing a org_kde_kwin_outputdevice.
     **/
    bool isValid() const;
    operator org_kde_kwin_outputdevice*();
    operator org_kde_kwin_outputdevice*() const;
    org_kde_kwin_outputdevice *output();
    /**
     * Size in millimeters.
     **/
    QSize physicalSize() const;
    /**
     * Position within the global compositor space.
     **/
    QPoint globalPosition() const;
    /**
     * Textual description of the manufacturer.
     **/
    QString manufacturer() const;
    /**
     * Textual description of the model.
     **/
    QString model() const;
    /**
     * Size in the current mode.
     **/
    QSize pixelSize() const;
    /**
     * The geometry of this OutputDevice in pixels.
     * Convenient for QRect(globalPosition(), pixelSize()).
     * @see globalPosition
     * @see pixelSize
     **/
    QRect geometry() const;
    /**
     * Refresh rate in mHz of the current mode.
     **/
    int refreshRate() const;
    /**
     * Scaling factor of this output.
     *
     * A scale larger than 1 means that the compositor will automatically scale surface buffers
     * by this amount when rendering. This is used for very high resolution displays where
     * applications rendering at the native resolution would be too small to be legible.
     **/
    int scale() const;
    /**
     * Subpixel orientation of this OutputDevice.
     **/
    SubPixel subPixel() const;
    /**
     * Transform that maps framebuffer to OutputDevice.
     *
     * The purpose is mainly to allow clients render accordingly and tell the compositor,
     * so that for fullscreen surfaces, the compositor will still be able to scan out
     * directly from client surfaces.
     **/
    Transform transform() const;

    /**
     * @returns The Modes of this OutputDevice.
     **/
    QList<Mode> modes() const;

    /**
     * Sets the @p queue to use for bound proxies.
     **/
    void setEventQueue(EventQueue *queue);
    /**
     * @returns The event queue to use for bound proxies.
     **/
    EventQueue *eventQueue() const;

    /**
     * @returns The EDID information for this output.
     **/
    Edid* edid() const;

    /**
     * @returns Whether this output is enabled or not.
     **/
    bool enabled() const;

Q_SIGNALS:
    /**
     * Emitted whenever at least one of the data changed.
     **/
    void changed();
    /**
     * Emitted whenever the enabled property changes.
     **/
    void enabledChanged(bool enabled);
    /**
     * Emitted whenever a new Mode is added.
     * This normally only happens during the initial promoting of modes.
     * Afterwards only modeChanged should be emitted.
     * @param mode The newly added Mode.
     * @see modeChanged
     **/
    void modeAdded(const KWayland::Client::OutputDevice::Mode &mode);
    /**
     * Emitted whenever a Mode changes.
     * This normally means that the @c Mode::Flag::Current is added or removed.
     * @param mode The changed Mode
     **/
    void modeChanged(const KWayland::Client::OutputDevice::Mode &mode);

private:
    class Private;
    QScopedPointer<Private> d;
};

}
}

Q_DECLARE_METATYPE(KWayland::Client::OutputDevice::SubPixel)
Q_DECLARE_METATYPE(KWayland::Client::OutputDevice::Transform)
Q_DECLARE_METATYPE(KWayland::Client::OutputDevice::Mode)
Q_DECLARE_OPERATORS_FOR_FLAGS(KWayland::Client::OutputDevice::Mode::Flags)

#endif
