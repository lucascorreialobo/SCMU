package com.example.averno

import android.content.ContentValues.TAG
import android.util.Log
import androidx.lifecycle.ViewModel
import com.google.firebase.Firebase
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.database
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow

data class Coordinates(
    val latitude: String = "0.0",
    val longitude: String = "0.0",
)
data class SensorData(
    var sensorId: String = "0",
    val humidity: Float = 0f,
    val temperatureC: Float = 0f,
    val temperatureF: Float = 0f,
    val gas: Float = 0f,
    val wind_speed: Float = 0f,
    val rain: Float = 0f,
    val smoke_danger: String = "0",
    val local_fwi: Float = 0f,
    var coordinates: Coordinates = Coordinates(),
)

data class ForestData(
    var avgHumi: Float = 0f,
    var avgTempC: Float = 0f,
    var avgTempF: Float = 0f,
    val avgGas: Float = 0f,
    val avgWindSpeed: Float = 0f,
    val avgRain: Float = 0f,
    val maxFWI: Float = 0f,
)


class GetFirebaseData: ViewModel(){

    private val database = Firebase.database
    val myRef = database.getReference("forests")

    private val _forestSensorMap = MutableStateFlow<Map<String, List<SensorData>>>(emptyMap())
    val forestSensorMap: StateFlow<Map<String, List<SensorData>>> = _forestSensorMap

    private val _forestData = MutableStateFlow<Map<String, ForestData>>(emptyMap())
    val forestData: StateFlow<Map<String, ForestData>> = _forestData

    init {
        fetchMessage()
    }


    private fun fetchMessage() {
        myRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val sensorDataMap = mutableMapOf<String, List<SensorData>>()
                val forestDataMap = mutableMapOf<String, ForestData>()
                for (forestSnapshot in snapshot.children) {
                    val forestName = forestSnapshot.key ?: continue
                    val sensorList = mutableListOf<SensorData>()
                    var amountOfSensors = 0
                    val sensorSums = Array<Float>(7) { 0f }
                    for (sensorSnapshot in forestSnapshot.children) {
                        val dataSnapshot = sensorSnapshot.child("data")
                        val coordinatesSnapshot = sensorSnapshot.child("coordinates")

                        val sensorData = dataSnapshot.children.toList().lastOrNull()?.getValue(SensorData::class.java) ?: continue
                        val coordinatesData = coordinatesSnapshot.getValue(Coordinates::class.java) ?: continue
                        sensorData.sensorId = sensorSnapshot.key ?: continue
                        sensorData.coordinates = coordinatesData
                        sensorList.add(sensorData)

                        //Use to calculate Forest info
                        sensorSums[0] += sensorData.humidity
                        sensorSums[1] += sensorData.temperatureC
                        sensorSums[2] += sensorData.temperatureF
                        sensorSums[3] += sensorData.gas
                        sensorSums[4] += sensorData.wind_speed
                        sensorSums[5] += sensorData.rain
                        if (sensorSums[6] < sensorData.local_fwi){
                            sensorSums[6] = sensorData.local_fwi
                        }
                        amountOfSensors++;
                    }

                    val newForest = ForestData(
                        avgHumi = sensorSums[0]/amountOfSensors,
                        avgTempC = sensorSums[1]/amountOfSensors,
                        avgTempF = sensorSums[2]/amountOfSensors,
                        avgGas = sensorSums[3]/amountOfSensors,
                        avgWindSpeed = sensorSums[4]/amountOfSensors,
                        avgRain = sensorSums[5]/amountOfSensors,
                        maxFWI = sensorSums[6]
                    )

                    forestDataMap[forestName] = newForest

                    sensorDataMap[forestName] = sensorList
                }
                _forestData.value = forestDataMap
                _forestSensorMap.value = sensorDataMap
            }

            override fun onCancelled(error: DatabaseError) {
                Log.w(TAG, "Failed to read value.", error.toException())
            }
        })

        Log.d(TAG, "Failed to read value.")
    }
}