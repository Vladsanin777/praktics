import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QtEasy.TitlesBars

Rectangle {
    id: root
    width: 500
    height: 500
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        QTitleBarFilesOperations {
            id: titleBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            
            title: qsTr("Text editor")
            openFileText: qsTr("Open file")
            saveFileText: qsTr("Save file")
            saveAsFileText: qsTr("Save as file")
            filter: "HTML Files (*.html)"

            onOpened: {
                var content = titleBar.read()
                if (content !== "") {
                    textArea.text = content
                }
            }
            onSaved: {
                titleBar.write(textArea.text) // textArea.text возвращает HTML, если текст форматированный
            }
        }

        // 2. Панель форматирования текста (ToolBar)
        ToolBar {
            id: toolBar
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            Layout.minimumWidth: 500
            
            background: Rectangle { color: "#f0f0f0" } // Базовая стилизация взамен QSS

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                Button {
                    text: "B"
                    font.bold: true
                    font.family: "Arial"
                    font.pointSize: 10
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 30
                    checkable: true
                    checked: textArea.font.bold
                    onClicked: textArea.font.bold = !textArea.font.bold

                    Shortcut {
                        sequence: StandardKey.Bold
                        onActivated: textArea.font.bold = !textArea.font.bold
                    }
                }

                Button {
                    text: "I"
                    font.italic: true
                    font.family: "Arial"
                    font.pointSize: 10
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 30
                    checkable: true
                    checked: textArea.font.italic
                    onClicked: textArea.font.italic = !textArea.font.italic

                    Shortcut {
                        sequence: StandardKey.Italic
                        onActivated: textArea.font.italic = !textArea.font.italic
                    }
                }

                Button {
                    text: qsTr("Color")
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 30
                    onClicked: colorDialog.open()
                }

                ComboBox {
                    id: fontBox
                    Layout.preferredHeight: 30
                    Layout.fillWidth: true
                    model: Qt.fontFamilies()
                    currentIndex: model.indexOf("Arial")
                    onCurrentTextChanged: textArea.font.family = currentText
                }

                ComboBox {
                    id: sizeBox
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 70
                    model: ["8", "10", "12", "14", "16", "18", "24", "36", "48", "72"]
                    currentIndex: 2
                    onCurrentTextChanged: textArea.font.pointSize = parseFloat(currentText)
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: textArea
                textFormat: TextEdit.RichText
                selectByMouse: true
                font.family: "Arial"
                font.pointSize: 12
                focus: true
                tabChangesFocus: true

                placeholderText: qsTr("Start typing here...")
            }
        }
    }

    ColorDialog {
        id: colorDialog
        title: qsTr("Select text color")
        selectedColor: "black"
        onAccepted: {
            textArea.color = selectedColor
        }
    }
}
