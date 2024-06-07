package com.example.averno.Pages.MainPages

import android.Manifest
import android.annotation.SuppressLint
import android.location.Location
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableDoubleStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.google.accompanist.permissions.ExperimentalPermissionsApi
import com.google.accompanist.permissions.rememberMultiplePermissionsState
import com.google.android.gms.location.FusedLocationProviderClient
import com.google.android.gms.location.LocationServices
import sendSensorData

@OptIn(ExperimentalPermissionsApi::class)
@Composable
fun AddWatcher() {
    val context = LocalContext.current
    val locationProviderClient = remember { LocationServices.getFusedLocationProviderClient(context) }
    val locationState = remember { mutableStateOf<Boolean>(false) }
    var latitude by remember { mutableDoubleStateOf(0.0) }
    var longitude by remember { mutableDoubleStateOf(0.0) }

    val permissions = rememberMultiplePermissionsState(
        permissions = listOf(
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION
        )
    )

    LaunchedEffect(Unit) {
        permissions.launchMultiplePermissionRequest()
    }

    if (permissions.allPermissionsGranted) {
        GetLocation(locationProviderClient) { location ->
            locationState.value = true
            latitude = location.latitude
            longitude = location.longitude
        }
    }

    Box(modifier = Modifier.fillMaxSize()) {
        Column(
            modifier = Modifier
                .fillMaxSize()
                .align(Alignment.Center),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Text(text = "Add watcher", fontSize = 30.sp, color = Color.White)

            if (locationState.value) {
                Spacer(modifier = Modifier.padding(10.dp))

                Text("Latitude: ${latitude}\nLongitude: ${longitude}")

                Spacer(modifier = Modifier.padding(10.dp))

                Button(onClick = {
                    sendSensorData(latitude.toString(), longitude.toString())
                }) {
                    Text(text = "Send")
                }
            } else {
                Text("An error has occurred. Please check if you have location turned on and app has permission to access it.")
            }

            if (!permissions.allPermissionsGranted) {
                Button(onClick = { permissions.launchMultiplePermissionRequest() }) {
                    Text("Request Permissions")
                }
            }
        }
    }
}

@SuppressLint("MissingPermission")
@Composable
fun GetLocation(
    locationProviderClient: FusedLocationProviderClient,
    onLocationReceived: (Location) -> Unit
) {
    LaunchedEffect(Unit) {
        locationProviderClient.lastLocation
            .addOnSuccessListener { location: Location? ->
                location?.let { onLocationReceived(it) }
            }
    }
}
