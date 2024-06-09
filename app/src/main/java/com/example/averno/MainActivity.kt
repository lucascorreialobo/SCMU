package com.example.averno

import android.content.ContentValues
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Settings
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.example.averno.Pages.MainPages.AddWatcher
import com.example.averno.Pages.MainPages.Configuration
import com.example.averno.Pages.MainPages.Home
import com.example.averno.ui.theme.AvernoTheme
import com.example.averno.ui.theme.BottomNavigationTabGreen
import com.google.firebase.messaging.FirebaseMessaging


class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            AvernoTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = Color.White
                ) {
                    MyBottomAppBar()
                }
            }
        }

        // Subscribe to a topic
        FirebaseMessaging.getInstance().subscribeToTopic("sensor_alerts")
            .addOnCompleteListener { task ->
                var msg = "Subscribed to sensor_alerts topic"
                if (!task.isSuccessful) {
                    msg = "Subscription to sensor_alerts topic failed"
                }
                Log.d("FCM", msg)
            }
    }


}

@Composable
fun MyBottomAppBar(){

    Log.d(ContentValues.TAG, "Failed to read value.")

    val navigationController = rememberNavController()
    val selected = remember {
        mutableStateOf(Icons.Default.Home)
    }

    Scaffold(
        bottomBar = {
            BottomAppBar (
                containerColor = BottomNavigationTabGreen,
                modifier = Modifier
                    .fillMaxWidth()
                    .size(300.dp, 125.dp)

            ){
                IconButton(
                    onClick = {
                        selected.value = Icons.Default.Home
                        navigationController.navigate(Screens.Home.screen){
                            popUpTo(0)
                        }
                    },
                    modifier = Modifier.weight(1f)
                ) {
                    Icon(Icons.Default.Home, contentDescription = null, modifier = Modifier.size(30.dp),
                        tint = if (selected.value == Icons.Default.Home) Color.White else Color.DarkGray)
                }

                Box(modifier = Modifier
                    .weight(1f)
                    .padding(6.dp),
                    contentAlignment = Alignment.Center

                ) {
                    FloatingActionButton(onClick = {
                        selected.value = Icons.Default.Add
                        navigationController.navigate(Screens.Add.screen){
                            popUpTo(0)
                        }
                    }) {
                        Icon(Icons.Default.Add, contentDescription = null, modifier = Modifier.size(30.dp),
                            tint = if (selected.value == Icons.Default.Add) Color.White else Color.Black)
                    }
                }

                IconButton(
                    onClick = {
                        selected.value = Icons.Default.Settings
                        navigationController.navigate(Screens.Configuration.screen){
                            popUpTo(0)
                        }
                    },
                    modifier = Modifier.weight(1f)
                ) {
                    Icon(Icons.Default.Settings, contentDescription = null, modifier = Modifier.size(30.dp),
                        tint = if (selected.value == Icons.Default.Settings) Color.White else Color.DarkGray)
                }

            }
        }
    ) {paddingValues ->
        NavHost(navController = navigationController,
            startDestination = Screens.Home.screen,
            modifier = Modifier.padding(paddingValues)){
            composable(Screens.Home.screen){ Home() }
            composable(Screens.Configuration.screen){ Configuration() }
            composable(Screens.Add.screen){ AddWatcher() }
    }

    }
}