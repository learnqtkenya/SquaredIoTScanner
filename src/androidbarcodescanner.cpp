#include "androidbarcodescanner.h"
#include <QDebug>

namespace {
constexpr const char *JAVA_MAIN_ACTIVITY = "com/example/barcodescanner/MainActivity";
constexpr const char *JAVA_SCANNER_CLASS = "com/example/barcodescanner/BarcodeScanner";
}

AndroidBarcodeScanner::AndroidBarcodeScanner(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_ANDROID
    JNINativeMethod methods[] = {
        { "onScanResult", "(JLjava/lang/String;Ljava/lang/String;)V",
          reinterpret_cast<void *>(onScanResultNative) },
        { "onScanCancelled", "(J)V",
          reinterpret_cast<void *>(onScanCancelledNative) },
        { "onScanError", "(JLjava/lang/String;)V",
          reinterpret_cast<void *>(onScanErrorNative) }
    };

    QJniEnvironment env;
    if (auto clazz = env.findClass(JAVA_SCANNER_CLASS)) {
        env.registerNativeMethods(clazz, methods, 3);
    } else {
        qWarning() << "Failed to find BarcodeScanner class";
    }

    QJniObject::callStaticMethod<void>(
        JAVA_MAIN_ACTIVITY, "createScanner", "(J)V", reinterpret_cast<jlong>(this));
#endif
}

AndroidBarcodeScanner::~AndroidBarcodeScanner() {}

void AndroidBarcodeScanner::startScan()
{
#ifdef Q_OS_ANDROID
    if (m_state == ScannerState::Scanning) {
        qDebug() << "Scanner already active";
        return;
    }

    setState(ScannerState::Scanning);
    QJniObject::callStaticMethod<void>(JAVA_MAIN_ACTIVITY, "startScan", "()V");
#else
    emit scanError("Barcode scanning only available on Android");
#endif
}

void AndroidBarcodeScanner::setState(ScannerState state)
{
    if (m_state == state)
        return;

    bool wasScanningBefore = isScanning();
    m_state = state;
    emit stateChanged();

    if (wasScanningBefore != isScanning()) {
        emit isScanningChanged();
    }
}

#ifdef Q_OS_ANDROID
void AndroidBarcodeScanner::onScanResultNative(
    JNIEnv *env, jobject, jlong nativePtr, jstring code, jstring format)
{
    auto *scanner = reinterpret_cast<AndroidBarcodeScanner *>(nativePtr);
    if (!scanner)
        return;

    const char *codeStr = env->GetStringUTFChars(code, nullptr);
    const char *formatStr = env->GetStringUTFChars(format, nullptr);

    QString qCode = QString::fromUtf8(codeStr);
    QString qFormat = QString::fromUtf8(formatStr);

    env->ReleaseStringUTFChars(code, codeStr);
    env->ReleaseStringUTFChars(format, formatStr);

    QMetaObject::invokeMethod(
        scanner,
        [scanner, qCode, qFormat]() {
            scanner->setState(ScannerState::Idle);
            emit scanner->scanResult(qCode, qFormat);
        },
        Qt::QueuedConnection);
}

void AndroidBarcodeScanner::onScanCancelledNative(JNIEnv *, jobject, jlong nativePtr)
{
    auto *scanner = reinterpret_cast<AndroidBarcodeScanner *>(nativePtr);
    if (!scanner)
        return;

    QMetaObject::invokeMethod(
        scanner,
        [scanner]() {
            scanner->setState(ScannerState::Idle);
            emit scanner->scanCancelled();
        },
        Qt::QueuedConnection);
}

void AndroidBarcodeScanner::onScanErrorNative(JNIEnv *env, jobject, jlong nativePtr, jstring error)
{
    auto *scanner = reinterpret_cast<AndroidBarcodeScanner *>(nativePtr);
    if (!scanner)
        return;

    const char *errorStr = env->GetStringUTFChars(error, nullptr);
    QString qError = QString::fromUtf8(errorStr);
    env->ReleaseStringUTFChars(error, errorStr);

    QMetaObject::invokeMethod(
        scanner,
        [scanner, qError]() {
            scanner->setState(ScannerState::Error);
            emit scanner->scanError(qError);
        },
        Qt::QueuedConnection);
}
#endif
