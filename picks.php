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

    echo "<body id=\"picks".$_GET['week']."\">\n";
    echo "<div id=\"wrapper\">\n";

    include_once("includes/title_menu.inc");

    echo "<div id=\"body-content\">";
    echo "<h2>Select Winners</h2>";

    if (isset($_GET['player'])) {
        $viewingplayer=$_GET['player'];
    } else {
        $viewingplayer=$_SESSION['logid'];
    }

    if (isset($_POST['winners'])) {
        foreach ($_POST['winners'] as $selects) {
            /*Update picks table with player selections from displaypicksform*/
        	/*echo "id= ".$selects['gameid']."; winner=".$selects['winner']."; tiebreakerpts=".$_POST['tiebreakerpts']."; tbgame=".$_POST['tbgame'];*/
            if (isset($selects['winner'])) {
                $query="UPDATE picks SET pick='".$selects['winner']."' WHERE game_id='".$selects['gameid']."' AND player_id='$viewingplayer'"; /*update pick outcome for each game*/
                $result=pg_exec($cxn,$query) or die ("Could not execute game outcome update query");
        	}
            
            if ($_POST['tbgame']==$selects['gameid']) {
                $query="UPDATE picks SET tiebreakerpts=".$_POST['tiebreakerpts']." WHERE game_id='".$selects['gameid']."' AND player_id='$viewingplayer'"; /*update tiebreaker points for tiebreaker game*/
                $result=pg_exec($cxn,$query) or die ("Could not execute tiebreaker game points update query");
            } else {
                $query="UPDATE picks SET tiebreakerpts=0 WHERE game_id='".$selects['gameid']."' AND player_id='$viewingplayer'"; /*update tiebreaker points to 0 for non-tiebreaker game*/
                $result=pg_exec($cxn,$query) or die ("Could not execute non-tiebreaker game update query");
            }
        }
    }
    
    $currentweekid=$_GET['week'];
    if (isset($_POST['display_button'])) {
        /*If clicked 'Save and View' redirect to outcomes.php page*/
    	if ($_POST['display_button']=="Save and View") {header("Location: outcomes.php?week=$currentweekid&player=$viewingplayer");} 
    }

    /*Retrieve current week parameters to display proper info and games on page*/
    // $currentweekid=$_GET['week'];
    $query = "SELECT * FROM weeks WHERE num=$currentweekid";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute weeks table query.");
    $row=pg_fetch_assoc($result);
    extract($row);

    // date_default_timezone_set('America/New_York');
    $deadlinedatetime=date("Y-m-d H:i:s", strtotime($deadline_date." 12:00:00"));
    		       
    if ($currentdatetime>$deadlinedatetime AND $_SESSION['logname']<>'admin') {
        /*If current date is beyond picks deadline date, don't allow Edit mode for players*/
        header("Location: outcomes.php?week=$currentweekid&player=$viewingplayer");
    } 

    echo "<h3> Week&nbsp".$currentweekid." (".date('F jS Y',strtotime($from_date))." - ".date('F jS Y',strtotime($to_date)).")</h3>";

    /*display first and last name of player whose picks are being shown*/
    $query = "SELECT firstname, lastname FROM player WHERE id=$viewingplayer"; 
    $result = pg_exec($cxn, $query) or die ("Couldn't execute firstname, lastname lookup from player table.");
    $row=pg_fetch_assoc($result);
    extract($row);
    echo "<h3>Player: $firstname $lastname</h3>";

    echo "Select winners for this week and predict the total combined game points of the tiebreaker game (denoted by an '*' in the TB column)<br/>";
    echo "***Your picks are due by 12:00PM EST on ".date('l, F jS Y',strtotime($deadline_date))."***<br/><br/>";

    echo "<form action=\"picks.php?week=".$currentweekid."&player=".$viewingplayer."\" method = 'POST'>";
        include_once("includes/displaypicksform.inc"); /*display Pick Games form*/
    echo "</form>";
    echo "</div>"; /*end of body-content div*/
    include_once("includes/footer.inc");
    echo "</div>"; /*end of wrapper div*/
    echo "</body>";
    echo "</html>";
?>