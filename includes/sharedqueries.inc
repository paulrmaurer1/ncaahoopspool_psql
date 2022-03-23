<!--PHP functions that support queries needed by other pages & functions -->

<?php
	function getCurrentWeek($currentdate)
	{
		/*
		This function returns the current week for highlighting menu entries and supporting defaults
		Used by title_menu.inc
		*/
        require ("misc.inc");

        $currentweek=1;  /*default to Week 1*/
        $query = "SELECT * FROM weeks ORDER BY num ASC"; /*find current week by searching between from & to dates for each week*/
        $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 1.");
        while ($row=pg_fetch_assoc($result)) {
            extract($row);
            if ($currentdate<$from_date) {
              break;  /*to capture dates before season starts*/
            }
            if ($currentdate>=$from_date && $currentdate<=$to_date) {
              $currentweek=$num;
              break;
            }
            $currentweek=$num; /*to capture last week in case past last week of season*/
        }
        return $currentweek;
    }

    function getLatestWeekSetup()
    {
		/*
		This function returns the latest week which the admin user has setup at least one game
		Used by title_menu.inc to only show the weeks to players which are eligible for making picks
		*/
        require ("misc.inc");

        $latestweek = 1; /*default to Week 1*/
        $query = "SELECT weeks.num, games.locale FROM weeks INNER JOIN games ON weeks.num = games.week_num ORDER BY weeks.num ASC";
        $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 2.");
        while ($row=pg_fetch_assoc($result)) {
            extract($row);
            if ($locale > 0) {
                $latestweek = $num;
            }
        }
        return $latestweek;
    }

    function getPlayerSortQuery($orderby, $latestweekwithatleast1completegame)
    {
        /*
        This function returns the query string needed to properly sort Players on the Overall Summary table from index.php
        */
        switch ($orderby) {
            case 0: /*sort by overall record*/
                return "SELECT * FROM player WHERE email<>'admin' ORDER BY correct_picks DESC";
            case 1: /*sort by current week's record*/
                return "
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
        }
    }

    function allPicksMade($currentweekid)
    {
        /*
        This function returns a true/false based on whether all picks have been made for
        a given week
        */
        require ("misc.inc");

        $allpicksmade = true;

        $query = "
            SELECT 
                g1.id
                ,sum(case when p1.pick = 0 then 1 else 0 end) as missingpicks

            FROM 
                games as g1
                join picks as p1
                    on g1.id = p1.game_id
            WHERE 
                g1.week_num=$currentweekid
                AND g1.locale <> 0
                
            GROUP BY 1
        ";
        $result = pg_exec($cxn, $query) or die ("Couldn't execute missingpicks query.");
        while ($row=pg_fetch_assoc($result)) 
        {
            extract($row);
            if ($missingpicks > 0) {
                $allpicksmade = false;
            }
        }
        return $allpicksmade;
    }

    function allPicksMadeByPlayer($currentweekid, $playerid)
    {
        /*
        This function returns a true/false based on whether all picks have been made for
        a given week by a given player
        */
        require ("misc.inc");

        $allpicksmade = true;

        $query = "
            SELECT 
                g1.id
                ,sum(case when p1.pick = 0 then 1 else 0 end) as missingpicks

            FROM 
                games as g1
                join picks as p1
                    on g1.id = p1.game_id
            WHERE 
                g1.week_num=$currentweekid
                AND g1.locale <> 0
                AND p1.player_id=$playerid
                
            GROUP BY 1
        ";
        $result = pg_exec($cxn, $query) or die ("Couldn't execute missingpicks query.");
        while ($row=pg_fetch_assoc($result)) 
        {
            extract($row);
            if ($missingpicks > 0) {
                $allpicksmade = false;
            }
        }
        return $allpicksmade;
    }
?>