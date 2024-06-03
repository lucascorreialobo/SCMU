package com.example.averno

import android.content.ContentValues.TAG
import android.util.Log
import androidx.lifecycle.ViewModel
import com.google.firebase.Firebase
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.database
import com.google.firebase.database.getValue
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow

data class SensorData(
    var sensorId: String = "0",
    val humi: Int = 0,
    val temp: Int = 0,
    var avgHumi: Int = 0,
    var avgTemp: Int = 0,
)


class ForestData: ViewModel(){

    private val database = Firebase.database
    val myRef = database.getReference("forests")

    private val _forestData = MutableStateFlow<Map<String, List<SensorData>>>(emptyMap())
    val forestData: StateFlow<Map<String, List<SensorData>>> = _forestData

    init {
        fetchMessage()
    }


    private fun fetchMessage() {
        myRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val dataMap = mutableMapOf<String, List<SensorData>>()
                for (sensorSnapshot in snapshot.children) {
                    val forestName = sensorSnapshot.key ?: continue
                    val sensorList = mutableListOf<SensorData>()
                    var sensorData: SensorData
                    var latestSnapshot = SensorData()
                    var sumTemp = 0
                    var sumHumi = 0
                    var totalTimestamps = 0
                    for (timeSnapshot in sensorSnapshot.children) {
                        //latestSnapshot = timeSnapshot.child("1").getValue(SensorData::class.java) ?: continue
                        for (childSnapshot in timeSnapshot.children){
                            sensorData = childSnapshot.getValue(SensorData::class.java) ?: continue
                            sensorData.sensorId = timeSnapshot.key ?: continue
                            sumTemp += sensorData.temp
                            sumHumi += sensorData.humi
                            totalTimestamps += 1
                            latestSnapshot = sensorData
                        }
                        latestSnapshot.avgHumi = sumHumi/totalTimestamps
                        latestSnapshot.avgTemp = sumTemp/totalTimestamps
                        sensorList.add(latestSnapshot)
                    }

                    dataMap[forestName] = sensorList
                }
                _forestData.value = dataMap
            }

            override fun onCancelled(error: DatabaseError) {
                Log.w(TAG, "Failed to read value.", error.toException())
            }
        })

        Log.d(TAG, "Failed to read value.")
    }
}