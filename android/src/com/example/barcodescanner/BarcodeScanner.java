package com.example.barcodescanner;

import android.app.Activity;
import android.content.Intent;
import com.journeyapps.barcodescanner.CaptureActivity;
import com.journeyapps.barcodescanner.ScanOptions;
import com.journeyapps.barcodescanner.ScanIntentResult;

public class BarcodeScanner {
    private final Activity activity;
    private final long nativePtr;

    public BarcodeScanner(Activity activity, long nativePtr) {
        this.activity = activity;
        this.nativePtr = nativePtr;
    }

    public void startScan() {
        try {
            ScanOptions options = new ScanOptions();
            options.setDesiredBarcodeFormats(ScanOptions.ALL_CODE_TYPES);
            options.setPrompt("Scan QR Code or Barcode");
            options.setCameraId(0);
            options.setBeepEnabled(true);
            options.setBarcodeImageEnabled(false);
            options.setOrientationLocked(true);
            options.setCaptureActivity(CaptureActivity.class);

            Intent intent = options.createScanIntent(activity);
            activity.startActivityForResult(intent, MainActivity.getRequestCodeScan());
        } catch (Exception e) {
            onScanError(nativePtr, "Failed to start scanner: " + e.getMessage());
        }
    }

    public void handleActivityResult(int requestCode, int resultCode, Intent data) {
        ScanIntentResult result = ScanIntentResult.parseActivityResult(resultCode, data);

        if (result != null) {
            if (result.getContents() == null) {
                onScanCancelled(nativePtr);
            } else {
                String format = result.getFormatName() != null ? result.getFormatName() : "UNKNOWN";
                onScanResult(nativePtr, result.getContents(), format);
            }
        } else {
            onScanCancelled(nativePtr);
        }
    }

    private static native void onScanResult(long nativePtr, String code, String format);
    private static native void onScanCancelled(long nativePtr);
    private static native void onScanError(long nativePtr, String error);
}
