import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Qt Multithread Demo")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Label {
            text: "Qt Multithread Demo"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        GroupBox {
            title: "Thread Configuration"
            Layout.fillWidth: true

            GridLayout {
                columns: 2
                anchors.fill: parent

                Label { text: "Number of Threads:" }
                SpinBox {
                    id: numThreadsSpinBox
                    from: 1
                    to: 16
                    value: 4
                    Layout.fillWidth: true
                    enabled: !taskManager.isRunning
                }

                Label { text: "Iterations per Thread:" }
                SpinBox {
                    id: iterationsSpinBox
                    from: 1
                    to: 1000
                    value: 20
                    Layout.fillWidth: true
                    enabled: !taskManager.isRunning
                }

                Label { text: "Delay (ms):" }
                SpinBox {
                    id: delaySpinBox
                    from: 10
                    to: 1000
                    value: 100
                    stepSize: 10
                    Layout.fillWidth: true
                    enabled: !taskManager.isRunning
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Button {
                text: taskManager.isRunning ? "Stop" : "Start"
                Layout.fillWidth: true

                onClicked: {
                    if (taskManager.isRunning) {
                        taskManager.stopAllTasks();
                    } else {
                        taskManager.startTask(
                            numThreadsSpinBox.value,
                            iterationsSpinBox.value,
                            delaySpinBox.value
                        );
                    }
                }
            }
        }

        GroupBox {
            title: "Progress"
            Layout.fillWidth: true

            ColumnLayout {
                anchors.fill: parent

                ProgressBar {
                    id: progressBar
                    value: taskManager.progress / 100
                    Layout.fillWidth: true
                }

                Label {
                    text: taskManager.progress + "%"
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        GroupBox {
            title: "Results"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent

                TextArea {
                    text: taskManager.result
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                }
            }
        }
    }
}
