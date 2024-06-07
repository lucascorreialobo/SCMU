package com.example.averno

import com.google.firebase.messaging.FirebaseMessagingService
import com.google.firebase.messaging.RemoteMessage
import android.util.Log
import android.widget.Toast

class MyFirebaseMessagingService : FirebaseMessagingService() {

    override fun onMessageReceived(remoteMessage: RemoteMessage) {
        super.onMessageReceived(remoteMessage)

        // Handle FCM messages here.
        val title = remoteMessage.notification?.title
        val body = remoteMessage.notification?.body
        Log.d("FCM", "Message Notification Title: $title")
        Log.d("FCM", "Message Notification Body: $body")

        // Show the message as a toast
        Toast.makeText(applicationContext, "$title: $body", Toast.LENGTH_LONG).show()
    }

    override fun onNewToken(token: String) {
        super.onNewToken(token)
        Log.d("FCM", "Refreshed token: $token")

        // You can send the token to your server if needed
    }
}