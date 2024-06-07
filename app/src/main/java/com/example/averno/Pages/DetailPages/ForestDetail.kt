package com.example.averno.Pages.DetailPages

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavHostController
import com.example.averno.ForestData
import com.example.averno.GetFirebaseData
import com.example.averno.SensorData
import com.example.averno.ui.theme.DetailBody
import com.example.averno.ui.theme.DetailHeadline
import com.example.averno.ui.theme.DetailTextColor
import com.example.averno.ui.theme.DetailTitle
import com.example.averno.ui.theme.statusColor

@Composable
fun ForestDetailPage(navigationController: NavHostController, key:String?) {

    val viewModel: GetFirebaseData = viewModel()
    val forestList by viewModel.forestSensorMap.collectAsState()
    val forestDataList by viewModel.forestData.collectAsState()
    val sensorList: List<SensorData>? = forestList[key]
    val forestData: ForestData? = forestDataList[key]

    if (forestData != null) {
        Column (modifier = Modifier
            .fillMaxSize()
            .background(statusColor(forestData.maxFWI))
            .verticalScroll(rememberScrollState())){

            //Forest name
            Row {
                if (key != null) {
                    Text(text = key,
                        fontFamily = FontFamily.Serif,
                        modifier = Modifier.padding(10.dp),
                        color = Color.White,
                        fontSize = DetailHeadline)
                }
            }

            HorizontalDivider(thickness = 2.dp, color = Color.DarkGray)

                Text(text = "Current Forest Average:",
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = Color.White,
                    fontSize = DetailTitle)

                Text(
                    text = "FWI: " + forestData.maxFWI,
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailTitle
                )

                Info(title = "Temperature", value = forestData.avgTempC, unit = "ºC")
                Info(title = "Humidity", value = forestData.avgHumi, unit = "%")
                Info(title = "Precipitation", value = forestData.avgRain, unit = "ºC")
                Info(title = "Wind Speed", value = forestData.avgWindSpeed, unit = "ºC")
                Info(title = "Air quality", value = forestData.avgGas, unit = "ºC")


            HorizontalDivider(thickness = 3.dp, color = Color.DarkGray)

            //Sensor Data
            if (sensorList != null) {

                for(sensor in sensorList) {

                    Card(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(6.dp),
                        border = BorderStroke(2.dp, Color.DarkGray),
                        colors = CardDefaults.cardColors(
                            containerColor = Color.Gray
                        ),


                        )
                    {
                        //Identifier
                        Text(
                            text = sensor.sensorId,
                            fontFamily = FontFamily.Serif,
                            modifier = Modifier.padding(10.dp),
                            color = Color.White,
                            fontSize = DetailTitle
                        )

                        Text(
                            text = "FWI: " + sensor.fwi,
                            fontFamily = FontFamily.Serif,
                            modifier = Modifier.padding(10.dp),
                            color = DetailTextColor,
                            fontSize = DetailTitle
                        )

                        Info(title = "Temperature", value = sensor.tempC, unit = "ºC")
                        Info(title = "Humidity", value = sensor.humi, unit = "%")
                        Info(title = "Precipitation", value = sensor.rain, unit = "ºC")
                        Info(title = "Wind Speed", value = sensor.windSpeed, unit = "ºC")
                        Info(title = "Air quality", value = sensor.gas, unit = "ºC")

                    }
                }
            }
        }
    }
}
@Composable
fun Info(title: String, value: Float, unit: String){
    Column {
        Text(
            text = "$title: $value$unit",
            fontFamily = FontFamily.Serif,
            modifier = Modifier.padding(10.dp),
            color = DetailTextColor,
            fontSize = DetailBody
        )
    }
}