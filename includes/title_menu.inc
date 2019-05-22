<div id="header">
    <a href="index.php"><h1></h1></a>
<?php
    /* Show user email address and Logout link in title area */
    echo "<h3>".$_SESSION['logname']."&nbsp&nbsp&nbsp";
    echo "<a class=\"logout\" href=\"login.php?logout=1\">Log out</a>&nbsp&nbsp</h3>";
?>

</div>
    
<div id="menu">
    <a href="index.php" class="summary">Overall Summary</a><br />
<?php
    /*determine current week so when click on Manage Games, defaults to current week*/
    date_default_timezone_set('America/New_York');
    $currentdate=date("Y-m-d");
    $currentdatetime=date("Y-m-d H:i:s");
    $currentweek=1;  /*default to display Week 1*/
    $query = "SELECT * FROM weeks ORDER BY num ASC"; /*find current week by searching between from & to dates for each week*/
    $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 1.");
    while ($row=pg_fetch_assoc($result))
    {
            extract($row);
            if ($currentdate>=$from_date && $currentdate<=$to_date)
            {
                $currentweek=$num;
            }
    }
    
if ($_SESSION['logname']=='admin') /*Show title of games section as "Setup Game" to admin user, "My Games" to rest*/
{
    echo "<a href=\"games.php?week=$currentweek\" class=\"games\">Setup Games</a><br />";
}
else
{
    echo "<a href=\"picks.php?week=$currentweek\" class=\"picks\">My Games</a><br />";
}
    
        
if ($_SESSION['logname']=='admin') /*Only show Setup Games menu options to admin user*/
{
    /*display available weeks in sub-menu and highlight current week with an asterisk*/
    $query = "SELECT * FROM weeks ORDER BY weeks.num ASC";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 2.");
    while ($row=pg_fetch_assoc($result))
        {
                extract($row);
                if ($num==$currentweek)
                {
                    echo "<a class=\"gamessub$num\" href=\"games.php?week=$num\">Week $num*</a><br />";
                }
                else
                {
                    echo "<a class=\"gamessub$num\" href=\"games.php?week=$num\">Week $num</a><br />";
                }
        }
    echo "<a href=\"winners.php?week=$currentweek\" class=\"winners\">Update Results</a><br />";
    /*display available weeks in sub-menu and highlight current week with an asterisk*/
    $query = "SELECT * FROM weeks ORDER BY weeks.num ASC";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 2.");
    while ($row=pg_fetch_assoc($result))
        {
                extract($row);
                if ($num==$currentweek)
                {
                    echo "<a class=\"winnerssub$num\" href=\"winners.php?week=$num\">Week $num*</a><br />";
                }
                else
                {
                    echo "<a class=\"winnerssub$num\" href=\"winners.php?week=$num\">Week $num</a><br />";
                }
        }
}
if ($_SESSION['logname']<>'admin') /*Show My Games menu options to non-admin user*/
{
    /*display available weeks in sub-menu and highlight current week with an asterisk*/
    /*first determine latest week for which the admin has set any games*/
    $latestweek = 1;
    $query = "SELECT weeks.num, games.locale FROM weeks INNER JOIN games ON weeks.num = games.week_num ORDER BY weeks.num ASC";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 2.");
    while ($row=pg_fetch_assoc($result))
        {
                extract($row);
                if ($locale > 0)
                {
                    $latestweek = $num;
                }
        }
    
    $query = "SELECT * FROM weeks where num <= $latestweek ORDER BY weeks.num ASC";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute week query pass 3.");
    while ($row=pg_fetch_assoc($result))
        {
                extract($row);
                if ($num==$currentweek)
                {
                    echo "<a class=\"pickssub$num\" href=\"picks.php?week=$num&player=".$_SESSION['logid']."\">Week $num*</a><br />";
                }
                else
                {
                    echo "<a class=\"pickssub$num\" href=\"picks.php?week=$num&player=".$_SESSION['logid']."\">Week $num</a><br />";
                }
        }
}
?>
<a class="roster" href="roster.php">Roster</a><br/>
<a href="help.php">Help</a><br/><br/>
</div> <!--end of menu div-->