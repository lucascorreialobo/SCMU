package com.example.averno

sealed class Screens(val screen: String) {
    data object Home: Screens("home")
    data object ForestList: Screens("forest_list")
    data object ForestDetail: Screens("forest_detail")
    data object Add: Screens("add")
    data object Configuration: Screens("configuration")
}