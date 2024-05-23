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

data class SensorData(
    var sensorId: String = "0",
    val humi: Int = 0,
    val temp: Int = 0
)


class ForestData: ViewModel(){

    private val database = Firebase.database
    val myRef = database.getReference("forests")

    private val _sensorData = MutableStateFlow<Map<String, List<SensorData>>>(emptyMap())
    val sensorData: StateFlow<Map<String, List<SensorData>>> = _sensorData

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
                    var sensorData = SensorData()
                    for (childSnapshot in sensorSnapshot.children) {
                        sensorSnapshot.key ?: continue
                        sensorData = childSnapshot.getValue(SensorData::class.java) ?: continue
                        sensorData.sensorId = childSnapshot.key ?: continue
                        sensorList.add(sensorData)
                    }
                    dataMap[forestName] = sensorList
                }
                _sensorData.value = dataMap
            }

            override fun onCancelled(error: DatabaseError) {
                Log.w(TAG, "Failed to read value.", error.toException())
            }
        })

        Log.d(TAG, "Failed to read value.")
    }
}