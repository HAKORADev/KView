import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Pdf
import org.kde.kirigami as Kirigami

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: "KView - " + fileHandler.fileName
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "#2b2b2b"

    // Set position to middle
    Component.onCompleted: {
        x = (Screen.width - width) / 2
        y = (Screen.height - height) / 2
    }

    Shortcut {
        sequence: "Escape"
        onActivated: root.close()
    }

    Shortcut {
        sequence: "Return"
        onActivated: {
            fileHandler.openFileExternally()
            closeTimer.restart()
        }
    }

    Shortcut {
        sequence: "Enter"
        onActivated: {
            fileHandler.openFileExternally()
            closeTimer.restart()
        }
    }

    Timer {
        id: closeTimer
        interval: 150
        onTriggered: root.close()
    }

    Shortcut {
        sequence: "Left"
        onActivated: fileHandler.previousFile()
    }

    Shortcut {
        sequence: "Right"
        onActivated: fileHandler.nextFile()
    }

    Timer {
        id: hideTimer
        interval: 3000
        onTriggered: infoPopup.opacity = 0
    }

    Connections {
        target: fileHandler
        function onMessageRequested(message) {
            infoLabel.text = message
            infoPopup.opacity = 1
            hideTimer.restart()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "#3daee9"
        border.width: 1

        Loader {
            id: contentLoader
            anchors.fill: parent
            anchors.margins: 10
            focus: true
            onLoaded: {
                if (item) item.forceActiveFocus()
            }
            sourceComponent: {
                let type = fileHandler.fileType
                if (type.startsWith("image/")) return imageComponent
                if (type.startsWith("video/")) return videoComponent
                if (type.startsWith("audio/")) return audioComponent
                if (type.startsWith("text/")) return textComponent
                if (type === "application/pdf") return pdfComponent
                if (type.includes("zip") || type.includes("tar") || type.includes("7z")) return archiveComponent
                return unsupportedComponent
            }
        }

        Rectangle {
            id: infoPopup
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 20
            width: infoLabel.implicitWidth + 40
            height: infoLabel.implicitHeight + 20
            color: "#CC000000"
            radius: 10
            opacity: 0
            Behavior on opacity { NumberAnimation { duration: 200 } }

            Label {
                id: infoLabel
                anchors.centerIn: parent
                color: "white"
                font.pointSize: 10
            }
        }
    }

    Component {
        id: imageComponent
        Image {
            source: fileHandler.fileUrl
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }
    }

    Component {
        id: videoComponent
        Video {
            id: video
            source: fileHandler.fileUrl
            autoPlay: true
            fillMode: VideoOutput.PreserveAspectFit
            loops: MediaPlayer.Infinite

            Shortcut {
                sequence: "Space"
                enabled: contentLoader.status === Loader.Ready
                onActivated: video.playbackState === MediaPlayer.PlayingState ? video.pause() : video.play()
            }
            Shortcut {
                sequence: "E"
                enabled: contentLoader.status === Loader.Ready
                onActivated: video.position = Math.min(video.duration, video.position + video.duration * 0.1)
            }
            Shortcut {
                sequence: "Q"
                enabled: contentLoader.status === Loader.Ready
                onActivated: video.position = Math.max(0, video.position - video.duration * 0.1)
            }
        }
    }

    Component {
        id: audioComponent
        ColumnLayout {
            spacing: 20
            Label {
                text: "Audio Preview"
                color: "white"
                font.pointSize: 14
                Layout.alignment: Qt.AlignCenter
            }
            MediaPlayer {
                id: mediaPlayer
                audioOutput: AudioOutput {}
                source: fileHandler.fileUrl
                autoPlay: true
            }
            Slider {
                Layout.fillWidth: true
                value: mediaPlayer.position
                to: mediaPlayer.duration
                enabled: false
            }

            Shortcut {
                sequence: "Space"
                enabled: contentLoader.status === Loader.Ready
                onActivated: mediaPlayer.playbackState === MediaPlayer.PlayingState ? mediaPlayer.pause() : mediaPlayer.play()
            }
            Shortcut {
                sequence: "E"
                enabled: contentLoader.status === Loader.Ready
                onActivated: mediaPlayer.position = Math.min(mediaPlayer.duration, mediaPlayer.position + mediaPlayer.duration * 0.1)
            }
            Shortcut {
                sequence: "Q"
                enabled: contentLoader.status === Loader.Ready
                onActivated: mediaPlayer.position = Math.max(0, mediaPlayer.position - mediaPlayer.duration * 0.1)
            }
        }
    }

    Component {
        id: textComponent
        ScrollView {
            contentWidth: -1
            TextArea {
                text: fileHandler.content
                readOnly: true
                color: "white"
                background: null
                font.family: "monospace"
                wrapMode: TextEdit.Wrap
            }
        }
    }

    Component {
        id: pdfComponent
        PdfMultiPageView {
            document: PdfDocument { source: fileHandler.fileUrl }
        }
    }

    Component {
        id: archiveComponent
        ColumnLayout {
            spacing: 10
            anchors.fill: parent
            Label {
                text: "archive: " + fileHandler.fileName + "\nsize = " + fileHandler.fileSize + "\n\npress enter to view"
                color: "white"
                font.pointSize: 12
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: fileHandler.archiveContents
                delegate: ItemDelegate {
                    width: parent.width
                    contentItem: Label {
                        text: modelData
                        color: "white"
                    }
                    background: null
                }
            }
        }
    }

    Component {
        id: unsupportedComponent
        Label {
            text: "unsupported file format: " + fileHandler.fileName + "\n\npress enter to view if possible"
            color: "white"
            font.pointSize: 12
            horizontalAlignment: Text.AlignHCenter
            anchors.centerIn: parent
        }
    }
}
