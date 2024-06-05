import org.json.JSONObject
import java.io.BufferedOutputStream
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL
import kotlin.concurrent.thread

val ip = "192.168.4.1"
fun sendSensorData(latitude: String, longitude: String) {
    thread {
        try {
            val url = URL("http://$ip/") // Replace with your Arduino's IP address

            val json = JSONObject()
            json.put("sensor", "coordinates")
            json.put("latitude", latitude)
            json.put("longitude", longitude)

            val jsonString = json.toString()
            val postData = jsonString.toByteArray()

            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "POST"
            connection.setRequestProperty("Content-Type", "application/json")
            connection.setRequestProperty("Content-Length", postData.size.toString())
            connection.doOutput = true

            val outputStream = BufferedOutputStream(connection.outputStream)
            outputStream.write(postData)
            outputStream.flush()
            outputStream.close()

            val responseCode = connection.responseCode
            if (responseCode == HttpURLConnection.HTTP_OK) {
                val inputStream = connection.inputStream
                val reader = BufferedReader(InputStreamReader(inputStream))
                val response = StringBuilder()
                var line: String?

                while (reader.readLine().also { line = it } != null) {
                    response.append(line)
                }
                reader.close()

                // Handle the response data
                println("Response: $response")
            } else {
                println("POST request failed. Response Code: $responseCode")
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}
