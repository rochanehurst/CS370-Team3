#ifndef APIMAP_H
#define APIMAP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QGeoCoordinate>

class ApiMap : public QObject
{
    Q_OBJECT

public:
    explicit ApiMap(QObject *parent = nullptr);
    void fetchCoordinatesFromName(const QString &placeName);
    void fetchWalkingRoute(const QGeoCoordinate &start, const QGeoCoordinate &end);
    QGeoCoordinate coordinateForBuilding(const QString &buildingName) const; // coordinate list
    void showMarkerAt(const QGeoCoordinate &coord);
    void removeMarkerAt(const QGeoCoordinate &coord);
    void removeAllMap();
    void addInfoToMap(const QGeoCoordinate &coord, const QString &name, const QString &days, const QString &start, const QString &end);
    void removeInfoMap(const QGeoCoordinate &coord, const QString &name, const QString &days, const QString &start, const QString &end);

signals:
    void coordinatesReceived(const QGeoCoordinate &coordinate);
    void routeReceived(const QList<QGeoCoordinate> &route);
    void apiError(const QString &errorString);
    void routeReceivedQml(const QVariantList &route); // edit l8r
    void markerCoordinate(double latitutde, double longitude);
    void removeMarker(double latitude, double longitutde);
    void removeAll();
    void addInfo(double latitude, double longitutde, const QString &info);
    void removeInfo(double latitude, double longitutde, const QString &info);

private slots:
    void handleGeocodingReply(QNetworkReply *reply);
    void handleRoutingReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager_;
    QString mapboxToken_;
    QMap<QString, QGeoCoordinate> buildingCoordinates_;    // New member: map of building names to coordinates
};

#endif // APIMAP_H
