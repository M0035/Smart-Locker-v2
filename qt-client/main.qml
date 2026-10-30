import QtQuick
import QtQuick.Controls
import QtQuick.VirtualKeyboard

Window {
    id: window
    width: 480
    height: 854
    visible: true

    property int text_size: 25
    property int spacing_value: 10
    property string host: "127.0.0.1"
    property int port: 8888

    StackView {
        id: stack
        initialItem: mainView
        anchors.fill: parent
    }

    Component {
        id: mainView

        Column {
            spacing: spacing_value

            Image {
                id: deliver_image
                width: parent.width
                fillMode: Image.PreserveAspectFit
                Component.onCompleted: {
                    deliver_image.source = "qrc:/new/prefix1/C++遍历容器最后一个元素.png"
                }
            }
            Button {
                text: "投 递"
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width*0.5
                contentItem: Text {
                    id: button_deliver
                    text: qsTr("投 递")
                    font.pixelSize: text_size
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: stack.push("qrc:/login.qml")
            }
            Button {
                text: "取 件"
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width*0.5
                contentItem: Text {
                    id: button_pickup
                    text: qsTr("取 件")
                    font.pixelSize: text_size
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: stack.push("qrc:/pickup.qml")
            }
            //            Button {
            //                text: "取 件"
            //                enabled: stack.depth > 1
            //                onClicked: stack.push()
            //        }

        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
