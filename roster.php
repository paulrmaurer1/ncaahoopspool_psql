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
	include_once("includes/checkrosterform.inc");

	echo "<body id=\"roster\">\n";
	echo "<div id=\"wrapper\">\n";

	include_once("includes/title_menu.inc");

	echo "<div id=\"body-content\">";
	if($_SESSION['logname']=='admin') {
		echo "<h2>Please enter a pool participant</h2>";
	} else {
		echo "<h2>Participant Roster</h2>";
	}

	if($_POST) {
		if($_POST['display_button'] == "Add Player") {
			if (checkrosterform($_POST)) { /* Check to see if errors in form entries */
				/*If no errors, add player by inserting rows to player, picks, games and weeks_won tables*/
				/*display success message*/
				echo $message;
				
				/*loop to insert new player record into player table*/
				foreach($_POST as $field => $value) {
				    if ($field<>'display_button') {
						$_POST[$field] = trim($_POST[$field]);
						$_POST[$field] = strip_tags($_POST[$field]);
						$field_array[]=$field;
						$value_array[]=$value;
				    }
				}
				$fields=implode(",",$field_array);
				$values= implode("','",$value_array);
				
				/*echo "<h3>$values</h3><br/>";*/
				/*echo "<h3>$fields</h3>";*/
				
				/*insert new player record into player table*/
				$query="INSERT INTO player ($fields) VALUES ('$values') RETURNING id"; 
				$result=pg_exec($cxn,$query) or die ("Could not execute player insert query");
				$insert_row = pg_fetch_row($result);
				$playerid = $insert_row[0];

				// $playerid=mysqli_insert_id($cxn); /*retrieve id of new player just inserted*/
				
				/*loop to insert new picks for new player for existing games*/
				$query="SELECT id FROM games"; 
				$gameresult=pg_exec($cxn,$query) or die ("Could not execute games SELECT query");
				while ($gamerow=pg_fetch_assoc($gameresult)) {
					extract($gamerow);
					$query="INSERT INTO picks (player_id,game_id) VALUES ('$playerid','$id')"; /*insert 1 pick for new player for each existing game*/
					$result=pg_exec($cxn,$query) or die ("Could not execute picks table insert query");
				}			
				
				/*loop to insert new games and picks for new team for all players for each week of season (except Admin)*/
				foreach($_POST as $field => $value) {
					if ($field=='team_id') {
						$query="SELECT * FROM weeks";
						$weekresult=pg_exec($cxn,$query) or die ("Could not execute week SELECT query");
						while ($weekrow=pg_fetch_assoc($weekresult)) {
							extract($weekrow);
							/*echo $field.": ".$value."; ".$weekrow['id'];*/
							$week_num=$weekrow['num'];
							$query="INSERT INTO games ($field,week_num) VALUES ('$value','$week_num') RETURNING id"; /*insert 1 game for each week into game table*/
							$result=pg_exec($cxn,$query) or die ("Could not execute games table insert query");
							$insert_row = pg_fetch_row($result);
							$gameid = $insert_row[0];

							// $gameid=mysqli_insert_id($cxn); /*retrieve id of row just inserted*/
							
							$query="SELECT id, email FROM player"; /*retrieve ids of player roster for insertion of 1 pick per player per week for new team into picks table*/
							$playerresult=pg_exec($cxn,$query) or die ("Could not execute player SELECT query");
							while ($playerrow=pg_fetch_assoc($playerresult)) {
								extract($playerrow);
								/*echo $id." : ".$gameid;*/
								if ($email<>'admin') { /*Check to make sure admin doesn't have Picks created*/
									$query="INSERT INTO picks (player_id,game_id) VALUES ('$id','$gameid')"; /*insert 1 pick for each week for all players for new team into picks table*/
									$result=pg_exec($cxn,$query) or die ("Could not execute picks table insert query");
								}
							}
							
							/*insert records into weeks_won table for player for each week of season*/
							$query="INSERT INTO weeks_won (week_id,player_id,week_result) VALUES ('$week_num','$playerid','0')";
							$result=pg_exec($cxn,$query) or die ("Could not execute games table insert query");
						}
					}
				}
				
				/*Clear out form fields for next entry*/
				$_POST = array();  
			} else {
			/*If errors, display warning messages and re-display form*/
				echo $message;
			}
		} else {
		/*Delete players who were checked on roster form.  Delete appropriate rows from player, picks, games and weeks_won tables*/
			foreach($_POST['deleteplayer'] as $field => $value) {
				/* echo "<h3>value: $value field: $field</h3>"; */
				$query="SELECT team_id FROM player WHERE id=$value"; /*find associated team for deleted player*/
				$tresult=pg_exec($cxn,$query) or die ("Could not execute player lookup query");
				$trow=pg_fetch_row($tresult);
				$query="SELECT id FROM games WHERE team_id=$trow[0]"; 
				$gamedelresult=pg_exec($cxn,$query) or die ("Could not execute games SELECT query");
				while ($gamedelrow=pg_fetch_assoc($gamedelresult)) {
					extract($gamedelrow);
					$query="DELETE FROM picks WHERE game_id=$id"; /*delete all pick records for any games with deleted player team*/
					$result=pg_exec($cxn,$query) or die ("Could not execute games delete query");
				}
				$query="DELETE FROM games WHERE team_id=$trow[0]"; /*delete games for that player's team from games table*/
				$result=pg_exec($cxn,$query) or die ("Could not execute games delete query");
				$query="DELETE FROM picks WHERE player_id=$value"; /*delete pick records for that player from picks table*/
				$result=pg_exec($cxn,$query) or die ("Could not execute games delete query");
				$query="DELETE FROM player WHERE id=$value"; /*delete player record from player table*/
				$result=pg_exec($cxn,$query) or die ("Could not execute player delete query");
				$query="DELETE FROM weeks_won WHERE player_id=$value"; /*delete player records from weeks_won table*/
				$result=pg_exec($cxn,$query) or die ("Could not execute player delete query");
			}
			echo '<p class="success">Player(s) successfully deleted</p>';
		} //end else
	} //end if($_POST)
?>
	<form action="roster.php" method='POST'>
		<?php
			if($_SESSION['logname']=='admin') {
				/*display roster entry form for admin user*/
				include_once("includes/displayrosterform.inc");
			}
			include_once("includes/displayroster.inc"); /*display roster below form for all*/
		?>
	</form><br/><br/>
<?php
	echo "</div>"; /*end of body-content div*/
	include_once("includes/footer.inc");
	echo "</div>";  /*end of wrapper div*/
	echo "</body>";
	echo "</html>";
?>
