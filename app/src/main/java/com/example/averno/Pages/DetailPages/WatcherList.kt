package com.example.averno.Pages.DetailPages

import androidx.compose.foundation.Image
import androidx.compose.foundation.background
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
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavHostController
import com.example.averno.ForestData
import com.example.averno.R
import com.example.averno.SensorData
import com.example.averno.ui.theme.backgroundColor
import com.example.averno.ui.theme.statusColor

@Composable
fun WatcherListPage(navigationController: NavHostController, forestKey:String?){

    val viewModel: ForestData = viewModel()
    val forestList by viewModel.forestData.collectAsState()
    val sensorList: List<SensorData>? = forestList[forestKey]

    val maxDanger = 0 //TODO change to dangerLevel

    Column (modifier = Modifier
        .fillMaxSize()
        .background(backgroundColor(maxDanger.toFloat())) //TODO change to dangerLevel
        .verticalScroll(rememberScrollState())){
        if (forestKey != null) {
            Text(text = forestKey,
                fontFamily = FontFamily.Serif,
                modifier = Modifier.padding(10.dp),
                color = Color.White,
                fontSize = 35.sp)
        }

        if (sensorList != null) {
            for (s in sensorList) {
                Button(
                    onClick = { navigationController.navigate("forest_detail_page/" + s.sensorId)},
                    Modifier
                        .fillMaxSize()
                        .padding(4.dp),
                    shape = RoundedCornerShape(20),
                    colors = ButtonDefaults.buttonColors(containerColor = statusColor(0f)) //TODO change to dangerLevel
                ) {
                    Text(text = s.sensorId,
                        fontSize = 25.sp,
                        fontFamily = FontFamily.Serif,
                        modifier = Modifier
                            .align(Alignment.CenterVertically))
                }
            }
        }
    }
}
