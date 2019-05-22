<?php
	/* Load Composer dependencies */
	require_once ("vendor/autoload.php");

    /* Start session */
	ob_start();
	session_start();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
if($_SESSION['auth']!= "yes")
{
	header ("Location: login.php");
	exit();
}

include_once("includes/head.inc");
include_once("includes/misc.inc");

echo "<body id=\"summary\">\n";
echo "<div id=\"wrapper\">\n";

include_once("includes/title_menu.inc");

echo "<div id=\"body-content\">";
echo "<h2>Overall Summary</h2>";

/*Determine proper weeks view to show (1-6, 7-12, or 13-18)*/
if (isset($_GET['view']))
	{$view=$_GET['view'];}
else
	{
		switch ($currentweek) {
			case 1: case 2: case 3: case 4: case 5: case 6:
				$view=0; break;
			case 7: case 8: case 9: case 10: case 11: case 12:
				$view=1; break;
			case 13: case 14: case 15: case 16: case 17: case 18:
				$view=2; break;
		}
	}
switch ($view) {
	case 0:
	$previousview_id=0; $nextview_id=1; break;
	case 1:
	$previousview_id=0; $nextview_id=2; break;
	case 2:
	$previousview_id=1; $nextview_id=2; break;
}
$fromweek = ($view*6+1);
$toweek = ($view+1)*6;

/*Determine which sort order to display players in and display options at top*/

if (isset($_GET['orderby']))
	{$orderby=$_GET['orderby'];}
else /*default order by current week id (=1)*/
	{$orderby=1;}

switch ($orderby) {
	case 0:
		echo "<h5>Re-order by:&nbsp&nbsp&nbsp<a class=\"order\" href=\"index.php?view=$view&orderby=1\">Current Week</a></h5>";
		break;
	case 1:
		echo "<h5>Re-order by:&nbsp&nbsp&nbsp<a class=\"order\" href=\"index.php?view=$view&orderby=0\">Picks Record</a></h5>";
		break;
}

/*Display table header*/
echo "<table class=\"summary\">";
	echo "<thead>";
		echo "<tr>";
		echo "<th></th>";
		echo "<th class=\"name\">Name</th>";
		echo "<th class=\"team\">Team</th>";
		echo "<th class=\"precord\">Picks Record</th>";
		echo "<th class=\"wrecord\">Weeks Record</th>";
		echo "<th><a href=\"index.php?view=$previousview_id&orderby=$orderby\"><<</a></th>";
		
		$query = "SELECT num FROM weeks WHERE num >= $fromweek AND num <= $toweek ORDER BY num ASC";
		$result = pg_exec($cxn, $query) or die ("Couldn't execute week range query.");
		while ($row=pg_fetch_assoc($result))
		{
			extract($row);
			echo "<th class=\"week\">Week $num</th>";
		}
		
		echo "<th><a href=\"index.php?view=$nextview_id&orderby=$orderby\">>></a></th>";
		echo "</tr>";
	echo "</thead>";
	echo "<tbody>";

	/*Build weeks completed array (to only show winners after all game outcomes are final*/
	$weekscompleted = array();
	$latestweekwithatleast1completegame=1;
	$weeksquery = "SELECT num FROM weeks ORDER BY num ASC";
	$weeksresult = pg_exec($cxn, $weeksquery) or die ("Couldn't execute weeks_won weekly records query.");
	while ($weeksrow=pg_fetch_assoc($weeksresult))
	{
		$complete=1;
		$atleastonegamecomplete=0;
		extract($weeksrow);
		$gamesquery = "SELECT locale, outcome FROM games WHERE week_num = '$num'";
		$gamesresult = pg_exec($cxn, $gamesquery) or die ("Couldn't execute weeks_won weekly records query.");
		while ($gamesrow=pg_fetch_assoc($gamesresult))
		{
			extract($gamesrow);
			if ($outcome==0 AND $locale>0)
				{$complete=0;}
			if ($outcome<>0 AND $locale>0)
				{$atleastonegamecomplete=1;}
		}
		$weekscompleted["$num"] = $complete;
		if ($atleastonegamecomplete==1)
			{$latestweekwithatleast1completegame=$num;}
	}

