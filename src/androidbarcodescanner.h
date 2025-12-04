#pragma once

#include <QObject>
#include <qqmlintegration.h>

#ifdef Q_OS_ANDROID
#include <QJniEnvironment>
#include <QJniObject>
#endif

class AndroidBarcodeScanner : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)
    Q_PROPERTY(ScannerState state READ state NOTIFY stateChanged)

public:
    enum class ScannerState { Idle, Scanning, Processing, Error };
    Q_ENUM(ScannerState)

    explicit AndroidBarcodeScanner(QObject *parent = nullptr);
    ~AndroidBarcodeScanner() override;

    Q_INVOKABLE void startScan();

    bool isScanning() const { return m_state == ScannerState::Scanning; }
    ScannerState state() const { return m_state; }

signals:
    void scanResult(const QString &code, const QString &format);
    void scanCancelled();
    void scanError(const QString &error);
    void isScanningChanged();
    void stateChanged();

private:
    void setState(ScannerState state);

#ifdef Q_OS_ANDROID
    static void onScanResultNative(JNIEnv *env, jobject, jlong nativePtr, jstring code, jstring format);
    static void onScanCancelledNative(JNIEnv *env, jobject, jlong nativePtr);
    static void onScanErrorNative(JNIEnv *env, jobject, jlong nativePtr, jstring error);
#endif

    ScannerState m_state = ScannerState::Idle;
};
