<?php ob_start();session_start();?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<?php
if($_SESSION['auth']!= "yes")
{
	header ("Location: Login.php");
	exit();
}

include_once("includes/head.inc");
include_once("includes/misc.inc");
include_once("includes/outcomesfunctions.inc");

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

if (isset($_GET['player']))
    {$viewingplayer=$_GET['player'];}
    else
    {$viewingplayer=$_SESSION['logid'];}

/*Set deadline date for current weeek*/
date_default_timezone_set('America/New_York');
$deadlinedatetime=date("Y-m-d H:i:s", strtotime($deadline_date." 12:00:00"));

echo "<form action=\"picks.php?week=".$currentweekid."&player=".$viewingplayer."\" method = 'POST'>";
	displayoutcomes($currentweekid, $viewingplayer); /*display game results table*/
	if (($currentdatetime<=$deadlinedatetime AND $_SESSION['logid']==$viewingplayer) OR $_SESSION['logname']=='admin')
	{
		echo "<p class=\"submit\"><input type=\"submit\" class=\"savebuttons\" name=\"display_button\" value=\"Edit\"/></p>";
	}
echo "</form><br/><br/>";

echo "</div>"; /*end of body-content div*/
include_once("includes/footer.inc");
echo "</div>";  /*end of wrapper div*/
echo "</body>";
echo "</html>";
?>