/*Display body of table by looping through players, then weeks_won*/	
	/*Determine deadline date time for current week to determine whether or not to show links with records to players*/
	$deadlinequery = "SELECT deadline_date FROM weeks WHERE num='$currentweek'";
	$deadlineresult = pg_exec($cxn, $deadlinequery) or die ("Couldn't execute weeks_won weekly records query.");
	$deadlinerow = pg_fetch_assoc($deadlineresult);
	extract ($deadlinerow);
	
	date_default_timezone_set('America/New_York');
	$deadlinedatetime=date("Y-m-d H:i:s", strtotime($deadline_date." 12:00:00"));
	/*echo $currentdatetime." : ".$deadlinedatetime;*/
	/*echo $latestweekwithatleast1completegame;*/
	
	$counter=0;
	$lastrecord=0;
	$tietracker=1;
	switch ($orderby) {
		case 0: /*sort by overall record*/
			$query = "SELECT * FROM player WHERE email<>'admin' ORDER BY correct_picks DESC";
			break;
		case 1: /*sort by current week's record*/
			$query = "
			SELECT 
				p1.*
				,ww1.week_correct_picks
				,tiebreak.game_diff

			FROM player p1

			INNER JOIN weeks_won ww1
			ON p1.id = ww1.player_id
			
			JOIN
				(select 
					pi1.player_id
				,ABS(pi1.tiebreakerpts - g1.tiebreakerpts) as game_diff
				,g1.week_num
					
				from games g1
			    join picks pi1
			    on g1.id = pi1.game_id
			    where 
					g1.week_num = $latestweekwithatleast1completegame
					and g1.tiebreaker <>0) as tiebreak
			ON p1.id = tiebreak.player_id and ww1.week_id = tiebreak.week_num
			
			WHERE p1.email<>'admin'
			
			ORDER BY ww1.week_correct_picks DESC, tiebreak.game_diff ASC
			";
			break;
	}
	$result = pg_exec($cxn, $query) or die ("Couldn't execute player roster query.");
	while ($row=pg_fetch_assoc($result))
	{		
                extract($row);
		switch ($orderby) {
			case 0: /*determine ranking by evaluating $correct_picks and account for ties*/
			if ($correct_picks <> $lastrecord) {
				$counter=$counter+$tietracker;
				$tietracker=1;
				$lastrecord = $correct_picks;
			}
			else {$tietracker=$tietracker+1;}
			break;
			case 1: /*determine ranking by evaluting $week_correct_picks and account for ties*/
			if ($week_correct_picks <> $lastrecord) {
				$counter=$counter+$tietracker;
				$tietracker=1;
				$lastrecord = $week_correct_picks;
			}
			else {$tietracker=$tietracker+1;}
			break;
		}
		if ($id == $_SESSION['logid'])
			{$trclass = "highlightrow";} else {$trclass = "";}
		echo "<tr class='$trclass'>\n";
		echo "<td>$counter.&nbsp</td>\n";
		echo "<td>$firstname"." "."$lastname</td>\n";
		$tquery = "SELECT team_name FROM teams WHERE id=$team_id";
		$tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
		$trow=pg_fetch_assoc($tresult);
		extract($trow);
		echo "<td>$team_name</td>\n";
		
		/*Display overall picks record*/
		echo "<td>$correct_picks - $wrong_picks</td>\n";
		
		/*Determine and display players weeks won record*/
		$weekswon=0;
		$weekstied=0;
		$weekslost=0;
		$wrecordquery = "SELECT week_result, week_id FROM weeks_won WHERE player_id='$id' AND (week_correct_picks > 0 OR week_wrong_picks > 0)";
		$wrecordresult = pg_exec($cxn, $wrecordquery) or die ("Couldn't execute weeks_won overall record query.");
		while ($row=pg_fetch_assoc($wrecordresult))
		{		
			extract($row);
			if ($week_result==1 AND $weekscompleted["$week_id"]==1) {$weekswon=$weekswon+1;}
			if ($week_result==2 AND $weekscompleted["$week_id"]==1) {$weekstied=$weekstied+1;}
			if ($week_result==0 AND $weekscompleted["$week_id"]==1) {$weekslost=$weekslost+1;}
		}
		echo "<td>$weekswon - $weekslost - $weekstied</td>\n";
		echo "<td></td>\n";
		
		/*Determine and display each player's week by week records*/
		$recordquery = "SELECT week_id, week_correct_picks, week_wrong_picks, week_result, money_result FROM weeks_won
			WHERE player_id='$id' AND (week_id >= $fromweek AND week_id <= $toweek) ORDER BY week_id ASC";
		$recordresult = pg_exec($cxn, $recordquery) or die ("Couldn't execute weeks_won weekly records query.");
		while ($row=pg_fetch_assoc($recordresult))
		{		
			extract($row);
			/*echo $currentdate." : ".$deadline_date."<br/";*/
			if ((($week_correct_picks > 0 OR $week_wrong_picks > 0) AND $currentdatetime>$deadlinedatetime) OR $_SESSION['logname']=='admin' OR $week_id<$currentweek)
			{
				if (($week_result==1 OR $week_result==2) AND $weekscompleted["$week_id"]==1) {$class_string="boldweeklyrecords";} else {$class_string="weeklyrecords";}
				if (($money_result==1 OR $money_result==2) AND $weekscompleted["$week_id"]==1) {$ast="*";} else {$ast="";} 
				echo "<td class=\"centerweeklyrecords\"><a class=\"$class_string\" href=\"outcomes.php?week=$week_id&player=$id\">$week_correct_picks - $week_wrong_picks</a>$ast</td>\n";
			}
			else
			{
				echo "<td class=\"centerweeklyrecords\">$week_correct_picks - $week_wrong_picks</td>\n";
			}
		}
		echo "<td></td>\n";		
		echo "</tr>\n";
	}
	echo "</tbody>";
echo "</table>";
echo "<div id=\"tablecaption\">";
echo    "<h4 class=\"blue\">W - L in bold is week's winner(s).&nbsp&nbsp&nbsp</h4>";
echo    "<h4 class=\"blueunderline\">Click to see player's picks.</h4>";
echo    "<h4>&nbsp&nbsp&nbsp&nbsp* denotes weekly prize winner(s)</h4>";
echo "</div><br/><br/>"; /*end of tablecaption div*/

echo "</div>"; /*end of body-content div*/
include_once("includes/footer.inc");
echo "</div>";  /*end of wrapper div*/
echo "</body>";
echo "</html>";
?>