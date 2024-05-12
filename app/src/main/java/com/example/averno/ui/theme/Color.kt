package com.example.averno.ui.theme

import androidx.compose.ui.graphics.Color

val Purple80 = Color(0xFFD0BCFF)
val PurpleGrey80 = Color(0xFFCCC2DC)
val Pink80 = Color(0xFFEFB8C8)

val Purple40 = Color(0xFF6650a4)
val PurpleGrey40 = Color(0xFF625b71)
val Pink40 = Color(0xFF7D5260)

val BackgroundGreen = Color(100,200,100)

//Return greener colours at lower danger levels and redder colours at higher danger levels
//Danger level goes from 0 to 510
fun StatusColor(dangerLevel: Int): Color{
    if(dangerLevel <= 255){
        return Color(dangerLevel,255,0)
    }
    else{
        return Color(255,255 - (dangerLevel - 255),0)
    }
}