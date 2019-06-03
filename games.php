<!doctype html>
<html lang="en">
<?php
    // Start session 
    session_start();

    // If user not authorized, redirect to login page
    if($_SESSION['auth']!= "yes") {
        header ("Location: Login.php");
        exit();
    }

    include_once("includes/head.inc");
    include_once("includes/misc.inc");

    echo "<body id=\"games".$_GET['week']."\">\n";
    echo "<div id=\"wrapper\">\n";

    include_once("includes/title_menu.inc");

    echo "<div id=\"body-content\">";
    echo "<h2>Manage Games</h2>";

    if (isset($_GET['player'])) {
        $viewingplayer=$_GET['player'];
    } else {
        $viewingplayer=$_SESSION['logid'];
    }

    /**********************************************************************************/
    /*Update all game setup changes to games and weeks table                          */
    /**********************************************************************************/

    $gamecounter=0;
    if (isset($_POST['selections'])) {
        foreach ($_POST['selections'] as $selects) {
            /*Update games table with admin game setup selections from displaygamesform*/
            /*echo "id= ".$selects['gameid']."; date=".$selects['date']."; locale= ".$selects['locale']."; opponent id=".$selects['opponent_id']."<br/>";*/
            if ($selects['gameid']==$_POST['tbgame']) {
                $query="UPDATE games SET tiebreaker=1 WHERE id='".$selects['gameid']."'"; /*update tiebreaker game*/
                $result=pg_exec($cxn,$query) or die ("Could not execute gamedate update query");
            } else {
                $query="UPDATE games SET tiebreaker=0 WHERE id='".$selects['gameid']."'"; /*update non-tiebreaker game*/
                $result=pg_exec($cxn,$query) or die ("Could not execute gamedate update query");
            }
            $query="UPDATE games SET gamedate='".$selects['date']."' WHERE id='".$selects['gameid']."'"; /*update game date for each game*/
        	$result=pg_exec($cxn,$query) or die ("Could not execute gamedate update query");
            $query="UPDATE games SET locale=".$selects['locale']." WHERE id='".$selects['gameid']."'"; /*update game date for each game*/
        	$result=pg_exec($cxn,$query) or die ("Could not execute gamedate update query");
            $query="UPDATE games SET opponent_id=".$selects['opponent_id']." WHERE id='".$selects['gameid']."'"; /*update game date for each game*/
        	$result=pg_exec($cxn,$query) or die ("Could not execute gamedate update query");
            if ($selects['locale']<>0) {
                $gamecounter=$gamecounter+1;
            }
            /*echo "success</br>";*/
        } // end foreach
        $query="UPDATE weeks SET deadline_date='".$_POST['deadlinedate']."', num_games=$gamecounter WHERE num='".$_POST['currentweekid']."'"; /*update deadline date for associated week*/
        $result=pg_exec($cxn,$query) or die ("Could not execute gamedate update query");
        
        $currentweekid=$_GET['week'];
        if ($_POST['display_button']=="Save and View") {
            header("Location: winners.php?week=$currentweekid");
        } /*If click 'Save & View' redirect back to winners.php page*/
        
        /*echo "tiebreaker game =".$_POST['tbgame']."deadline date =".$_POST['deadlinedate'];*/
    }

    /*Retrieve current week parameters to display proper info and games on page*/

    $currentweekid=$_GET['week'];
    $query = "SELECT * FROM weeks WHERE num=$currentweekid";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute weeks table query.");
    $row=pg_fetch_assoc($result);
    extract($row);
    echo "<h3>Week&nbsp".$currentweekid." (".date('F jS Y',strtotime($from_date))." - ".date('F jS Y',strtotime($to_date)).")</h3>";
    echo "Select games for this week by entering Date, Team & Opponent for each game.<br/><br/>";
          
    echo "<form action=\"games.php?week=".$currentweekid."\" method = 'POST'>";
        include_once("includes/displaygamesform.inc"); /*display Manage Games form*/
    echo "</form>";

    echo "</div>"; /*end of body-content div*/
    include_once("includes/footer.inc");
    echo "</div>";  /*end of wrapper div*/
    echo "</body>";
    echo "</html>";
?>
