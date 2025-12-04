package com.example.barcodescanner;

import android.content.Intent;
import android.os.Bundle;
import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {
    private static MainActivity instance;
    private BarcodeScanner barcodeScanner;
    private static final int REQUEST_CODE_SCAN = 49374;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        instance = this;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        instance = null;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_CODE_SCAN) {
            if (barcodeScanner != null) {
                barcodeScanner.handleActivityResult(requestCode, resultCode, data);
            }
        } else {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

    public static void createScanner(long nativePtr) {
        if (instance != null) {
            instance.barcodeScanner = new BarcodeScanner(instance, nativePtr);
        }
    }

    public static void startScan() {
        if (instance != null && instance.barcodeScanner != null) {
            instance.barcodeScanner.startScan();
        }
    }

    public static int getRequestCodeScan() {
        return REQUEST_CODE_SCAN;
    }
}
