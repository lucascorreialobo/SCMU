import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.NetworkInterface
import java.net.SocketException
import java.net.URL
import kotlin.concurrent.thread

val mac = "C8:C9:A3:D8:DF:54";

fun sendSensorData(data: Int) {
    thread {
        try {
            val ip = getIPAddressFromMAC(mac)
            val url = URL("http://$ip/?data=IuriCarrelo")
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

fun getIPAddressFromMAC(macAddress: String): String? {
    try {
        val networkInterfaces = NetworkInterface.getNetworkInterfaces()
        while (networkInterfaces.hasMoreElements()) {
            val networkInterface = networkInterfaces.nextElement()
            val inetAddresses = networkInterface.inetAddresses
            while (inetAddresses.hasMoreElements()) {
                val inetAddress = inetAddresses.nextElement()
                if (!inetAddress.isLoopbackAddress) {
                    val ipAddress = inetAddress.hostAddress
                    val mac = networkInterface.hardwareAddress
                    if (mac != null) {
                        val macString = mac.joinToString(separator = ":") { byte -> "%02X".format(byte) }
                        if (macString.equals(macAddress, ignoreCase = true)) {
                            return ipAddress
                        }
                    }
                }
            }
        }
    } catch (e: SocketException) {
        e.printStackTrace()
    }
    return null
}
