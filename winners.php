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
?>
<head>
    <title>NCAAHoopsPool.com</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <link href="style1.css" rel="stylesheet" type="text/css"/>
    
    <script src="_js/jquery-1.7.2.min.js"></script>
    
    <script>
        $(document).ready(function() {
            $('#messagebox').hide();
            $('#listbelow').click(function() {
                var position = $(this).offset();
                $('#messagebox').show();
            }); //end click listbelow button
            $('#messagebox h3').click(function() {
               $('#messagebox').hide(); 
            });
        }); //end ready
    </script>
</head>

<?php
/*include_once("includes/head_winners.inc");*/
include_once("includes/misc.inc");
include_once("includes/mailfunctions.inc");

echo "<body id=\"winners".$_GET['week']."\">\n";
echo "<div id=\"wrapper\">\n";

include_once("includes/title_menu.inc");

echo "<div id=\"body-content\">";
echo "<h2>Update Winners</h2>";

$viewingplayer=$_SESSION['logid'];

/*************************************************************************************************/
/*Update all results to games table.  Update all pick results to picks, player, weeks_won tables.*/
/*************************************************************************************************/

if (isset($_POST['winners']))
{
    foreach ($_POST['winners'] as $selects)
    {
        /*Update games table with winning results posted by admin on displaypicksform*/
        IF (isset($selects['winner']))
	{
            $query="UPDATE games SET outcome=".$selects['winner']." WHERE id='".$selects['gameid']."'"; /*update game outcome for each game*/
            $result=pg_exec($cxn,$query) or die ("Could not execute game outcome update query");
        
            /*Update picks table pick_result for each player depending on outcome of game*/
            $query = "SELECT games.outcome, picks.pick, picks.id FROM games INNER JOIN picks on games.id=picks.game_id WHERE games.id='".$selects['gameid']."'";
            $result = pg_exec($cxn, $query) or die ("Couldn't execute matching picks table select query.");
            while ($row=pg_fetch_assoc($result)) 
            {
                extract($row);
                /*echo $selects['gameid']." : ".$id." : ".$outcome." : ".$pick."<br/>";*/
                if ($outcome==$pick)
                {
                    $pickresultquery="UPDATE picks SET pick_result=1 WHERE game_id='".$selects['gameid']."' AND id='$id'"; /*update pick_result for each matching game*/
                    $pickresult=pg_exec($cxn,$pickresultquery) or die ("Could not execute pick pick_result=1 update query");
                }
                else
                {
                    $pickresultquery="UPDATE picks SET pick_result=0 WHERE game_id='".$selects['gameid']."' AND id='$id'"; /*update pick_result for each matching game*/
                    $pickresult=pg_exec($cxn,$pickresultquery) or die ("Could not execute pick pick_result=0 update query");
                }
            }
        }
        if ($_POST['tbgame']==$selects['gameid'])
        {
            $query="UPDATE games SET tiebreakerpts=".$_POST['tiebreakerpts']." WHERE id='".$selects['gameid']."'"; /*update tiebreaker points for tiebreaker game*/
            $result=pg_exec($cxn,$query) or die ("Could not execute tiebreaker game points update query");
        }
        else
        {
            $query="UPDATE games SET tiebreakerpts=0 WHERE id='".$selects['gameid']."'"; /*update tiebreaker points to 0 for non-tiebreaker game*/
            $result=pg_exec($cxn,$query) or die ("Could not execute non-tiebreaker game update query");

        }
    }
    /*Update total correct and wrong picks for every player*/

    $query = "SELECT id FROM player";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute matching picks table select query.");
    while ($row=pg_fetch_assoc($result)) 
    {
        $playercorrectpickcounter=0;
        $playerwrongpickcounter=0;

        $playerweekcorrectpickcounter=0;
        $playerweekwrongpickcounter=0;
        
        extract($row);
        $pickquery = "SELECT picks.pick_result, games.week_num FROM picks INNER JOIN games ON games.id=picks.game_id
        WHERE picks.player_id='$id' AND games.outcome>0";
        $pickresult = pg_exec($cxn, $pickquery) or die ("Couldn't execute matching picks table select query.");
        while ($pickrow=pg_fetch_assoc($pickresult)) 
        {
            extract($pickrow);
            if ($pick_result==1)
                {$playercorrectpickcounter=$playercorrectpickcounter+1;}
                else {$playerwrongpickcounter=$playerwrongpickcounter+1;}
            if ($week_num==$_GET['week'] AND $pick_result==1)
                {$playerweekcorrectpickcounter=$playerweekcorrectpickcounter+1;}
            if  ($week_num==$_GET['week'] AND $pick_result==0)
                {$playerweekwrongpickcounter=$playerweekwrongpickcounter+1;}
        }
        $updatequery = "UPDATE player SET correct_picks=$playercorrectpickcounter, wrong_picks=$playerwrongpickcounter WHERE id='$id'";
        $updateresult = pg_exec($cxn,$updatequery) or die ("Could not execute non-tiebreaker game update query");
        $updatequery = "UPDATE weeks_won SET week_correct_picks=$playerweekcorrectpickcounter, week_wrong_picks=$playerweekwrongpickcounter
        WHERE player_id = '$id' AND week_id='".$_GET['week']."'";
        $updateresult = pg_exec($cxn,$updatequery) or die ("Could not execute non-tiebreaker game update query");
    }
    
    /*Update weeks_won table with player records for updated week*/
    /*Determine the most correct picks for a specific week*/
    $maxquery = "SELECT MAX(week_correct_picks) FROM weeks_won WHERE week_id='".$_GET['week']."'";
    $maxresult = pg_exec($cxn,$maxquery) or die ("Could not execute max correct picks query");
    $maxrow = pg_fetch_row($maxresult);
    
    /*Determine the number of players who picked the most correct picks for a specific week*/
    $numwinnersquery = "SELECT id FROM weeks_won WHERE week_correct_picks='".$maxrow[0]."' AND week_id='".$_GET['week']."'";
    $numwinnersresult = pg_exec($cxn,$numwinnersquery) or die ("Could not execute num winners query");
    $numwinners = pg_numrows($numwinnersresult);
    
    /*Update the weeks_won week_result with winners, including ties*/
    $tbdiff=array();
    $playerquery = "SELECT id, week_correct_picks, player_id FROM weeks_won WHERE week_id='".$_GET['week']."'";
    $playerresult = pg_exec($cxn, $playerquery) or die ("Couldn't execute matching picks table select query.");
    while ($playerrow=pg_fetch_assoc($playerresult)) 
        {
            extract($playerrow);
            if ($week_correct_picks==$maxrow[0])
                {if ($numwinners==1)
                    {
                        $updatequery="UPDATE weeks_won SET week_result=1 WHERE id='$id'";
                        $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won week_result update query");
                        $updatequery="UPDATE weeks_won SET money_result=1 WHERE id='$id'";
                        $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won week_result update query");
                    }
                else
                    {
                        $updatequery="UPDATE weeks_won SET week_result=2 WHERE id='$id'";
                        $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won week_result update query");
                        
                        /*Build tiebreakerpoints (absolute) difference array to determine $$ winner in next section if ties*/
                        $selecttbquery="SELECT games.tiebreakerpts AS gametiebreakerpts, picks.tiebreakerpts FROM games INNER JOIN picks ON games.id=picks.game_id
                        WHERE picks.player_id='$player_id' AND games.tiebreaker='1' AND week_num='".$_GET['week']."'";
                        $selecttbresult = pg_exec($cxn,$selecttbquery) or die ("Could not execute tiebreaker lookups query");
                        $tbrow = pg_fetch_assoc($selecttbresult);
                        extract($tbrow);
                        $playertbdiff = abs($gametiebreakerpts-$tiebreakerpts);
                        $tbdiff["$player_id"] = $playertbdiff;
                        $updatequery="UPDATE weeks_won SET money_result=0 WHERE id='$id'";
                        $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won week_result update query");

                    }
                }
            else
            {
                $updatequery="UPDATE weeks_won SET week_result=0 WHERE id='$id'";
                $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won week_result update query");
                $updatequery="UPDATE weeks_won SET money_result=0 WHERE id='$id'";
                $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won week_result update query");

            }
        }
    /*update weeks_won money_result if multiple winners for a week*/
    if ($tbdiff)
    {
        $tbdiffties=array();
        $mintbdiff = min($tbdiff); /*is closest player tiebreakerpts to game total*/
        $index=0;
        /*fill array with any players who had this closest difference*/
        foreach($tbdiff as $field => $value) 
	    {if ($value==$mintbdiff)
                {
                    $tbdiffties["$index"] = $field;
                    $index=$index+1;
                }
            }
        /*update weeks_won money_result fields for winner or ties*/
        if (count($tbdiffties)==1)
        {
            $updatequery="UPDATE weeks_won SET money_result=1 WHERE player_id=$tbdiffties[0] AND week_id='".$_GET['week']."'";
            $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won money_result if tied update query");
        }
        else
        {
            foreach($tbdiffties as $field => $value)
            {
                $updatequery="UPDATE weeks_won SET money_result=2 WHERE player_id=$value AND week_id='".$_GET['week']."'";
                $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won money_result if tied update query");
            }
        }
    }
                /* Old code to determine single Price is Right winner*/
                /*
                if (max($tbdiff)<0)
                {
                    $tbindex=max($tbdiff);
                    $tbdiffplayer=array_flip($tbdiff);
                    $winningplayer=$tbdiffplayer["$tbindex"];
                }
                elseif (min($tbdiff)>=0)
                {
                    $tbindex=min($tbdiff);
                    $tbdiffplayer=array_flip($tbdiff);
                    $winningplayer=$tbdiffplayer["$tbindex"];
                }
                else
                {
                    $tbdiffpositives = array_filter($tbdiff, function($v) { return $v >= 0; });
                    $tbindex=min($tbdiffpositives);
                    $tbdiffplayer=array_flip($tbdiffpositives);
                    $winningplayer=$tbdiffplayer["$tbindex"];
                }
                $updatequery="UPDATE weeks_won SET money_result=1 WHERE player_id='$winningplayer' AND week_id='".$_GET['week']."'";
                $updateresult=pg_exec($cxn,$updatequery) or die ("Could not execute weeks_won money_result if tied update query");*/
    
                /*echo $winningplayer."<br/>";*/
                /*print_r($tbdiff);*/
    
    /****Go to different page or perform specific action depending on button clicked****/
    $currentweekid=$_GET['week'];
    $emailmessage = $_POST['email_message'];
    $emailsubject = $_POST['email_subject'];
    if ($_POST['display_button']=="Edit Games") {header("Location: games.php?week=$currentweekid");} /*If clicked just 'Save' redirect back to games.php page*/
    if ($_POST['display_button']=="Save, Update Summary") {echo "<p class='success'>Games results and the Overall Summary have been successfully updated!</p>";}
    if ($_POST['display_button']=="Email Schedule to ALL") {mailschedule($currentweekid, 1, $emailmessage, $emailsubject); echo "<p class='success'>Emails have been sent to the entire roster!</p>";}
    if ($_POST['display_button']=="Email to List Below") {mailschedule($currentweekid, 0, $emailmessage, $emailsubject); echo "<p class='success'>Emails have been sent to the players who have not submitted picks yet!</p>";}
}


