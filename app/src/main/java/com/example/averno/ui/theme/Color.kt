package com.example.averno.ui.theme

import androidx.compose.ui.graphics.Color


val Purple80 = Color(0xFFD0BCFF)
val PurpleGrey80 = Color(0xFFCCC2DC)
val Pink80 = Color(0xFFEFB8C8)

val Purple40 = Color(0xFF6650a4)
val PurpleGrey40 = Color(0xFF625b71)
val Pink40 = Color(0xFF7D5260)


val BackgroundGreen = Color(0xFF247037)
val BottomNavigationTabGreen = Color(0xFF247037)


//Return greener colours at lower danger levels and redder colours at higher danger levels
//Danger level goes from 0 to 100
fun statusColor(dangerLevel: Float): Color{
    //change value (third argument) to change colour saturation
    return Color.hsv(100-dangerLevel,0.9f, 0.65f)
}

fun backgroundColor(dangerLevel: Float): Color {
    return Color.hsv(100-dangerLevel,1f, 0.50f)
}