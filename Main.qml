import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SquaredIoTScanner

ApplicationWindow {
    id: root
    width: 400
    height: 600
    visible: true
    title: "Barcode Scanner Example"

    property string scannedCode: ""
    property string scannedFormat: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Label {
            text: "Qt Barcode & QR Code Scanner"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            id: resultDisplay
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            color: "#f0f0f0"
            radius: 10
            border.color: "#cccccc"
            border.width: 1

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10

                Label {
                    text: root.scannedCode ? "Scanned Code:" : "No code scanned yet"
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: root.scannedCode || ""
                    font.pixelSize: 14
                    font.bold: true
                    color: "#2196F3"
                    Layout.alignment: Qt.AlignHCenter
                    wrapMode: Text.Wrap
                    Layout.maximumWidth: resultDisplay.width - 40
                }

                Label {
                    text: root.scannedFormat ? `Format: ${root.scannedFormat}` : ""
                    font.pixelSize: 12
                    color: "#666666"
                    Layout.alignment: Qt.AlignHCenter
                    visible: root.scannedFormat
                }
            }
        }

        Button {
            text: AndroidBarcodeScanner.isScanning ? "Scanning..." : "Scan Barcode"
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            enabled: !AndroidBarcodeScanner.isScanning
            font.pixelSize: 18

            onClicked: AndroidBarcodeScanner.startScan()
        }

        Label {
            text: "Supports: QR Code, EAN, Code 128, and more"
            font.pixelSize: 12
            color: "#666666"
            Layout.alignment: Qt.AlignHCenter
        }

        Item {
            Layout.fillHeight: true
        }

        Label {
            id: statusLabel
            text: "Status: " + statusLabel.getStatusText()
            font.pixelSize: 12
            color: statusLabel.getStatusColor()
            Layout.alignment: Qt.AlignHCenter

            function getStatusText() {
                switch (AndroidBarcodeScanner.state) {
                case AndroidBarcodeScanner.ScannerState.Idle:
                    return "Ready"
                case AndroidBarcodeScanner.ScannerState.Scanning:
                    return "Scanning..."
                case AndroidBarcodeScanner.ScannerState.Processing:
                    return "Processing..."
                case AndroidBarcodeScanner.ScannerState.Error:
                    return "Error"
                default:
                    return "Unknown"
                }
            }

            function getStatusColor() {
                switch (AndroidBarcodeScanner.state) {
                case AndroidBarcodeScanner.ScannerState.Idle:
                    return "#4CAF50"
                case AndroidBarcodeScanner.ScannerState.Scanning:
                    return "#2196F3"
                case AndroidBarcodeScanner.ScannerState.Error:
                    return "#F44336"
                default:
                    return "#666666"
                }
            }
        }
    }

    Connections {
        target: AndroidBarcodeScanner

        function onScanResult(code, format) {
            root.scannedCode = code
            root.scannedFormat = format
            console.log("Scanned:", code, "Format:", format)
        }

        function onScanCancelled() {
            console.log("Scan cancelled by user")
        }

        function onScanError(error) {
            console.error("Scan error:", error)
            root.scannedCode = ""
            root.scannedFormat = ""
        }
    }
}
