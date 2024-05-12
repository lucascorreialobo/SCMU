package com.example.averno

import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.averno.ui.theme.BackgroundGreen
import com.example.averno.ui.theme.StatusColor

@Composable
fun Home(){
    val nums = arrayOf(1,2,3,4,5,6,7,8,9,10,11,12,13)
    Column (modifier = Modifier
        .fillMaxSize()
        .background(BackgroundGreen)
        .verticalScroll(rememberScrollState()),
        horizontalAlignment = Alignment.CenterHorizontally){
        Text("Averno", fontSize = 50.sp, fontWeight = FontWeight.Bold,)

        for (x in nums) {
            Button(
                onClick = { /*TODO*/ },
                Modifier
                    .fillMaxSize()
                    .padding(5.dp),
                colors = ButtonDefaults.buttonColors(containerColor = StatusColor(125))
            ) {
                Row{
                    Image(
                        painter = painterResource(R.drawable.vale_do_silencio),
                        contentDescription = "Contact profile picture",
                        Modifier
                            .size(50.dp)
                    )
                    Spacer(modifier = Modifier.size(15.dp))
                    Text("Florest " + x, fontSize = 30.sp)
                }
            }
        }
    }
}
