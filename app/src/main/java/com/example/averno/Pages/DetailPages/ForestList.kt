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
import com.example.averno.ui.theme.backgroundColor
import com.example.averno.ui.theme.statusColor

@Composable
fun ForestListPage(navigationController: NavHostController){

    val viewModel: ForestData = viewModel()
    val forestList by viewModel.sensorData.collectAsState()

    val maxDanger = 0 //TODO change to dangerLevel

    Column (modifier = Modifier
        .fillMaxSize()
        .background(backgroundColor(maxDanger.toFloat())) //TODO change to dangerLevel
        .verticalScroll(rememberScrollState())){
        Text(text = "Averno",
            fontFamily = FontFamily.Serif,
            modifier = Modifier.padding(10.dp),
            color = Color.White,
            fontSize = 35.sp)

        for (f in forestList) {
            Button(
                onClick = { navigationController.navigate("forest_detail_page/" + f.key)},
                Modifier
                    .fillMaxSize()
                    .padding(4.dp),
                shape = RoundedCornerShape(20),
                colors = ButtonDefaults.buttonColors(containerColor = statusColor(0f)) //TODO change to dangerLevel
            ) {
                Row (Modifier.fillMaxWidth()){
                    Image(
                        painter = painterResource(R.drawable.vale_do_silencio),
                        contentDescription = "Contact profile picture",
                        Modifier
                            .size(50.dp)
                            .align(Alignment.CenterVertically)
                    )
                    Spacer(modifier = Modifier.size(15.dp))
                    Text(text = f.key,
                        fontSize = 25.sp,
                        fontFamily = FontFamily.Serif,
                        modifier = Modifier
                            .align(Alignment.CenterVertically))

                }
            }
        }
    }
}
