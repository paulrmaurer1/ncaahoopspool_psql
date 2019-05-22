<?php ob_start();session_start();?>
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

echo "<body id=\"help\">\n";
echo "<div id=\"wrapper\">\n";
		
include_once("includes/title_menu.inc");

echo "<div id=\"body-content\">";
echo "<h2>Help</h2>";

echo "<div id=\"helpcontent\">";

if ($_SESSION['logname']=='admin')
	{/*show help contents for Administrators*/
		echo "<h2><span style='color:blue; font-weight:bold'>Overall Summary</span> - This page shows the standings of players ranked by overall <strong>Picks Record</strong>,
		or the total number of correct and incorrect picks for the season to that point.  Other columns on this table are:</h2>";
		echo "<ul>\n";
		echo "<li><strong>Weeks Record</strong> - The record of the # of weeks a player picked, tied, or lost for the most correct picks.</li>";
		echo "<li><strong>Week #</strong> - The week by week record of picks for each player (e.g. 10-8).  Once the deadline has passed and the Administrator has recorded at least one game
		result, the records will be clickable (e.g. <span style='color:blue; text-decoration:underline'>10-8</span>).  Click on the link to View or Edit (Administrator only) any of the player's picks.
		Once all of the game results have been submitted by the Administrator, winners will be highlighted as follows.  If the record is
		in bold (e.g. <span style='color:blue; text-decoration:underline; font-weight:bold'>10-8</span>), the player won or tied for the most correct picks for the week.
		If the record is followed by an asterisk (e.g. <span style='color:blue; text-decoration:underline; font-weight:bold'>10-8</span>*),
		the player won or tied for the money prize for the week.</li>";
		echo "</ul><br/>";
		
		echo "<h2><span style='color:blue; font-weight:bold'>Setup Games</span> - These are the pages (there is a separate page for each week of the season) where the
		Administrator will select opponents for each team in the pool.  The following columns are displayed and used to setup each game:</h2>";
		echo "<ul>\n";
		echo "<li><strong>TB</strong> - Select the radio button next to the game that is the Tiebreaker game for the week.</li>";
		echo "<li><strong>Date</strong> - Select the date for the game.</li>";
		echo "<li><strong>Locale</strong> - Select whether the game is Home, Away, Neutral or No game.</li>";
		echo "<li><strong>Team</strong> - The pool team for that game, defaulted for Administrator.</li>";
		echo "<li><strong>Opponent</strong> - Select the opponent for each game of the week.</li>";
		echo "</ul>";
		echo "<h2>After selecting the above criteria for each game, click the 'Save and View' button at the bottom
		to save your game selections and view how the schedule will appear to players.  You will be taken to the 'Update Results' page for that week.</h2>";
		
		echo "<h2><span style='color:blue; font-weight:bold'>Update Results</span> - These are the pages (there is a separate page for each week of the season)
		where the Administrator will update each game's results.  Once the results are added or edited click the 'Save, Update Summary'
		button to update the Overall Summary page and save the results to the database.
		There are additional button on this page for the Administrator:</h2>";
		echo "<ul>\n";
		echo "<li><strong>'Edit Games'</strong> button - this will take you to the 'Setup Games' page for that week in
		case you need to change a game opponent, locale, date, etc.</li>";
		echo "<li><strong>'Email Schedule to ALL'</strong> button - once you are satisfied with all of the games that you setup for the week,
		click this button to send an email to the entire roster with the schedule for the displayed week.</li>";
		echo "<li><strong>'Email to List Below'</strong> button - this does the same thing as the 'Email Schedule to ALL'
		button but only sends an email to the list of players (below) who haven't yet completed all of their picks.</li>";
		echo "</ul>";

		echo "<h2><span style='color:blue; font-weight:bold'>Roster</span> - This is the page where the Administrator can Add or Delete players from the Roster.
		Deleting a player will delete all of the selected player's picks and games for his team so be careful when using this button.  For each player, add the following:</h2>";
		echo "<ul>\n";
		echo "<li><strong>First Name, Last Name</strong> - Name of each player on the roster.</li>";
		echo "<li><strong>Email</strong> - The email address that is used by the player to register & login.  Also the address where emails from the Administrator are sent.</li>";
		echo "<li><strong>Team Name</strong> - The team chosen by the player.  Each week of the season will be populated with an entry for each player's team.</li>";
		echo "<li><strong># Team Wins</strong> - The # of games predicted to be won by each player's team.
		Used as a tie-breaker for the overall season record in the event of ties.</li>";
		echo "</ul><br/>";
	}
	else /*Help contents for players*/
	{
		echo "<h2><span style='color:blue; font-weight:bold'>Overall Summary</span> - This page shows the standings of players ranked by overall <strong>Picks Record</strong>,
		or the total number of correct and incorrect picks for the season to that point.  Other columns on this table are:</h2>";
		echo "<ul>\n";
		echo "<li><strong>Weeks Record</strong> - The record of the # of weeks a player picked, tied, or lost for the most correct picks.</li>";
		echo "<li><strong>Week #</strong> - The week by week record of picks for each player (e.g. 10-8).  Once the deadline has passed and the Administrator has recorded at least one game
		result, the records will be clickable (e.g. <span style='color:blue; text-decoration:underline'>10-8</span>).  Click on the link to View any of the player's picks.
		Once all of the game results have been submitted by the Administrator, winners will be highlighted as follows.  If the record is
		in bold (e.g. <span style='color:blue; text-decoration:underline; font-weight:bold'>10-8</span>), the player won or tied for the most correct picks for the week.
		If the record is followed by an asterisk (e.g. <span style='color:blue; text-decoration:underline; font-weight:bold'>10-8</span>*),
		the player won or tied for the money prize for the week.</li>";
		echo "</ul>";

		echo "<h2><span style='color:blue; font-weight:bold'>My Games (pre Deadline)</span> - These are the pages (there is a separate page for
		each week of the season) where the player will choose winners of each week's games prior to the deadline
		(by 12:00PM EST on the Monday of each week).  The following are columns displayed on this page:</h2>";
		echo "<ul>\n";
		echo "<li><strong>TB</strong> - An asterisk denotes the Tiebreaker game for the week.</li>";
		echo "<li><strong>Date</strong> - The date for the game.</li>";
		echo "<li><strong>Team</strong> - The pool team for that game.  If there's an \"at\" before the team's name, this is a home game for the player's Team.</li>";
		echo "<li><strong>Opponent</strong> - The opponent for each game of the week. If there's an \"at\" before the opponent's name, this is away game for the player's Team.
		(If there is not an \"at\" before either the Team or Opponent's name, the game is neutral.  If it says, \"No game this week\",
		 the Team doesn't have a scheduled game for the week.</li>";
		echo "<li><strong>Tiebreaker Game Points Total</strong> - Input the sum total # of points you think will be scored by both teams for the Tiebreaker game.</li>";
		echo "</ul>";
		echo "<h2>After choosing a winner for each game, click the <strong>'Save and View'</strong> button at the bottom to save your game selections and
		view how the schedule will appear to other players.  You will be taken to the 'My Games' (post Deadline) view of your games.</h2>";

		echo "<h2><span style='color:blue; font-weight:bold'>My Games (post Deadline)</span> - These are the pages (there is a separate page
		for each week of the season) where the player sees his current picks.  Once game results are updated by the Administrator,
		the results of each game will be displayed as follows:</h2>";
		echo "<ul>\n";
		echo "<li>x [Teamname] in black/normal - The player's chosen winner for each game shown prior to the results being updated.</li>";
		echo "<li><span style='color:green; font-weight:bold'>x [Teamname] in green/bold</span> - Your pick was correct and winner of the game.</li>";
		echo "<li><span style='color:red; font-weight:bold; text-decoration: line-through;'>x [Teamname] in red/strike-through</span> - Your pick was incorrect.</li>";
		echo "<li><span style='color:green'>[Teamname] in green/normal</span> - The actual winner of an incorrectly picked game.</li><br/>";
		
		echo "<li><strong>'Edit' button</strong> - This button will be available prior to the deadline date (12:00PM EST on Monday of each week)
		so you can go back the edit your winning team selections.</li>";
		echo "</ul>";
		
		echo "<h2><span style='color:blue; font-weight:bold'>Roster</span> - This is the page where you can view the players participating in the pool.
		For each player, the following is displayed:</h2>";
		echo "<ul>\n";
		echo "<li><strong>First Name, Last Name</strong> - Name of each player on the roster.</li>";
		echo "<li><strong>Email</strong> - The email address that is used by the player to register & login.  Also the address where emails from the Administrator are sent.</li>";
		echo "<li><strong>Team Name</strong> - The team chosen by the player.  Each week of the season will be populated with an entry for each player's team.</li>";
		echo "<li><strong># Team Wins</strong> - The # of games predicted to be won by each player's team.
		Used as a tie-breaker for the overall season record in the event of ties.</li>";
		echo "</ul><br/>";
	}
echo "</div>"; /*end of helpcontent div*/
	
echo "</div>"; /*end of body-content div*/
include_once("includes/footer.inc");
echo "</div>";  /*end of wrapper div*/
echo "</body>";
echo "</html>";
?>