/*Retrieve current week parameters to display proper info and games on page*/
$currentweekid=$_GET['week'];
$query = "SELECT * FROM weeks WHERE num=$currentweekid";
$result = pg_exec($cxn, $query) or die ("Couldn't execute weeks table query.");
$row=pg_fetch_assoc($result);
extract($row);
echo "<h3> Week&nbsp".$currentweekid." (".date('F jS Y',strtotime($from_date))." - ".date('F jS Y',strtotime($to_date)).")</h3>";
echo "Select winners for this week<br/><br/>";

echo "<form action=\"winners.php?week=".$currentweekid."\" method = 'POST'>";
    include_once("includes/displaypicksform.inc"); /*display Pick Games form*/

    /*build default subject line for form below*/
    $query = "SELECT deadline_date FROM weeks WHERE num=$currentweekid";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute firstname, lastname lookup from player table for mail.");
    $row=pg_fetch_assoc($result);
    extract($row);
    
    $subject = "Week ".$currentweekid." schedule. Please submit your picks by ".date('l\,\ M j',strtotime($deadline_date)).".";
?>
<!--Pop-up message box that gets prompted before sending emails-->
<div id="messagebox">
        <h2>Enter your email Subject:</h2>
        <input type="text" name="email_subject" id="email_subject" value="<?php echo $subject ?>"/>
        <h2>Enter your message:</h2>
        <textarea name="email_message" rows="3" cols="60" id="email_message"></textarea>
        <p>
            <input type="submit" name="display_button" value="Email to List Below" id="submit_button"/>
            <input type="submit" name="display_button" value="Email Schedule to ALL" id="submit_button"/>
            <h3>Cancel</h3>
        </p>
</div>

<?php
echo "</form>";
echo "<h4 style='color:red;'>Players who have not submitted all of their picks yet:</h4>";
    include_once("includes/displayincompletepicksnames.inc");

echo "</div>"; /*end of body-content div*/
include_once("includes/footer.inc");
echo "</div>"; /*end of wrapper div*/
echo "</body>";
echo "</html>";
?>