<div id="header">
<?php
    /* Show user email address and Logout link in title area */
    echo "<h3>".$_SESSION['logname']."&nbsp&nbsp&nbsp";
    echo "<a class=\"logout\" href=\"login.php?logout=1\">Log out</a>&nbsp&nbsp</h3>";
?>
    <a href="index.php">
        <h1>NCAAHoopsPool</h1>
    </a>
</div>
    
<div id="menu">
    <a href="index.php" class="summary">Overall Summary</a><br />
    <?php
        // set current date
        date_default_timezone_set('America/New_York');
        $currentdate=date("Y-m-d");
        $currentdatetime=date("Y-m-d H:i:s");
        include_once("sharedqueries.inc");

        $currentweek=getCurrentWeek($currentdate);
        
        if ($_SESSION['logname']=='admin') { 
            /*Show title of games section as "Setup Game" to admin user, "My Games" to rest*/
            echo "<a href=\"games.php?week=$currentweek\" class=\"games\">Setup Games</a><br />";
        } else {
            echo "<a href=\"picks.php?week=$currentweek\" class=\"picks\">My Games</a><br />";
        }
            
        // Admin portion of menu
        if ($_SESSION['logname']=='admin') { 
            /*Only show Setup Games menu options to admin user*/
            /*display available weeks in sub-menu and highlight current week with an asterisk*/
            $query = "SELECT * FROM weeks ORDER BY weeks.num ASC";
            $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 2.");
            while ($row=pg_fetch_assoc($result)) {
                extract($row);
                if ($num==$currentweek) {
                    echo "<a class=\"gamessub$num\" href=\"games.php?week=$num\">Week $num*</a><br />";
                } else {
                    echo "<a class=\"gamessub$num\" href=\"games.php?week=$num\">Week $num</a><br />";
                }
            }

            echo "<a href=\"winners.php?week=$currentweek\" class=\"winners\">Update Results</a><br />";
            /*display available weeks in sub-menu and highlight current week with an asterisk*/
            $query = "SELECT * FROM weeks ORDER BY weeks.num ASC";
            $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 2.");
            while ($row=pg_fetch_assoc($result)) {
                extract($row);
                if ($num==$currentweek) {
                    echo "<a class=\"winnerssub$num\" href=\"winners.php?week=$num\">Week $num*</a><br />";
                } else {
                    echo "<a class=\"winnerssub$num\" href=\"winners.php?week=$num\">Week $num</a><br />";
                }
            }
        } // end Admin portion of menu

        // Non Admin portion of menu
        if ($_SESSION['logname']<>'admin') {
            /*Show My Games menu options to non-admin user*/
            /*first determine latest week for which the admin has set any games*/
            $latestweek = getLatestWeekSetup();
            
            /*display available weeks in sub-menu and highlight current week with an asterisk*/
            $query = "SELECT * FROM weeks where num <= $latestweek ORDER BY weeks.num ASC";
            $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 3.");
            while ($row=pg_fetch_assoc($result)) {
                extract($row);
                if ($num==$currentweek) {
                    echo "<a class=\"pickssub$num\" href=\"picks.php?week=$num\">Week $num*</a><br />";
                } else {
                    echo "<a class=\"pickssub$num\" href=\"picks.php?week=$num\">Week $num</a><br />";
                }
            }
        } // end Non Admin portion of menu
    ?>
    <a class="roster" href="roster.php">Roster</a><br/>
    <a href="help.php">Help</a><br/><br/>
</div> <!--end of menu div-->