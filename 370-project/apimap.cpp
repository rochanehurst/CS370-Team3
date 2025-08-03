#include "apimap.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

ApiMap::ApiMap(QObject *parent)
    : QObject(parent),
    networkManager_(new QNetworkAccessManager(this)),
    mapboxToken_("pk.eyJ1IjoicmNhbHA5ODQ2IiwiYSI6ImNtZGllZnFodDBidGIybG9hNWl6MGo2bGkifQ.YkWGginaPMHEMwHFO92L4A") // ← Replace with your actual token
{
    // Initialize buildingCoordinates_ map here:
    buildingCoordinates_["Academic Hall (ACD)"] = QGeoCoordinate(33.12812065219856, -117.15856711844148);
    buildingCoordinates_["Arts Building (ARTS)"] = QGeoCoordinate(33.130035070262586, -117.15802583677679);
    buildingCoordinates_["Extended Learning Building (ELB)"] = QGeoCoordinate(33.13414127002338, -117.16011764848307);
    buildingCoordinates_["Kellogg Library (KEL)"] = QGeoCoordinate(33.129085897084224, -117.15967655175587);
    buildingCoordinates_["Markstein Hall (MARK)"] = QGeoCoordinate(33.12797627144677, -117.15782577448168);
    buildingCoordinates_["Science Hall I (SCI 1)"] = QGeoCoordinate(33.12767420594281, -117.15873979957497);
    buildingCoordinates_["Science Hall II (SCI 2)"] = QGeoCoordinate(33.130700120745125, -117.15794987104177);
    buildingCoordinates_["Social and Behavioral Sciences Building (SBSB)"] = QGeoCoordinate(33.13060042405003, -117.15724256704904);
    buildingCoordinates_["University Hall (UNIV)"] = QGeoCoordinate(33.128726937160486, -117.15855757758689);
    buildingCoordinates_["Viasat Engineering Pavilion (VEP)"] = QGeoCoordinate(33.12722195391456, -117.15802726557422);
}

QGeoCoordinate ApiMap::coordinateForBuilding(const QString &buildingName) const
{
    if (buildingCoordinates_.contains(buildingName)) {
        return buildingCoordinates_.value(buildingName);
    }
    return QGeoCoordinate(); // invalid coordinate if not found
}

void ApiMap::fetchCoordinatesFromName(const QString &placeName)
{
    QString encodedPlace = QUrl::toPercentEncoding(placeName);
    QString url = QString("https://api.mapbox.com/geocoding/v5/mapbox.places/%1.json?access_token=%2")
                      .arg(encodedPlace, mapboxToken_);

    QNetworkReply *reply = networkManager_->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleGeocodingReply(reply); });
}

void ApiMap::fetchWalkingRoute(const QGeoCoordinate &start, const QGeoCoordinate &end)
{
    QString coordinates = QString::number(start.longitude(), 'f', 6) + "," +
                          QString::number(start.latitude(), 'f', 6) + ";" +
                          QString::number(end.longitude(), 'f', 6) + "," +
                          QString::number(end.latitude(), 'f', 6);

    QString url = QString("https://api.mapbox.com/directions/v5/mapbox/walking/%1?geometries=geojson&overview=full&access_token=%2")
                      .arg(coordinates, mapboxToken_);

    //not full access route
    //     QString url = QString("https://api.mapbox.com/directions/v5/mapbox/walking/%1?geometries=geojson&access_token=%2")
    //                       .arg(coordinates, mapboxToken_);

    QNetworkReply *reply = networkManager_->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleRoutingReply(reply); });
}

void ApiMap::handleGeocodingReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit apiError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray features = rootObj["features"].toArray();

    if (!features.isEmpty()) {
        QJsonArray coords = features.first()["geometry"].toObject()["coordinates"].toArray();
        double lon = coords.at(0).toDouble();
        double lat = coords.at(1).toDouble();
        emit coordinatesReceived(QGeoCoordinate(lat, lon));
    } else {
        emit apiError("No coordinates found for the place.");
    }

    reply->deleteLater();
}

void ApiMap::handleRoutingReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit apiError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray routes = rootObj.value("routes").toArray();
    // QJsonArray routes = jsonDoc.object()["routes"].toArray();
    if (routes.isEmpty()) {
        emit apiError("No route found.");
        reply->deleteLater();
        return;
    }

    QJsonArray coordinates = routes.first().toObject()
                                 .value("geometry").toObject()
                                 .value("coordinates").toArray();

    QList<QGeoCoordinate> routeList;
    // for (const QJsonValue &val : coordinates) {
        // QJsonArray coord = val.toArray();
    for (int i = 0; i < coordinates.size(); i++){
        QJsonArray coord = coordinates.at(i).toArray();
        double lon = coord.at(0).toDouble();
        double lat = coord.at(1).toDouble();
        routeList.append(QGeoCoordinate(lat, lon));
    }

    //===
    // Convert routeList to QVariantList for QML
    QVariantList qmlRoute;
    for (const QGeoCoordinate &coord : routeList) {
        qmlRoute.append(QVariant::fromValue(coord));
    }

    reply->deleteLater();
}

void ApiMap::showMarkerAt(const QGeoCoordinate &coord) {
    qDebug() << "Emitting marker at:" << coord;
    emit markerCoordinate(coord.latitude(), coord.longitude());
}

void ApiMap::removeMarkerAt(const QGeoCoordinate &coord){
    qDebug() << "Removing marker at:" << coord;
    emit removeMarker(coord.latitude(), coord.longitude());
}

void ApiMap::removeAllMap(){
    qDebug() << "Removing All Markers:";
    emit removeAll();
}

void ApiMap::addInfoToMap(const QGeoCoordinate &coord, const QString &name, const QString &days, const QString &start, const QString &end){
    QString info = "-" + name + ", " + days + ", " + start + " - " + end;
    qDebug() << "Adding " << info << " to the map";
    emit addInfo(coord.latitude(), coord.longitude(), info);
}

void ApiMap::removeInfoMap(const QGeoCoordinate &coord, const QString &name, const QString &days, const QString &start, const QString &end){
    QString info = "-" + name + ", " + days + ", " + start + " - " + end;
    qDebug() << "Removing " << info << " from the map";
    emit removeInfo(coord.latitude(), coord.longitude(), info);
}

