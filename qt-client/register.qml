import QtQuick
import QtQuick.Controls

Item {
    id:registerView
    property string company: ""
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
        TextField{
            id:passwordFiled2
            width: parent.width
            placeholderText: "请确认密码"
            echoMode: TextInput.Password
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize:text_size
        }
        ComboBox{
            id:courierSelecetor
            width:200
            //使用列表模型
            ListModel{
                id:courierList
                ListElement{code:"JD";name:"京东快递"}
                ListElement { code: "SF";  name: "顺丰速运" }
                ListElement { code: "YTO"; name: "圆通速递" }
                ListElement { code: "ZTO"; name: "中通快递" }
                ListElement { code: "STO"; name: "申通快递" }
                ListElement { code: "YUNDA"; name: "韵达速递" }
                ListElement { code: "EMS"; name: "中国邮政" }
                ListElement { code: "JTSD"; name: "极兔速递" }
                ListElement { code: "DBL"; name: "德邦快递" }
                ListElement { code: "UC";  name: "优速快递" }
                ListElement { code: "TTKD"; name: "天天快递" }
                ListElement { code: "ZJS"; name: "宅急送" }
            }
            //设置模型和文本
            model:courierList
            textRole: "name"

            onCurrentIndexChanged: {
            company=courierList.get(currentIndex).name
            }
        }
        Row {
            spacing: 10

            Button {
                text: "注 册"
                font.pixelSize:text_size
                enabled: stack.depth > 1
                onClicked: {
                    if(phoneNumberField.text==="" || passwordFiled.text===""||passwordFiled2.text===""){
                        receivedMessage.text="注册信息不能为空"
                    }else if(phoneNumberField.text.length !==11){
                        receivedMessage.text="手机号格式不对"
                    }else if(passwordFiled.text!==passwordFiled2.text){
                        receivedMessage.text="两次密码不一致"
                    }else{
                        receivedMessage.text="发送消息"
                        udpClient.sendMessage('r'+phoneNumberField.text+'\0'+passwordFiled.text+'\0'+company+'\0',host,port)
                        stack.push("qrc:/login.qml")
                    }


                }
            }
            Button {
                text: "取 消"
                font.pixelSize:text_size
                enabled: stack.depth > 1
                onClicked: stack.pop()
            }
            Text{
                id:receivedMessage
                text:"Received message: "
            }
                    Connections{
                        target: udpClient
                        function onMsgRegister(message){
                            receivedMessage.text="Received message: "+message
                        }
                    }

        }
    }
}
