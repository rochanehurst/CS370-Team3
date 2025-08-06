import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15


Rectangle {
    anchors.fill: parent

    Plugin {
        id: osmPlugin
        name: "osm"
    }

    // ListModel to hold marker coordinates
    ListModel {
        id: markerModel
    }

    // ListModel to hold class info on map
    ListModel{
        id: infoModel
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: osmPlugin
        center: QtPositioning.coordinate(33.1284, -117.1609)
        zoomLevel: 18
        minimumZoomLevel: 17
        maximumZoomLevel: 20

        property var lastMousePos: Qt.point(0, 0)
        property bool dragging: false


        MapPolyline {
                   id: routeLine
                   line.width: 5
                   line.color: "blue"
                   path: []  // will be updated dynamically
               }

        MouseArea {
            anchors.fill: parent
            drag.target: null

            onPressed: function(mouse) {
                map.lastMousePos = Qt.point(mouse.x, mouse.y)
                map.dragging = true
            }
            onReleased: function(mouse) {
                map.dragging = false
            }
            onCanceled: function(mouse) {
                map.dragging = false
            }
            onExited: function(mouse) {
                map.dragging = false
            }

            onPositionChanged: function(mouse) {
                if (map.dragging) {
                    var dx = mouse.x - map.lastMousePos.x
                    var dy = mouse.y - map.lastMousePos.y
                    var scale = 360 / (256 * Math.pow(2, map.zoomLevel))
                    var newLat = map.center.latitude + dy * scale
                    var newLon = map.center.longitude - dx * scale
                    map.center = QtPositioning.coordinate(newLat, newLon)
                    map.lastMousePos = Qt.point(mouse.x, mouse.y)
                }
            }

            onWheel: function(event) {
                const zoomStep = 0.5;
                if (event.angleDelta.y > 0) {
                    map.zoomLevel = Math.min(map.zoomLevel + zoomStep, map.maximumZoomLevel);
                } else {
                    map.zoomLevel = Math.max(map.zoomLevel - zoomStep, map.minimumZoomLevel);
                }
                event.accepted = true;
            }
        }

        function addMarker(lat, lon) {
            markerModel.append({ latitude: lat, longitude: lon })
        }

        function removeMarker(lat, lon) {
            for (var i = 0; i < markerModel.count; i++) {
                var item = markerModel.get(i)
                if (item.latitude === lat && item.longitude === lon) {
                    markerModel.remove(i)
                    break
                }
            }
        }

        function removeAll() {
            markerModel.clear()
            infoModel.clear()
        }

        function addInfo(lat, lon, newText) {
            for (var i = 0; i < infoModel.count; i++) {
                var item = infoModel.get(i)
                if (item.latitude === lat && item.longitude === lon) {
                    infoModel.setProperty(i, "text", item.text + "\n" + newText)
                    return
                }
            }
            infoModel.append({ latitude: lat, longitude: lon, text: newText })
        }

        function removeInfo(lat, lon, textToRemove) {
            for (var i = 0; i < infoModel.count; i++) {
                var item = infoModel.get(i)
                if (item.latitude === lat && item.longitude === lon) {
                    var lines = item.text.split("\n")

                    var index = lines.indexOf(textToRemove)
                    if (index !== -1)
                        lines.splice(index, 1)

                    if (lines.length > 0) {
                        infoModel.setProperty(i, "text", lines.join("\n"))
                    } else {
                        infoModel.remove(i)
                    }
                    return
                }
            }
        }

        MapItemView {
            model: markerModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(latitude, longitude)
                anchorPoint.x: 8
                anchorPoint.y: 8
                sourceItem: Rectangle {
                    width: 16
                    height: 16
                    radius: 8
                    color: "red"
                    border.color: "black"
                    border.width: 1
                }
            }
        }

        MapItemView {
            model: infoModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(latitude, longitude)

                sourceItem: Rectangle {
                    id: labelRect
                    color: "#ffffffcc"
                    radius: 6
                    border.color: "#444"
                    border.width: 1
                    width: 140
                    height: implicitHeight
                    visible: model.text && model.text.length > 0

                    Text {
                        anchors.fill: parent
                        anchors.margins: 6
                        text: model.text
                        wrapMode: Text.Wrap
                        font.pixelSize: 15
                        color: "black"
                    }
                }
                anchorPoint.x: 0
                anchorPoint.y: labelRect.height / 2
                x: 16
                y: 0
            }
        }

    }

    //used to conenct to QML from other files
    Connections {
        target: apiMap

        onMarkerCoordinate: function(lat, lon) {
            map.addMarker(lat, lon)
        }

        onRemoveMarker: function(lat, lon) {
            map.removeMarker(lat, lon)
        }

        onRemoveAll: {
            map.removeAll()
        }

        onAddInfo: function(lat, lon, infoText) {
            map.addInfo(lat, lon, infoText)
        }

        onRemoveInfo: function(lat, lon, infoText) {
            map.removeInfo(lat, lon, infoText)
        }

        onRouteReady: function(routeCoordinates) {
            // Clear any previously drawn route
            routeLine.path = [];

            // Then add the new route
            for (let i = 0; i < routeCoordinates.length; i++) {
                routeLine.path.push(routeCoordinates[i]);
            }

            // (Optional) Recenter the map to the start of the route
            if (routeCoordinates.length > 0)
                map.center = routeCoordinates[0];
        }

        onNoRoute: {
            routeLine.path = [];
        }
    }
}
