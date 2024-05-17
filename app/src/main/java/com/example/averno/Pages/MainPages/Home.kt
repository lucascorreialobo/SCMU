package com.example.averno.Pages.MainPages

import androidx.compose.runtime.Composable
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import com.example.averno.Forest
import com.example.averno.Pages.DetailPages.ForestDetailPage
import com.example.averno.Pages.DetailPages.ForestListPage

@Composable
fun Home(){
    val navigationController = rememberNavController()

    NavHost(navController = navigationController, startDestination = "forest_list_page"){
        composable(
            "forest_list_page"
        ) {
            ForestListPage(navigationController)
        }

        composable(
            "forest_detail_page/{forestId}",
            arguments = listOf(navArgument("forestId") {defaultValue = 0})
        ) { backStackEntry ->
            ForestDetailPage(navigationController, backStackEntry.arguments?.getInt("forestId"))
        }

    }
}
