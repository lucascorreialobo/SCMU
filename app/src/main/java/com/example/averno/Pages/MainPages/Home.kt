package com.example.averno.Pages.MainPages

import androidx.compose.runtime.Composable
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import com.example.averno.Pages.DetailPages.ForestDetailPage
import com.example.averno.Pages.DetailPages.ForestListPage
import com.example.averno.Pages.DetailPages.WatcherDetailPage
import com.example.averno.Pages.DetailPages.WatcherListPage

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
            arguments = listOf(navArgument("forestId") {defaultValue = ""})
        ) { backStackEntry ->
            ForestDetailPage(navigationController, backStackEntry.arguments?.getString("forestId"))
        }

        composable(
            "watcher_list_page/{forestKey}",
            arguments = listOf(navArgument("forestKey") {defaultValue = ""})
        ) { backStackEntry ->
            WatcherListPage(navigationController, backStackEntry.arguments?.getString("forestKey"))
        }

        composable(
            "watcher_detail_page/{forestKey}/{sensorKey}",
            arguments = listOf(navArgument("forestKey") {defaultValue = ""},
                               navArgument("sensorKey") {defaultValue = 0})
        ) { backStackEntry ->
            WatcherDetailPage(navigationController, backStackEntry.arguments?.getString("forestKey"), backStackEntry.arguments?.getInt("sensorKey"))
        }

    }
}
