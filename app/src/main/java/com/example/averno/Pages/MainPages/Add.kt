package com.example.averno.Pages.MainPages

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import sendSensorData

@Composable
fun Add(){
    Box(modifier = Modifier.fillMaxSize()){
        Column(modifier = Modifier
            .fillMaxSize()
            .align(Alignment.Center),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally) {

            var latitude by remember { mutableStateOf("36º57'N") }
            var longitude by remember { mutableStateOf("6º12'O") }

            Text(text = "Add watcher", fontSize = 30.sp, color = Color.Green)

            Spacer(modifier = Modifier.padding(10.dp))

            TextField(
                value = latitude,
                onValueChange = { latitude = it },
                label = { Text("Latitude:") }
            )

            TextField(
                value = longitude,
                onValueChange = { longitude = it },
                label = { Text("Longitude:") }
            )

            Spacer(modifier = Modifier.padding(10.dp))

            Button(onClick = {
                sendSensorData(latitude, longitude)
            }) {
                Text(text = "Send")
            }
        }
    }
}
