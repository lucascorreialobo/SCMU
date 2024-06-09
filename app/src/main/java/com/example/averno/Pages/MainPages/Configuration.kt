package com.example.averno.Pages.MainPages
import android.Manifest
import android.content.Context
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.tooling.preview.Preview
import com.google.accompanist.permissions.ExperimentalPermissionsApi
import com.google.accompanist.permissions.rememberMultiplePermissionsState


@OptIn(ExperimentalPermissionsApi::class)
@Composable
fun Configuration() {
    val context = LocalContext.current
    val permissionsState = rememberMultiplePermissionsState(
        permissions = listOf(
            Manifest.permission.CHANGE_WIFI_STATE,
            Manifest.permission.ACCESS_WIFI_STATE,
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION
        )
    )

    var ssid by remember { mutableStateOf(TextFieldValue("")) }
    var password by remember { mutableStateOf(TextFieldValue("")) }
    var status by remember { mutableStateOf("Not Connected") }

    LaunchedEffect(Unit) {
        permissionsState.launchMultiplePermissionRequest()
    }

    if (permissionsState.allPermissionsGranted) {
        Column(modifier = Modifier.fillMaxSize()) {
            // Input fields for SSID and password
            androidx.compose.material3.OutlinedTextField(
                value = ssid,
                onValueChange = { ssid = it },
                label = { Text("SSID") }
            )
            androidx.compose.material3.OutlinedTextField(
                value = password,
                onValueChange = { password = it },
                label = { Text("Password") }
            )
            Button(onClick = {
                connectToWifi(context, ssid.text, password.text) {
                    status = it
                }
            }) {
                Text("Connect")
            }
            Text("Status: $status")
        }
    } else {
        // Display a message requesting the user to grant permissions
        Text("Please grant all required permissions to proceed.")
    }
}

@Suppress("DEPRECATION")
fun connectToWifi(context: Context, ssid: String, password: String, onStatusChanged: (String) -> Unit) {
    val wifiManager = context.applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager

    val wifiConfig = WifiConfiguration().apply {
        SSID = "\"$ssid\""
        preSharedKey = "\"$password\""
    }

    val netId = wifiManager.addNetwork(wifiConfig)
    if (netId != -1) {
        wifiManager.disconnect()
        wifiManager.enableNetwork(netId, true)
        wifiManager.reconnect()
        onStatusChanged("Connected to $ssid")
    } else {
        onStatusChanged("Failed to connect to $ssid")
    }
}