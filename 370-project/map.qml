import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15


Rectangle {
    anchors.fill: parent

    Plugin {
        id: osmPlugin
        name: "osm"
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


        //used to draw the returned API route coordinates
        // MapPolyline {
        //             line.width: 5
        //             line.color: 'blue'

        //             path: [
        //             ]
        //         }

        MouseArea {
            anchors.fill: parent
            drag.target: null

            onPressed: {
                map.lastMousePos = Qt.point(mouse.x, mouse.y)
                map.dragging = true
            }
            onReleased: map.dragging = false
            onCanceled: map.dragging = false
            onExited: map.dragging = false


            onPositionChanged: {
                if (map.dragging) {
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

        //currently figuring out how to zoom in and out of map
        // WheelHandler {
        // }
    }

}

