package com.example.averno

data class Forest(val id: Int, val name: String, val dangerLevel: Float, val details: Detail)

data class Detail(
    val maxTemperature: Float,
    val minTemperature: Float,
    val avgTemperature: Float,
    val maxHumidity: Float,
    val minHumidity: Float,
    val avgHumidity: Float,
    val maxGas: Float,
    val minGas: Float,
    val avgGas: Float,
)

class ForestData(){
    var detail = Detail(1f,1f,1f,1f,1f,1f,1f,1f,1f)

    var forestList = arrayOf<Forest>(
        Forest(0,"Floresta Laurissilva", 8.2f, detail),
        Forest(1,"Lagoa das Furnas", 17.2f, detail),
        Forest(2,"Grená", 24.6f, detail),
        Forest(3,"Parque Florestal de Monsanto", 38.3f, detail),
        Forest(4,"Covão D'Ametade", 0.1f, detail),
        Forest(5,"Mata-Jardim José do Canto", 4f, detail),
        Forest(6,"Posto Florestal Fanal", 0f, detail),
        Forest(7,"Parque Natural de Montesinho", 0f, detail),
        Forest(8,"Mata Nacional dos Sete Montes", 0f, detail),
    )

}