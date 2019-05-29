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
            if ($currentdate>=$from_date && $currentdate<=$to_date) {
                $currentweek=$num;
            }
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