<!--PHP program that logs in a user -->
<?php
	function loginUser($login_email)
	{  
        require ("misc.inc");

        // set current date/time
        date_default_timezone_set('America/New_York');
        $currentdatetime=date("Y-m-d H:i:s");

        // set session variables upon login
        session_start();
        $_SESSION['auth']="yes";
        $_SESSION['logname']=$login_email;
        $query = "SELECT id FROM player WHERE email='".$login_email."'";
        $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query.");
        $row=pg_fetch_row($result);
        $_SESSION['logid']=$row[0];

        // update player lastlogindate and add to player_login table
        $matchingid = $row[0];
        $query="UPDATE player SET lastlogindate='$currentdatetime' WHERE id='$matchingid'";
        $result=pg_exec($cxn,$query) or die ("Could not execute picks lastlogindate update query");
        $query="INSERT INTO player_login (player_id,login_date) VALUES ('$matchingid','$currentdatetime')";
        $result=pg_exec($cxn,$query) or die ("Could not execute player_login table insert query");

        // redirect user to main page
        header("Location: index.php");
	}
?>