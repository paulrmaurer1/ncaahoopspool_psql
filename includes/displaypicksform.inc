<!--block of code to display the Update Results week for admin user to choose game winners-->
<table class="picks">
    <thead>
        <tr>
            <th></th>
            <th class="tiebreaker">TB</th>
            <th class="date">Date</th>
            <th class="team">Team</th>
            <th class="opponent">Opponent</th>
        </tr>
    </thead>
    <tbody>
        <?php
            $counter=0;
            $query = "SELECT * FROM games WHERE week_num=$currentweekid AND locale<>0 ORDER BY gamedate"; /* */
            $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query.");
            while ($row=pg_fetch_assoc($result)) /*display existing games and values from games table   $result->fetch_assoc()*/ 
            {
                extract($row);
                $counter=$counter+1;
                echo "<input type='hidden' name='winners[$counter][gameid]' value=$id>";
                /*echo "<input type='hidden' name='currentweekid' value=$currentweekid>";*/
                echo "<tr>";
                echo "<td>$counter.&nbsp</td>\n";
                /*display tiebreaker game with asterisk in column*/
                echo "<td>";
                if ($tiebreaker==1)
                {
                    echo"<strong>*</strong>";
                    echo"<input type='hidden' name='tbgame' value=$id>";
                }
                echo "</td>";
                /*display date for each game*/
                echo "<td>".date('F jS Y',strtotime($gamedate))."</td>\n";
                /*display Team name*/
                if ($locale==1) {$homeat="at";} else {$homeat="";}
                $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
                $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
                $trow=pg_fetch_assoc($tresult);
                extract($trow);
                /*override $outcome variable with pick from picks table to non-admin user (id<>0) to show previous made selections vs. taking game results that admin set*/
                if ($viewingplayer<>0)
                {
                    $pquery = "SELECT pick FROM picks WHERE game_id=$id AND player_id='$viewingplayer'";
                    $presult = pg_exec($cxn, $pquery) or die ("Couldn't execute team lookup query.");
                    $prow=pg_fetch_row($presult);
                    $outcome=$prow[0];
                }
                /*THIS IS WHERE TO PUT THE IF STATEMENT TO SHOW THE RADIO BUTTON OR NOT*/
                echo "<td><input type='radio' name='winners[$counter][winner]' value=1 ";
                    if ($outcome==1) {echo "checked='checked'>";} else {echo ">";} /*default Home Team as Winner for picks; not for Admin updating game results if want default Players picks to Home Team, include this in IF: OR ($outcome==0 AND $_SESSION['logname']<>'admin')*/
                echo "$homeat $team_name</td>\n";
                /*display Opponent team name*/
                if ($locale==2) {$awayat="at";} else {$awayat="";}
                $oquery = "SELECT team_name FROM teams WHERE id=$opponent_id";
                $oresult = pg_exec($cxn, $oquery) or die ("Couldn't execute team lookup query.");
                $orow=pg_fetch_assoc($oresult);
                extract($orow);
                echo "<td><input type='radio' name='winners[$counter][winner]' value=2 ";
                    if ($outcome==2) {echo "checked='checked'>";} else {echo ">";}
                echo "$awayat $team_name</td></tr>\n";
            }
            /*display existing games and values from games table for teams without games (locale=0)*/
            $query = "SELECT * FROM games WHERE week_num=$currentweekid AND locale=0"; /* */
            $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query.");
            while ($row=pg_fetch_assoc($result))  
            {
                extract($row);
                $counter=$counter+1;
                echo "<tr>";
                echo "<td>$counter.&nbsp</td>\n";
                /*display tiebreaker game with asterisk in column*/
                echo "<td></td>\n";
                /*display date for each game*/
                echo "<td></td>\n";
                /*display team name*/
                $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
                $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
                $trow=pg_fetch_assoc($tresult);
                extract($trow);
                echo "<td>$team_name</td>\n";
                /*display opponent team name*/
                echo "<td><span style=\"font-style:italic\">No game this week</span></td></tr>\n";
            }
            $query = "SELECT tiebreakerpts FROM games WHERE week_num=$currentweekid AND tiebreaker=1";
            $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query.");
            if (pg_numrows($result)>0)
            {
                $row=pg_fetch_assoc($result);
                extract($row);
            }
            else {$tiebreakerpts=0;}
            /*override tiebreakerpts default with that from picks table for non-admin users*/
            if (($_SESSION['logid']==$viewingplayer AND $_SESSION['logname']<>'admin') OR ($_SESSION['logid']<>$viewingplayer AND $_SESSION['logname']=='admin'))
                {
                    $query = "SELECT picks.tiebreakerpts FROM picks INNER JOIN games ON picks.game_id=games.id WHERE games.week_num=$currentweekid AND
                    games.tiebreaker=1 AND picks.player_id=$viewingplayer";
                    $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query for tiebreakerpts.");
                    if (pg_numrows($result)>0)
                    {
                        $row=pg_fetch_row($result);
                        $tiebreakerpts=$row[0];
                    }
                    else {$tiebreakerpts=0;}
                }
            echo "<tr><td></td><td></td><td colspan=\"3\">* Tiebreaker Game Points Total:&nbsp";
            echo "<input type='number' name='tiebreakerpts' id='tiebreakerpts' size='5' maxlength='3' value=$tiebreakerpts></td>\n";
            echo "</tr>\n";
        ?>
    </tbody>
</table>
<?php
if ($_SESSION['logname']=='admin' AND $viewingplayer==0)
{
    /*Show buttons for admin user*/
    echo "<p class=\"submit\"><input type=\"submit\" class=\"savebuttons\" name=\"display_button\" value=\"Edit Games\"/>";
    echo "<input type=\"submit\" class=\"savebuttons\" name=\"display_button\" value=\"Save, Update Summary\"/>";
    /*echo "<input type=\"button\" class=\"savebuttons\" name=\"display_button\" value=\"Email Schedule to ALL\" id=\"listbelow\"/>";*/
    echo "<input type=\"button\" class=\"savebuttons\" name=\"display_button\" value=\"Email Players\" id=\"listbelow\"/></p>";
}
else
{
    /*Show buttons for standard user*/
    /*echo "<p class=\"submit\"><input type=\"submit\" class=\"savebuttons\" name=\"display_button\" value=\"Save\"/>";*/
    echo "<p class=\"submit\"><input type=\"submit\" class=\"savebuttons\" name=\"display_button\" value=\"Save and View\"/></p>";
}
?>