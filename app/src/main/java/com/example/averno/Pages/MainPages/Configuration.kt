package com.example.averno.Pages.MainPages

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.sp
import sendSensorData

@Composable
fun Configuration(){
    Box(modifier = Modifier.fillMaxSize()){
        Column(modifier = Modifier
            .fillMaxSize()
            .align(Alignment.Center),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally) {

            Text(text = "Configuration", fontSize = 30.sp, color = Color.Green)

            Button(onClick = {
                sendSensorData(1)
            }) {
                Text(text = "Turn On")
            }

            Button(onClick = {
                sendSensorData(0)
            }) {
                Text(text = "Turn Off")
            }
        }
    }
}
