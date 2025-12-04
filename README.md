# Qt Barcode & QR Code Scanner for Android

A production-ready barcode and QR code scanner for Qt 6.10.1+ Android applications using JNI integration with ZXing Android Embedded.

## Features

- Scans multiple barcode formats: QR Code, EAN, Code 128, Code 39, Data Matrix, PDF417, and more
- Native Android scanning UI with professional UX
- Thread-safe JNI callbacks
- QML singleton for easy integration
- Real-time scanning state management
- Supports Android API Level 24+

## Requirements

- Qt 6.10.1 or later
- Android SDK (API Level 24+)
- Android NDK
- Java 11 or later

## Project Structure

```
SquaredIoTScanner/
├── src/
│   ├── androidbarcodescanner.h      # C++ header for JNI bridge
│   └── androidbarcodescanner.cpp    # C++ implementation
├── android/
│   ├── build.gradle                 # Android dependencies
│   └── src/com/example/barcodescanner/
│       ├── MainActivity.java         # Activity lifecycle management
│       └── BarcodeScanner.java      # ZXing wrapper
├── Main.qml                         # Example UI
├── main.cpp                         # Application entry point
└── CMakeLists.txt                   # Build configuration
```

## Setup

### 1. Clone or Copy

Copy this project to your development environment.

### 2. Update Package Name (Optional)

If you want to use a custom package name instead of `com.example.barcodescanner`:

1. Rename the Java package directory in `android/src/`
2. Update package declarations in `MainActivity.java` and `BarcodeScanner.java`
3. Update the package references in `src/androidbarcodescanner.cpp`:
   ```cpp
   constexpr const char *JAVA_MAIN_ACTIVITY = "your/package/name/MainActivity";
   constexpr const char *JAVA_SCANNER_CLASS = "your/package/name/BarcodeScanner";
   ```

### 3. Build and Run

Open the project in Qt Creator and build for Android. The necessary ZXing dependencies will be downloaded automatically via Gradle.

## Usage

### Basic QML Integration

```qml
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    Button {
        text: "Scan Barcode"
        enabled: !AndroidBarcodeScanner.isScanning
        onClicked: AndroidBarcodeScanner.startScan()
    }

    Connections {
        target: AndroidBarcodeScanner

        function onScanResult(code, format) {
            console.log("Scanned:", code, "Format:", format)
            // Handle your scanned code here
        }

        function onScanCancelled() {
            console.log("User cancelled scan")
        }

        function onScanError(error) {
            console.error("Error:", error)
        }
    }
}
```

### Properties

- `isScanning: bool` - True when scanner is active
- `state: ScannerState` - Current scanner state (Idle, Scanning, Processing, Error)

### Methods

- `startScan()` - Launch the scanner

### Signals

- `scanResult(code: string, format: string)` - Emitted when a code is successfully scanned
- `scanCancelled()` - Emitted when user cancels scanning
- `scanError(error: string)` - Emitted on scanner errors

## Architecture

This implementation uses a three-layer architecture:

1. **QML Layer**: User interface and business logic
2. **C++ Layer**: JNI bridge between Qt and Java, thread-safe callbacks
3. **Java Layer**: ZXing Android Embedded integration, camera management

Data flows from QML → C++ → Java → ZXing → Java → C++ → QML with proper thread marshalling to ensure thread safety.

## Dependencies

The project uses:

- [ZXing Android Embedded 4.3.0](https://github.com/journeyapps/zxing-android-embedded) - Barcode scanning library
- [ZXing Core 3.5.3](https://github.com/zxing/zxing) - Core barcode processing

These are automatically resolved by Gradle from Maven Central.

## Limitations

- **Android only**: This implementation is specific to Android
- **UI customization**: Uses ZXing's built-in scanning UI (not customizable from QML)
- **Activity lifecycle**: Static instance pattern may need enhancement for complex lifecycle scenarios

## License

This code is provided as-is for educational and commercial use.

ZXing and ZXing Android Embedded are licensed under Apache License 2.0.

## Related Resources

- [Blog Post](https://squared.co.ke/blog) - Detailed explanation of the implementation
- [ZXing Android Embedded](https://github.com/journeyapps/zxing-android-embedded)
- [Qt JNI Documentation](https://doc.qt.io/qt-6/qtcore-platform-androidnotifier-example.html)

## Contributing

This is a reference implementation. Feel free to adapt it for your needs:

- Customize scan options in `BarcodeScanner.java`
- Add specific barcode format filters
- Implement additional error handling
- Enhance lifecycle management

## Support

For issues and questions:
- Qt-related: [Qt Forum](https://forum.qt.io/)
- ZXing-related: [ZXing Android Embedded Issues](https://github.com/journeyapps/zxing-android-embedded/issues)

## Acknowledgments

Built for Qt 6.10.1 targeting Android API Level 35. Based on the need for reliable barcode scanning when pure Qt solutions faced compatibility challenges with Qt 6's multimedia API changes.
