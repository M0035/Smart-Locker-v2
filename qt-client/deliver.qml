import QtQuick
import QtQuick.Controls

Item {
    id:deliverView
    property string cabinet_size: '0'
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
            id:parceField
            width: parent.width
            placeholderText: "输入包裹号"
            anchors.horizontalCenter: parent.horizontalCenter
            inputMethodHints: Qt.ImhDialableCharactersOnly
            font.pixelSize:text_size
        }
        TextField{
            id:phoneNumberField
            width: parent.width
            placeholderText: "输入手机号码"
            validator: RegularExpressionValidator {
                regularExpression: /^[0-9]{11}$/
            }
            anchors.horizontalCenter: parent.horizontalCenter
            inputMethodHints: Qt.ImhDialableCharactersOnly
            font.pixelSize:text_size
        }
        TextField{
            id:phoneNumberField2
            width: parent.width
            placeholderText: "确认手机号码"
            validator: RegularExpressionValidator {
                regularExpression: /^[0-9]{11}$/
            }
            anchors.horizontalCenter: parent.horizontalCenter
            inputMethodHints: Qt.ImhDialableCharactersOnly
            font.pixelSize:text_size
        }
        Row{
            RadioButton{
                id:cabinet_small
                font.pixelSize: text_size
                text:"小柜"
                checked:true
                onClicked: cabinet_size='0'
            }
            RadioButton{
                id:cabinet_medium
                font.pixelSize: text_size
                text:"中柜"
                onClicked: cabinet_size='1'
            }
            RadioButton{
                id:cabinet_large
                font.pixelSize: text_size
                text:"大柜"
                onClicked: cabinet_size='2'
            }
        }
        Row {
            spacing: 10

            Button {
                font.pixelSize: text_size
                text: "投 递"
                onClicked: {
                    if(phoneNumberField.text==="" || parceField.text===""){
                        receivedMessage.text="手机号或包裹号不能为空"
                    }else if(phoneNumberField.text.length !==11){
                        receivedMessage.text="手机号格式不对"
                    }else if(phoneNumberField.text !== phoneNumberField2.text){
                        receivedMessage.text="两次输入的手机号不一致"
                    }else{
                        receivedMessage.text="发送消息"
                        udpClient.sendMessage('d'+cabinet_size+parceField.text+'\0'+phoneNumberField.text+'\0',host,port)
                    }
                }
            }
            Button {
                text: "取 消"
                font.pixelSize: text_size
                enabled: stack.depth > 1
                onClicked: stack.pop()
            }
            Text{
                id:receivedMessage
                text:"Received message: "
            }
            Connections{
                target: udpClient
                function onMsgDeliver(message){
                    receivedMessage.text="Received message: "+message
                }
            }
        }

    }
}
