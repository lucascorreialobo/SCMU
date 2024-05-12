package com.example.averno

sealed class Screens(val sceen: String) {
    data object Home: Screens("home")
    data object Add: Screens("add")
    data object Configuration: Screens("configuration")
}