package com.example.averno.Pages.MainPages

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.sp

@Composable
fun Configuration(){
    Box(modifier = Modifier.fillMaxSize()){
        Column(modifier = Modifier
            .fillMaxSize()
            .align(Alignment.Center),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally) {

            Text(text = "Configuration", fontSize = 30.sp, color = Color.Green)
        }
    }
}

/*@Composable
fun GoogleMap(
    mergeDescendants: Boolean = false,
    modifier: Modifier = Modifier,
    cameraPositionState: CameraPositionState = rememberCameraPositionState(),
    contentDescription: String? = null,
    googleMapOptionsFactory: () -> GoogleMapOptions = { GoogleMapOptions() },
    properties: MapProperties = DefaultMapProperties,
    locationSource: LocationSource? = null,
    uiSettings: MapUiSettings = DefaultMapUiSettings,
    indoorStateChangeListener: IndoorStateChangeListener = DefaultIndoorStateChangeListener,
    onMapClick: (LatLng) -> Unit? = null,
    onMapLongClick: (LatLng) -> Unit? = null,
    onMapLoaded: () -> Unit? = null,
    onMyLocationButtonClick: () -> Boolean? = null,
    onMyLocationClick: (Location) -> Unit? = null,
    onPOIClick: (PointOfInterest) -> Unit? = null,
    contentPadding: PaddingValues = NoPadding,
    content: @Composable () -> Unit? = null
)*/
