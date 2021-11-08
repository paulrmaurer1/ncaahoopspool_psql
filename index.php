<!doctype html>
<html lang=en>
<?php
	// Start session 
	session_start();

	// If user not authorized, redirect to login page
	if (isset($_SESSION['auth'])) {
		if($_SESSION['auth']!= "yes") {
			header ("Location: login.php");
			exit();
		}
	}
	else {
		header ("Location: login.php");
		exit();
	}

	include_once("includes/head.inc");
	include_once("includes/misc.inc");
?>

<body id="summary">
	<div id="wrapper">
	<?php 
		include_once("includes/title_menu.inc");
		include_once("includes/sharedfunctions.inc");
	?>
		<div id="body-content">
		<h2>Overall Summary</h2>
		<?php
			// Determine proper view parameters for table

			// Determine proper page view to show (weeks 1-6[0], 7-12[1], or 13-18[2])
			if (isset($_GET['view'])) {
				$view = $_GET['view'];
			} else {
				$view = getCurrentView($currentweek);
			}

			// Get links for prior (<<) and next (>>) week links and $fromweek, $toweek for table column headers
			list($previousview_id, $nextview_id, $fromweek, $toweek) = getViewParameters($view);

			/*Determine which sort order to display players in and display options at top*/
			if (isset($_GET['orderby'])) {
				$orderby=$_GET['orderby'];
			} else {
				/*default order by current week id (=1)*/
				$orderby=1;
			}

			/* Change Re-order by option to other than that which is displayed for toggling*/
			switch ($orderby) {
				case 0:
					echo "<h5>Re-order by:&nbsp&nbsp&nbsp<a class=\"order\" href=\"index.php?view=$view&orderby=1\">Current Week</a></h5>";
					break;
				case 1:
					echo "<h5>Re-order by:&nbsp&nbsp&nbsp<a class=\"order\" href=\"index.php?view=$view&orderby=0\">Picks Record</a></h5>";
					break;
			}
		?>
		<!-- end Overall Summary table building parameters -->
		<!-- Build table head -->
		<table class="summary">
			<thead>
				<tr>
					<th></th>
					<th class="name">Name</th>
					<th class="team">Team</th>
					<th class="precord">Picks Record</th>
					<th class="wrecord">Weeks Record</th>
					<?php
						// Display week nums across column headers
						echo "<th><a href=\"index.php?view=$previousview_id&orderby=$orderby\"><<</a></th>";
						$query = "SELECT num FROM weeks WHERE num >= $fromweek AND num <= $toweek ORDER BY num ASC";
						$result = pg_exec($cxn, $query) or die ("Couldn't execute week range query.");
						while ($row=pg_fetch_assoc($result)) {
							extract($row);
							echo "<th class=\"week\">Week $num</th>";
						}
						echo "<th><a href=\"index.php?view=$nextview_id&orderby=$orderby\">>></a></th>";
					?>
				</tr>
			</thead>
			<!-- Build table body -->
			<tbody>
			<?php
				/*Build weeks completed array (to only show winners after all game outcomes are final*/
				$weekscompleted = array();
				$latestweekwithatleast1completegame=1;
				$weeksquery = "SELECT num FROM weeks ORDER BY num ASC";
				$weeksresult = pg_exec($cxn, $weeksquery) or die ("Couldn't execute weeks_won weekly records query.");
				while ($weeksrow=pg_fetch_assoc($weeksresult)) {
					$complete=1;
					$atleastonegamecomplete=0;
					extract($weeksrow);
					$gamesquery = "SELECT locale, outcome FROM games WHERE week_num = '$num'";
					$gamesresult = pg_exec($cxn, $gamesquery) or die ("Couldn't execute weeks_won weekly records query.");
					while ($gamesrow=pg_fetch_assoc($gamesresult)) {
						extract($gamesrow);
						if ($outcome==0 AND $locale>0) {
							$complete=0;
						}
						if ($outcome<>0 AND $locale>0) {
							$atleastonegamecomplete=1;
						}
					}
					$weekscompleted["$num"] = $complete;
					if ($atleastonegamecomplete==1) {
						$latestweekwithatleast1completegame=$num;
					}
				}

				/*Display body of table by looping through players, then weeks_won*/	
				/*Determine deadline date time for current week to determine whether or not to show links with records to players*/
				$deadlinequery = "SELECT deadline_date FROM weeks WHERE num='$currentweek'";
				$deadlineresult = pg_exec($cxn, $deadlinequery) or die ("Couldn't execute weeks_won weekly records query.");
				$deadlinerow = pg_fetch_assoc($deadlineresult);
				extract ($deadlinerow);
				
				date_default_timezone_set('America/New_York');
				$deadlinedatetime=date("Y-m-d H:i:s", strtotime($deadline_date." 12:00:00"));
				/*echo $currentdatetime. : .$deadlinedatetime;*/
				/*echo $latestweekwithatleast1completegame;*/
				
				$counter=0;
				$lastrecord=0;
				$tietracker=1;
				$query = getPlayerSortQuery($orderby, $latestweekwithatleast1completegame);
				$result = pg_exec($cxn, $query) or die ("Couldn't execute player roster query.");
				while ($row=pg_fetch_assoc($result)) {		
			        extract($row);
			        // Determine order #, i.e. $counter, for place of player
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
					if ($id == $_SESSION['logid']) {
						$trclass = "highlightrow";
					} else {
						$trclass = "";
					}
					echo "<tr class='$trclass'>\n";
					echo "<td>$counter.&nbsp</td>\n";

					echo "<td>$firstname"." "."$lastname</td>\n";
					
					// Display team name
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
					while ($row=pg_fetch_assoc($wrecordresult)) {		
						extract($row);
						if ($week_result==1 AND $weekscompleted[$week_id]==1) {$weekswon=$weekswon+1;}
						if ($week_result==2 AND $weekscompleted[$week_id]==1) {$weekstied=$weekstied+1;}
						if ($week_result==0 AND $weekscompleted[$week_id]==1) {$weekslost=$weekslost+1;}
					}
					echo "<td>$weekswon - $weekslost - $weekstied</td>\n";
					echo "<td></td>\n";
					
					/*Determine and display each player's week by week records*/
					$recordquery = "SELECT week_id, week_correct_picks, week_wrong_picks, week_result, money_result FROM weeks_won
						WHERE player_id='$id' AND (week_id >= $fromweek AND week_id <= $toweek) ORDER BY week_id ASC";
					$recordresult = pg_exec($cxn, $recordquery) or die ("Couldn't execute weeks_won weekly records query.");
					while ($row=pg_fetch_assoc($recordresult)) {		
						extract($row);
						/*
						If at least 1 game result has been updated, user is admin or week has been completed, 
						show weekly record with link to outcomes page (to see picks)
						*/
						if ((($week_correct_picks > 0 OR $week_wrong_picks > 0) AND $currentdatetime>$deadlinedatetime) OR $_SESSION['logname']=='admin' OR $week_id<$currentweek) {
							if (($week_result==1 OR $week_result==2) AND $weekscompleted["$week_id"]==1) {
								$class_string="boldweeklyrecords";
							} else {
								$class_string="weeklyrecords";
							}
							if (($money_result==1 OR $money_result==2) AND $weekscompleted["$week_id"]==1) {
								$ast="*";
							} else {
								$ast="";
							} 
							echo "<td class=\"centerweeklyrecords\"><a class=\"$class_string\" href=\"outcomes.php?week=$week_id&player=$id\">$week_correct_picks - $week_wrong_picks</a>$ast</td>\n";
						} else {
							// otherwise, just show weekly record without a link
							echo "<td class=\"centerweeklyrecords\">$week_correct_picks - $week_wrong_picks</td>\n";
						}
					}
					echo "<td></td>\n";		
					echo "</tr>\n";
				}
			?>
			</tbody>
		</table>
		<div id="tablecaption">
		    <h4 class="blue">W - L in bold is week's winner(s)&nbsp&nbsp&nbsp</h4>
		    <h4 class="blueunderline">Click to see player's picks.</h4>
		    <h4>&nbsp&nbsp&nbsp * denotes weekly prize winner(s)</h4>
		</div> <!-- end of tablecaption div -->
		<br/><br/>
	</div> <!-- end of wrapper div -->
	<?php include_once("includes/footer.inc"); ?>
</div> <!-- end of body-content div -->
</body>
</html>