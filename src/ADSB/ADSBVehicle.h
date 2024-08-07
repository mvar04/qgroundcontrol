/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QElapsedTimer>
#include <QtCore/QLoggingCategory>
#include <QtPositioning/QGeoCoordinate>

Q_DECLARE_LOGGING_CATEGORY(ADSBVehicleLog)

class ADSBVehicle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int              icaoAddress READ icaoAddress    CONSTANT)
    Q_PROPERTY(QString          callsign    READ callsign       NOTIFY callsignChanged)
    Q_PROPERTY(QGeoCoordinate   coordinate  READ coordinate     NOTIFY coordinateChanged)
    Q_PROPERTY(double           altitude    READ altitude       NOTIFY altitudeChanged)     // NaN for not available
    Q_PROPERTY(double           heading     READ heading        NOTIFY headingChanged)      // NaN for not available
    Q_PROPERTY(bool             alert       READ alert          NOTIFY alertChanged)        // Collision path

public:
    typedef struct {
        uint32_t        icaoAddress;    // Required
        QString         callsign;
        QGeoCoordinate  location;
        double          altitude;
        double          heading;
        bool            alert;
        uint32_t        availableFlags;
    } ADSBVehicleInfo_t;
    // TODO: Why can't we just use QGeoLocation with heading & altitude?

    ADSBVehicle(const ADSBVehicleInfo_t &vehicleInfo, QObject* parent);

    enum {
        CallsignAvailable =     1 << 1,
        LocationAvailable =     1 << 2,
        AltitudeAvailable =     1 << 3,
        HeadingAvailable =      1 << 4,
        AlertAvailable =        1 << 5,
    };

    int             icaoAddress (void) const { return static_cast<int>(_icaoAddress); }
    QString         callsign    (void) const { return _callsign; }
    QGeoCoordinate  coordinate  (void) const { return _coordinate; }
    double          altitude    (void) const { return _altitude; }
    double          heading     (void) const { return _heading; }
    bool            alert       (void) const { return _alert; }

    void update(const ADSBVehicleInfo_t &vehicleInfo);

    /// check if the vehicle is expired and should be removed
    bool expired();

signals:
    void coordinateChanged();
    void callsignChanged();
    void altitudeChanged();
    void headingChanged();
    void alertChanged();

private:
    uint32_t        _icaoAddress;
    QString         _callsign;
    QGeoCoordinate  _coordinate;
    double          _altitude;
    double          _heading;
    bool            _alert;

    QElapsedTimer   _lastUpdateTimer;

    static constexpr qint64 expirationTimeoutMs = 120000;   ///< timeout with no update in ms after which the vehicle is removed.
};

Q_DECLARE_METATYPE(ADSBVehicle::ADSBVehicleInfo_t)
