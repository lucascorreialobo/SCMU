package com.example.averno.Pages.DetailPages

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavHostController
import com.example.averno.Forest
import com.example.averno.ForestData
import com.example.averno.R
import com.example.averno.ui.theme.BottomNavigationTabGreen
import com.example.averno.ui.theme.DetailBody
import com.example.averno.ui.theme.DetailHeadline
import com.example.averno.ui.theme.DetailTextColor
import com.example.averno.ui.theme.DetailTitle
import com.example.averno.ui.theme.backgroundColor
import com.example.averno.ui.theme.getDangerColor
import com.example.averno.ui.theme.statusColor

@Composable
fun ForestDetailPage(navigationController: NavHostController, id:Int?) {
    var f: Forest
    if(id != null) {
        f = ForestData().forestList[id]
    } else {
        f = ForestData().forestList[0]
    }

    Column (modifier = Modifier
        .fillMaxSize()
        .background(statusColor(f.dangerLevel))
        .verticalScroll(rememberScrollState())){
        Row {
            Text(text = f.name,
                fontFamily = FontFamily.Serif,
                modifier = Modifier.padding(10.dp),
                color = Color.White,
                fontSize = DetailHeadline)
        }

        //Temperature
        Card(modifier = Modifier
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
                    text = "Max: " + f.details.maxTemperature.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )

                Text(
                    text = "Min: " + f.details.minTemperature.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )

                Text(
                    text = "Average: " + f.details.avgTemperature.toString(),
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
                    text = "Max: " + f.details.maxHumidity.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )

                Text(
                    text = "Min: " + f.details.minHumidity.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )

                Text(
                    text = "Average: " + f.details.avgHumidity.toString(),
                    fontFamily = FontFamily.Serif,
                    modifier = Modifier.padding(10.dp),
                    color = DetailTextColor,
                    fontSize = DetailBody
                )
            }
        }

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
        }

    }
}
