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
	include_once("includes/displayoutcomes.inc");
	include_once("includes/sharedqueries.inc");

	echo "<body id=\"picks".$_GET['week']."\">\n";
	echo "<div id=\"wrapper\">\n";

	include_once("includes/title_menu.inc");

	echo "<div id=\"body-content\">";
	echo "<h2>Game Results</h2>";

	/*Retrieve current week parameters to display proper info and games on page*/
	$currentweekid=$_GET['week'];
	$query = "SELECT * FROM weeks WHERE num=$currentweekid";
	$result = pg_exec($cxn, $query) or die ("Couldn't execute weeks table query.");
	$row=pg_fetch_assoc($result);
	extract($row);
	echo "<h3> Week&nbsp".$currentweekid." (".date('F jS Y',strtotime($from_date))." - ".date('F jS Y',strtotime($to_date)).")</h3>";

	if (isset($_GET['player'])) {
		$viewingplayer=$_GET['player'];
	} else {
		$viewingplayer=$_SESSION['logid'];
	}

	/*Set deadline date for current week*/
	date_default_timezone_set('America/New_York');

	/*For testing can add or subtract days to deadeline_date*/
	// $deadlinedatetime=date("Y-m-d H:i:s", strtotime($deadline_date." 12:00:00"."+ 2 days"));

	$deadlinedatetime=date("Y-m-d H:i:s", strtotime($deadline_date." 12:00:00"));
	
	// echo "<h1>".$deadlinedatetime."</h1>";
	
	/* If want to only show field picks when all players have made picks*/
	// $showfieldrecords = allPicksMade($currentweekid);

	/* If want to only show field picks when current date beyond deadline + 1 day*/
	// $showfieldrecords = $currentdatetime > date("Y-m-d H:i:s", strtotime($deadlinedatetime."+1 days"));

	/* Only show field picks when logged in player has made all picks for current week */
	/* AND is later than pick submission deadline for current week */
	$showfieldrecords = allPicksMadeByPlayer($currentweekid, $_SESSION['logid']) && $currentdatetime>$deadlinedatetime;

	echo "<form action=\"picks.php?week=".$currentweekid."&player=".$viewingplayer."\" method = 'POST'>";
		displayoutcomes($currentweekid, $viewingplayer, $showfieldrecords); /*display game results table*/
		if (($currentdatetime<=$deadlinedatetime AND $_SESSION['logid']==$viewingplayer) OR $_SESSION['logname']=='admin') {
			echo "<p class=\"submit\"><input type=\"submit\" class=\"savebuttons\" name=\"display_button\" value=\"Edit\"/></p>";
		}
	echo "</form><br/><br/>";

	echo "</div>"; /*end of body-content div*/
	include_once("includes/footer.inc");
	echo "</div>";  /*end of wrapper div*/
	echo "</body>";
	echo "</html>";
?>