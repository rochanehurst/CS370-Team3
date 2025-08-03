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
        zoomLevel: 19
        minimumZoomLevel: 14
        maximumZoomLevel: 20

        property var lastMousePos: Qt.point(0, 0)
        property bool dragging: false


        MapPolyline {
                   id: routeLine
                   line.width: 5
                   line.color: "blue"
                   path: []  // will be updated dynamically
               }

        //===== was used for testing clickable points on map =====
        // MapQuickItem {
        //     coordinate: QtPositioning.coordinate(33.1284, -117.1609) // Your target location
        //     anchorPoint.x: 8
        //     anchorPoint.y: 8

        //     sourceItem: Rectangle {
        //             width: 16
        //             height: 16
        //             radius: 8
        //             color: toggled ? "blue" : "red"
        //             border.color: "black"
        //             border.width: 2

        //             property bool toggled: false

        //             MouseArea {
        //                 anchors.fill: parent
        //                 onClicked: {
        //                     parent.toggled = !parent.toggled
        //                     console.log("Clicked marker. Toggled state:", parent.toggled)
        //                 }
        //             }
        //         }
        // }


        MouseArea {
            anchors.fill: parent //fill the parent area (map) to detect the mouse
            drag.target: null    //initally set drag indicator to null

            onPressed: function(mouse) { //store position of the mouse and change drag indicator to true
                map.lastMousePos = Qt.point(mouse.x, mouse.y)
                map.dragging = true
            }
            onReleased: function(mouse) { //when not pressed, set drag to false
                map.dragging = false
            }
            onCanceled: function(mouse) { //when sytstem error, set drag to false
                map.dragging = false
            }
            onExited: function(mouse) { //when leaving map area, set drag to false
                map.dragging = false
            }

            onPositionChanged: function(mouse) { //when mouse position changes
                if (map.dragging) { //if dragging map (drag indicator is true)
                    var dx = mouse.x - map.lastMousePos.x
                    var dy = mouse.y - map.lastMousePos.y

                    // Convert pixels to degrees roughly
                    var scale = 360 / (256 * Math.pow(2, map.zoomLevel))

                    var newLat = map.center.latitude + dy * scale
                    var newLon = map.center.longitude - dx * scale

                    map.center = QtPositioning.coordinate(newLat, newLon)

                    map.lastMousePos = Qt.point(mouse.x, mouse.y)
                }
            }
        }

        function addMarker(lat, lon) {
            console.log("Adding marker at:", lat, lon)
            markerModel.append({ latitude: lat, longitude: lon })
        }

        function removeMarker(lat, lon) {
            console.log("Removing marker at:", lat, lon)
            for (var i = 0; i < markerModel.count; i++) {
                var item = markerModel.get(i)
                if (item.latitude === lat && item.longitude === lon) {
                    markerModel.remove(i)
                    break
                }
            }
        }

        function removeAll() {
            console.log("Removing all markers and info labels")
            markerModel.clear()
            infoModel.clear()
        }

        function addInfo(lat, lon, newText) {
            console.log("Adding info at:", lat, lon, newText)
            for (var i = 0; i < infoModel.count; i++) {
                var item = infoModel.get(i)
                if (item.latitude === lat && item.longitude === lon) {
                    infoModel.setProperty(i, "text", item.text + "\n" + newText)
                     console.log("Updated infoModel at index", i, "with text:", infoModel.get(i).text)
                    return
                }
            }
            infoModel.append({ latitude: lat, longitude: lon, text: newText })
            console.log("Appended new info:", infoModel.get(infoModel.count - 1))
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
                        console.log("Updated infoModel at index", i, "with text:", lines.join("\n"))
                    } else {
                        infoModel.remove(i)
                        console.log("Removed infoModel at index", i, "because it was empty")
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
                        font.pixelSize: 18
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
    }

    //for showing a walking route on the map
    // Connections {
    //     target: apiMap
    //     onRouteReceivedQml: function(route) {
    //         console.log("Received route with length:", route.length)
    //         routeLine.path = route
    //         if (route.length > 0) {
    //             map.center = route[0]
    //         }
    //     }
    // }
}
