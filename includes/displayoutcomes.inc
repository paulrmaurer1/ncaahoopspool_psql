<!--PHP function that displays a players pick results based on game outcomes -->

<?php
function displayoutcomes($weeknum, $playerid, $showfieldrecords)
{   require ("misc.inc");
    
    /*display first and last name of player whose picks are being shown*/
    $query = "SELECT firstname, lastname FROM player WHERE id=$playerid"; 
    $result = pg_exec($cxn, $query) or die ("Couldn't execute firstname, lastname lookup from player table.");
    $row=pg_fetch_assoc($result);
    extract($row);
    echo "<h3>Player: $firstname $lastname</h3>";
    
    /*display table header*/
    echo "<table class=\"picks\"\n>";
    /*echo "<caption class=\"greenbold\">x [Teamname] = correct pick</caption>";*/
    /*echo "<caption class=\"redstrike\">x [Teamname] = wrong pick</caption>";*/
    echo "<thead>\n";
        echo "<tr>\n";
            echo "<th></th>\n";
            echo "<th class=\"tiebreaker\">TB</th>\n";
            echo "<th class=\"date\">Date</th>\n";
            echo "<th class=\"team\">Team";
            if ($showfieldrecords) {
                    echo "&nbsp(Field picks)";
                }
            echo "</th>\n";
            echo "<th class=\"opponent\">Opponent";
            if ($showfieldrecords) {
                    echo "&nbsp(Field picks)";
                }
            echo "</th>\n";
        echo "</tr>\n";
    echo "</thead>\n";
    /*display table body*/
    echo "<tbody>\n";
        $counter=0;
        /*display scheduled games and values from games table*/ 
        $query = "SELECT * FROM games WHERE week_num=$weeknum AND locale<>0 ORDER BY gamedate"; /* */
        $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query.");
        while ($row=pg_fetch_assoc($result)) 
        {
            extract($row);
            $counter=$counter+1;
            echo "<tr>";
                /*display game number*/
                echo "<td>$counter.&nbsp</td>\n";
                
                /*display tiebreaker game with asterisk in column*/
                echo "<td>";
                if ($tiebreaker==1)
                {
                    echo"<strong>*</strong>";
                }
                echo "</td>";
                
                /*display date for each game*/
                echo "<td>".date('F jS Y',strtotime($gamedate))."</td>\n";

                /* Determine field pick records*/
                $fquery = "
                    SELECT 
                        sum(case when p1.pick = 1 then 1 else 0 end) as teampicks
                        ,sum(case when p1.pick = 2 then 1 else 0 end) as oppteampicks

                    FROM 
                        picks as p1
                    WHERE 
                        game_id=$id
                ";
                $fresult = pg_exec($cxn, $fquery) or die ("Couldn't execute field pick record query.");
                $frow=pg_fetch_assoc($fresult);
                extract($frow);
                
                /*determine Team name*/
                if ($locale==1) {$homeat="at";} else {$homeat="";}
                $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
                $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
                $trow=pg_fetch_assoc($tresult);
                extract($trow);
                $playerteam=$team_name;
                
                /*determine Opponent team name*/
                if ($locale==2) {$awayat="at";} else {$awayat="";}
                $oquery = "SELECT team_name FROM teams WHERE id=$opponent_id";
                $oresult = pg_exec($cxn, $oquery) or die ("Couldn't execute team lookup query.");
                $orow=pg_fetch_assoc($oresult);
                extract($orow);
                $opponentteam=$team_name;

                /*determine which css class to use depending on whether result has been selected and which team player picked*/
                $pquery = "SELECT pick FROM picks WHERE game_id=$id AND player_id=$playerid"; /*determine which team the player picked for highlighting pick with "x"*/
                $presult = pg_exec($cxn, $pquery) or die ("Couldn't execute team lookup query.");
                $prow=pg_fetch_assoc($presult);
                extract($prow);
                
                $teamdisplay=">";
                $oppdisplay=">";
                if ($outcome==0 AND $pick==1) /*no outcome posted yet, display home team pick in bold*/
                {
                    $teamdisplay="class=\"picked\">x ";
                }
                if ($outcome==0 AND $pick==2) /*no outcome posted yet, display opponent pick in bold*/
                {
                    $oppdisplay="class=\"picked\">x ";
                }
                if ($outcome==1 AND $pick==$outcome)
                {
                    $teamdisplay="class=\"won\">x ";
                }
                if ($outcome==2 AND $pick==$outcome)
                {
                    $oppdisplay="class=\"won\">x ";
                }
                if ($outcome==1 AND $pick<>$outcome)
                {
                    $teamdisplay="class=\"missed\">";
                    $oppdisplay="class=\"wrong\">x ";
                }
                if ($outcome==2 AND $pick<>$outcome)
                {
                    $teamdisplay="class=\"wrong\">x ";
                    $oppdisplay="class=\"missed\">";
                }
                /*display Team and Opponent with proper styling*/
                echo "<td ".$teamdisplay."$homeat $playerteam";
                if ($showfieldrecords) {
                        echo "&nbsp($teampicks)";
                    } 
                echo "</td>\n";
                echo "<td ".$oppdisplay."$awayat $opponentteam";
                 if ($showfieldrecords) {
                        echo "&nbsp($oppteampicks)";
                    }
                echo "</td>\n";
        }
        /*display games and values from games table for teams without games (locale=0)*/
        $query = "SELECT * FROM games WHERE week_num=$weeknum AND locale=0"; /* */
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
            /*display that there is no game this week for this Team*/
            echo "<td><span style=\"font-style:italic\">No game this week</span></td></tr>\n";
        }

        $query = "SELECT picks.tiebreakerpts, games.tiebreakerpts FROM picks INNER JOIN games ON picks.game_id=games.id WHERE games.week_num=$weeknum AND games.tiebreaker=1 AND picks.player_id=$playerid";
            $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query for tiebreakerpts.");
            if (pg_num_rows($result)>0) {
                $row=pg_fetch_row($result);
                $tiebreakerpts=$row[0];
                $actualtiebreakerpts=$row[1];
            } else {
                $tiebreakerpts=0;
                $actualtiebreakerpts=0;
            }
            $gamediff = $tiebreakerpts-$actualtiebreakerpts;

            echo "<tr><td></td><td></td><td colspan=\"3\"><b>* Tiebreaker Game Points Total:</b> $tiebreakerpts";
            if ($actualtiebreakerpts>0) {
                echo "($gamediff)&nbsp&nbsp&nbsp<b>Actual Total:</b> $actualtiebreakerpts";
            }
            echo "</td>\n</tr>\n";
    echo "</tbody>";
    echo "</table>";
    echo "<div id=\"tablecaption\">";
    echo    "<h4 class=\"correct\">x [Teamname] = correct pick&nbsp&nbsp</h3>";
    echo    "<h4 class=\"wrong\">x [Teamname] = wrong pick</h3>";
    echo "</div>";
}
?>    