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

                /* Build player lists for hover tooltips */
                $teamPickers = [];
                $oppPickers = [];
                if ($showfieldrecords) {
                    $teamPickersQuery = "
                        SELECT 
                            player.firstname || ' ' || player.lastname AS fullname
                        FROM picks
                        INNER JOIN player ON player.id = picks.player_id
                        WHERE picks.game_id=$id AND picks.pick = 1
                        ORDER BY player.lastname, player.firstname
                    ";
                    $teamPickersResult = pg_exec($cxn, $teamPickersQuery) or die ("Couldn't execute team pickers query.");
                    while ($pickerRow = pg_fetch_assoc($teamPickersResult)) {
                        $teamPickers[] = $pickerRow['fullname'];
                    }

                    $oppPickersQuery = "
                        SELECT 
                            player.firstname || ' ' || player.lastname AS fullname
                        FROM picks
                        INNER JOIN player ON player.id = picks.player_id
                        WHERE picks.game_id=$id AND picks.pick = 2
                        ORDER BY player.lastname, player.firstname
                    ";
                    $oppPickersResult = pg_exec($cxn, $oppPickersQuery) or die ("Couldn't execute opponent pickers query.");
                    while ($pickerRow = pg_fetch_assoc($oppPickersResult)) {
                        $oppPickers[] = $pickerRow['fullname'];
                    }
                }
                $teamPickersData = htmlspecialchars(json_encode($teamPickers), ENT_QUOTES);
                $oppPickersData = htmlspecialchars(json_encode($oppPickers), ENT_QUOTES);
                
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
                
                $teamClasses = ['player-pick-cell'];
                $oppClasses = ['player-pick-cell'];
                $teamPrefix = '';
                $oppPrefix = '';

                if ($outcome==0 AND $pick==1) /*no outcome posted yet, display home team pick in bold*/
                {
                    $teamClasses[] = "picked";
                    $teamPrefix = "x ";
                }
                if ($outcome==0 AND $pick==2) /*no outcome posted yet, display opponent pick in bold*/
                {
                    $oppClasses[] = "picked";
                    $oppPrefix = "x ";
                }
                if ($outcome==1 AND $pick==$outcome)
                {
                    $teamClasses[] = "won";
                    $teamPrefix = "x ";
                }
                if ($outcome==2 AND $pick==$outcome)
                {
                    $oppClasses[] = "won";
                    $oppPrefix = "x ";
                }
                if ($outcome==1 AND $pick<>$outcome)
                {
                    $teamClasses[] = "missed";
                    $oppClasses[] = "wrong";
                    $oppPrefix = "x ";
                }
                if ($outcome==2 AND $pick<>$outcome)
                {
                    $teamClasses[] = "wrong";
                    $teamPrefix = "x ";
                    $oppClasses[] = "missed";
                }

                if ($showfieldrecords) {
                    $teamClasses[] = 'player-pick-cell-enabled';
                    $oppClasses[] = 'player-pick-cell-enabled';
                }

                $teamNameAttr = htmlspecialchars(trim(($homeat ? $homeat.' ' : '').$playerteam), ENT_QUOTES);
                $oppNameAttr = htmlspecialchars(trim(($awayat ? $awayat.' ' : '').$opponentteam), ENT_QUOTES);
                $pickersEnabledAttr = $showfieldrecords ? "1" : "0";

                $teamClassAttr = implode(' ', array_unique($teamClasses));
                $oppClassAttr = implode(' ', array_unique($oppClasses));

                /*display Team and Opponent with proper styling*/
                echo "<td class=\"$teamClassAttr\" data-pickers=\"$teamPickersData\" data-team=\"$teamNameAttr\" data-pickers-enabled=\"$pickersEnabledAttr\">".$teamPrefix.($homeat ? $homeat." " : "").$playerteam;
                if ($showfieldrecords) {
                        echo "&nbsp($teampicks)";
                    } 
                echo "</td>\n";
                echo "<td class=\"$oppClassAttr\" data-pickers=\"$oppPickersData\" data-team=\"$oppNameAttr\" data-pickers-enabled=\"$pickersEnabledAttr\">".$oppPrefix.($awayat ? $awayat." " : "").$opponentteam;
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

    static $tooltipassetsinjected = false;
    if (!$tooltipassetsinjected) {
        $tooltipassetsinjected = true;
        echo "<style>.player-pick-cell-enabled{cursor:pointer;}</style>";
        echo "<div id=\"pickers-tooltip\" style=\"display:none; position:absolute; z-index:9999; background:#222; color:#fff; padding:6px 10px; border-radius:4px; font-size:0.85em; line-height:1.4; min-width:220px; max-width:280px; box-shadow:0 2px 8px rgba(0,0,0,0.3);\"></div>";
        echo "<script>
            jQuery(function(\$){
                var escapeHtml = function(value){
                    return String(value === undefined || value === null ? '' : value)
                        .replace(/&/g, '&amp;')
                        .replace(/</g, '&lt;')
                        .replace(/>/g, '&gt;')
                        .replace(/\"/g, '&quot;')
                        .replace(/'/g, '&#39;');
                };
                var \$tooltip = \$('#pickers-tooltip');
                var setTooltipPosition = function(event){
                    var offset = 8;
                    var tooltipWidth = \$tooltip.outerWidth();
                    var tooltipHeight = \$tooltip.outerHeight();
                    var scrollLeft = \$(window).scrollLeft();
                    var scrollTop = \$(window).scrollTop();
                    var viewportWidth = \$(window).width();
                    var viewportHeight = \$(window).height();
                    var clientX = event.clientX;
                    var clientY = event.clientY;

                    var left = clientX > viewportWidth / 2
                        ? event.pageX - tooltipWidth - offset
                        : event.pageX + offset;
                    var top = clientY > viewportHeight / 2
                        ? event.pageY - tooltipHeight - offset
                        : event.pageY + offset;

                    if (left + tooltipWidth > scrollLeft + viewportWidth - offset) {
                        left = scrollLeft + viewportWidth - tooltipWidth - offset;
                    }
                    if (left < scrollLeft + offset) {
                        left = scrollLeft + offset;
                    }

                    if (top + tooltipHeight > scrollTop + viewportHeight - offset) {
                        top = scrollTop + viewportHeight - tooltipHeight - offset;
                    }
                    if (top < scrollTop + offset) {
                        top = scrollTop + offset;
                    }

                    \$tooltip.css({ left: left, top: top });
                };
                \$('.player-pick-cell').on('mouseenter', function(e){
                    var enabled = \$(this).data('pickers-enabled');
                    if (!enabled) {
                        \$tooltip.hide();
                        return;
                    }
                    var pickers = \$(this).data('pickers');
                    if (typeof pickers === 'string') {
                        try {
                            pickers = JSON.parse(pickers);
                        } catch (err) {
                            pickers = pickers ? [pickers] : [];
                        }
                    }
                    if (!Array.isArray(pickers)) {
                        pickers = [];
                    }
                    var team = \$(this).data('team');
                    if (!pickers.length) {
                        pickers = ['No picks submitted'];
                    }
                    if (!team) {
                        team = 'Team';
                    }
                    var hasActualPicks = !(pickers.length === 1 && pickers[0] === 'No picks submitted');
                    var listHtml;
                    if (hasActualPicks) {
                        listHtml = '<ul style=\"margin:4px 0 0; padding-left:18px; text-align:left;\">';
                        \$.each(pickers, function(_, name){
                            listHtml += '<li>' + escapeHtml(name) + '</li>';
                        });
                        listHtml += '</ul>';
                    } else {
                        listHtml = '<div style=\"margin:4px 0 0; text-align:left;\">No picks submitted</div>';
                    }
                    var content = '<strong>' + escapeHtml(team) + '</strong>' + listHtml;
                    \$tooltip.stop(true, true).html(content).css({ display: 'block', opacity: 0 });
                    setTooltipPosition(e);
                    \$tooltip.animate({ opacity: 1 }, 120);
                }).on('mousemove', function(e){
                    setTooltipPosition(e);
                }).on('mouseleave', function(){
                    \$tooltip.hide();
                });
            });
        </script>";
    }
}
?>    