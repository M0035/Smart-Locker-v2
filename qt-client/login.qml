import QtQuick
import QtQuick.Controls

Item {
    id:logicView

    Image {
        id: login_image
        width: parent.width
        fillMode: Image.PreserveAspectFit
        Component.onCompleted: {
            login_image.source = "qrc:/new/prefix1/C++遍历容器最后一个元素.png"
        }
    }
    Column {
        spacing: spacing_value
        anchors.centerIn: parent
        width: parent.width*0.8
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
            id:passwordFiled
            width: parent.width
            placeholderText: "输入密码"
            echoMode: TextInput.Password
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize:text_size
        }

        Row {
            spacing: spacing_value
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                text: "登 陆"
                enabled: stack.depth > 1
                font.pixelSize:text_size
                onClicked: {
                    if(phoneNumberField.text==="" || passwordFiled.text===""){
                        receivedMessage.text="手机号或密码不能为空"
                    }else if(phoneNumberField.text.length !==11){
                        receivedMessage.text="手机号格式不对"

                    }else{
                        receivedMessage.text="发送消息"
                        udpClient.sendMessage('l'+phoneNumberField.text+'\0'+passwordFiled.text+'\0',host,port)
                        //stack.push("qrc:/deliver.qml")
                    }
                }
            }
            Button {
                text: "取 消"
                enabled: stack.depth > 1
                font.pixelSize:text_size
                onClicked: stack.pop()

            }
            Button {
                text: "注 册"
                enabled: stack.depth > 1
                font.pixelSize:text_size
                onClicked: stack.push("qrc:/register.qml")
            }


        }
        Text{
            id:receivedMessage
            text:"Received message: "
        }
        Connections{
            target: udpClient
            function onMsgLogin(message){
                receivedMessage.text="Received message: "+message
                if(message==="ok")
                    stack.push("qrc:/deliver.qml")

            }
        }
    }
}
