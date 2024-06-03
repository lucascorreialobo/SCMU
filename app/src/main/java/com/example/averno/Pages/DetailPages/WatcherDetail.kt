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
import com.example.averno.SensorData
import com.example.averno.ui.theme.DetailBody
import com.example.averno.ui.theme.DetailHeadline
import com.example.averno.ui.theme.DetailTextColor
import com.example.averno.ui.theme.DetailTitle
import com.example.averno.ui.theme.statusColor

@Composable
fun WatcherDetailPage(navigationController: NavHostController, forestKey:String?, sensorKey: Int?) {

    val viewModel: ForestData = viewModel()
    val forestList by viewModel.forestData.collectAsState()
    val sensorList: List<SensorData>? = forestList[forestKey]
    val sensor: SensorData? = sensorKey?.let { sensorList?.get(it) }

    var totalTemp: Float = 0f
    var totalHum: Float = 0f
    val numberOfSensors = sensorList?.size

    Column (modifier = Modifier
        .fillMaxSize()
        .background(statusColor(0f)) //TODO change to dangerLevel
        .verticalScroll(rememberScrollState())){

        //Sensor name
        Row {
            Text(text = sensorKey.toString(),
                fontFamily = FontFamily.Serif,
                modifier = Modifier.padding(10.dp),
                color = Color.White,
                fontSize = DetailHeadline)

        }

        //Sensor Data
        if (sensor!= null) {
                totalTemp += sensor.temp
                totalHum += sensor.humi

                //Identifier
                Text(text = "Sensor: " + sensor.sensorId,
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = Color.White,
                    fontSize = DetailTitle)

                //Temperature
                Card(
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(6.dp),
                    border = BorderStroke(2.dp, Color.DarkGray)

                )
                {
                    Column {
                        Text(
                            text = "Temperature",
                            fontFamily = FontFamily.Serif,
                            modifier = Modifier.padding(10.dp),
                            color = DetailTextColor,
                            fontSize = DetailTitle
                        )

                        Text(
                            text = "Current: " + sensor.temp,
                            fontFamily = FontFamily.Serif,
                            modifier = Modifier.padding(10.dp),
                            color = DetailTextColor,
                            fontSize = DetailBody
                        )
                    }
                }

                //Humidity
                Card(modifier = Modifier
                    .fillMaxWidth()
                    .padding(6.dp),
                    border = BorderStroke(2.dp, Color.DarkGray)

                )
                {
                    Column {
                        Text(
                            text = "Humidity",
                            fontFamily = FontFamily.Serif,
                            modifier = Modifier.padding(10.dp),
                            color = DetailTextColor,
                            fontSize = DetailTitle
                        )

                        Text(
                            text = "Current: " + sensor.humi,
                            fontFamily = FontFamily.Serif,
                            modifier = Modifier.padding(10.dp),
                            color = DetailTextColor,
                            fontSize = DetailBody
                        )
                    }
                }

        }

        if (numberOfSensors is Int){
            Text(text = "Average temperature: " + totalTemp/numberOfSensors,
                fontFamily = FontFamily.Serif,
                modifier = Modifier.padding(10.dp),
                color = Color.White,
                fontSize = DetailTitle)

            Text(text = "Average humidity: " + totalHum/numberOfSensors,
                fontFamily = FontFamily.Serif,
                modifier = Modifier.padding(10.dp),
                color = Color.White,
                fontSize = DetailTitle)
        }

        /*
        Card(modifier = Modifier
            .fillMaxWidth()
            .padding(6.dp),
            border = BorderStroke(2.dp, Color.DarkGray)

        )
        {
            Column {
                Text(
                    text = "Gas",
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailTitle
                )

                Text(
                    text = "Max: " + f.details.maxGas.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )

                Text(
                    text = "Min: " + f.details.minGas.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )

                Text(
                    text = "Average: " + f.details.avgGas.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )
            }
        }*/

    }
}
