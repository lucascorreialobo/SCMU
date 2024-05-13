package com.example.averno.MainPages

import androidx.compose.foundation.BorderStroke
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
import androidx.compose.material3.ElevatedButton
import androidx.compose.material3.LocalTextStyle
import androidx.compose.material3.OutlinedButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.StrokeJoin
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.googlefonts.GoogleFont
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.averno.Forest
import com.example.averno.R
import com.example.averno.ui.theme.BackgroundGreen
import com.example.averno.ui.theme.BottomNavigationTabGreen
import com.example.averno.ui.theme.backgroundColor
import com.example.averno.ui.theme.statusColor

@Composable
fun Home(){
    val forestList = arrayOf(
        Forest("Floresta Laurissilva", 0f),
        Forest("Lagoa das Furnas", 20f),
        Forest("Grená", 90f),
        Forest("Parque Florestal de Monsanto", 10f),
        Forest("Covão D'Ametade", 24f),
        Forest("Mata-Jardim José do Canto", 76f),
        Forest("Posto Florestal Fanal", 33f),
        Forest("Parque Natural de Montesinho", 54f),
        Forest("Mata Nacional dos Sete Montes", 64f),
        Forest("Quinta Da Ribafria", 11f),
        Forest("Cabril do Ceira", 49f),
        Forest("Curia Park", 55f),
    )

    forestList.sortWith(compareByDescending { it.dangerLevel })

    var maxDanger = forestList.get(0).dangerLevel

    Column (modifier = Modifier
        .fillMaxSize()
        .background(backgroundColor(maxDanger))
        .verticalScroll(rememberScrollState())){
        Text(text = "Averno",
            fontFamily = FontFamily.Serif,
            modifier = Modifier.padding(10.dp),
            color = Color.White,
            fontSize = 30.sp)

        for (f in forestList) {
            Button(
                onClick = { /*TODO*/ },
                Modifier
                    .fillMaxSize()
                    .padding(4.dp),
                shape = RoundedCornerShape(20),
                colors = ButtonDefaults.buttonColors(containerColor = statusColor(f.dangerLevel))
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
                    Text(text = f.name,
                        fontSize = 20.sp,
                        fontFamily = FontFamily.Serif,
                        modifier = Modifier.align(Alignment.CenterVertically))
                }
            }
        }
    }
}
