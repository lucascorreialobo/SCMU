import java.net.HttpURLConnection
import java.net.URL
import kotlin.concurrent.thread

val ip = "192.168.1.116";

fun sendSensorData(data: Int) {
    thread {
        try {
            val url = URL("http://$ip/?data=$data")
            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "GET"

            val responseCode = connection.responseCode
            /*if (responseCode == HttpURLConnection.HTTP_OK) {
                //val inputStream = connection.inputStream
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
                println("GET request failed. Response Code: $responseCode")
            }*/
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}

