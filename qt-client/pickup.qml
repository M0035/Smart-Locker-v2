import QtQuick
import QtQuick.Controls

Item {
    id:pickupView
    Image {
        id: deliver_image
        width: parent.width
        fillMode: Image.PreserveAspectFit
        Component.onCompleted: {
            deliver_image.source = "qrc:/new/prefix1/C++遍历容器最后一个元素.png"
        }
    }
    Column{
        spacing: spacing_value
        anchors.centerIn: parent
        width: parent.width*0.8
        TextField{
            id:codeNumberField
            width: parent.width
            placeholderText: "输入取件码"
            validator: RegularExpressionValidator {
                regularExpression: /^[0-9]{8}$/
            }
            anchors.horizontalCenter: parent.horizontalCenter
            inputMethodHints: Qt.ImhDialableCharactersOnly
            font.pixelSize:text_size
        }
        Row {
            spacing: 10

            Button {
                text: "取 件"
                enabled: stack.depth > 1
                font.pixelSize:text_size
                onClicked: {
                    if(codeNumberField.text===""){
                        receivedMessage.text="取件码不能为空"
                    }else if(codeNumberField.text.length !==8){
                        receivedMessage.text="取件码位数不够"
                    }else{
                        receivedMessage.text="发送消息"
                        udpClient.sendMessage('p'+codeNumberField.text+'\0',host,port)
                    }
                    //stack.pop()
                }
            }
            Button {
                text: "取 消"
                enabled: stack.depth > 1
                font.pixelSize:text_size
                onClicked :{
                   // udpClient.sendMessage('p'+codeNumberField.text+'\0',host,port)
                    stack.pop()

                }
            }
            Text{
                id:receivedMessage
                text:"Received message: "
            }
            Connections{
                target: udpClient
                function onMsgPickup(message){
                    receivedMessage.text="Received message: "+message
                }
            }
        }

    }
}
