import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.3

import it.videx.label 1.0

ApplicationWindow {
    id:app
    visible: true
    width: imageLoader.width
    height: imageLoader.height

    minimumHeight: 480
    minimumWidth: 640
    title: "Image Labeling Viewer"

    FileDialog {
        id: imageFileDialog
        title: "Seleziona un'immagine"
        nameFilters: ["Immagini (*.jpg *.png)"]
        onAccepted: {
            ImageLabeler.imageSource = fileUrl.toString()
            ImageLabeler.labelFilePath = fileUrl.toString().replace("file:///", "").replace("qrc:/", "").replace("images","label").replace(".jpg",".txt")
            ImageLabeler.loadLabels()
        }
    }

    FileDialog {
        id: labelFileDialog
        title: "Seleziona il file di etichette"
        nameFilters: ["File di testo (*.txt)"]
        onAccepted: {
            ImageLabeler.labelFilePath = fileUrl.toString().replace("file:///", "").replace("qrc:/", "")
            ImageLabeler.loadLabels()
        }
    }

    Button {
        id:selectImageButton
        text: "Seleziona un'immagine"
        anchors.top: parent.top
        anchors.left: parent.left

        height: 48
        z:10000

        visible: true

        onClicked:{
            imageFileDialog.open()
        }
    }

    Button {
        height: 48
        visible: false
        anchors.top: parent.top
        anchors.left: selectImageButton.right
        text: "Seleziona il file di etichette"
        onClicked: labelFileDialog.open()
        z:10000
    }


    Repeater {
        id: labelListView
        model: ImageLabeler
        delegate: Rectangle {
            width: element_width*imageLoader.width
            height: element_height*imageLoader.height
            color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1.0)
            x: (element_x * imageLoader.width)-(width/2);
            y: (element_y * imageLoader.height)-(height/2);
            z:100
            opacity: 0.5
            visible: true
        }
    }

    Image {
        id: imageLoader
        anchors.fill: parent
        source: ImageLabeler.imageSource
        onSourceChanged: {
            ImageLabeler.loadLabels()
        }
    }
}
