<!--PHP program that registers a user -->

<?php
	function registerUser($register_email, $register_password)
	{  
        require ("misc.inc");

        // set current date/time
        date_default_timezone_set('America/New_York');
        $currentdatetime=date("Y-m-d H:i:s");

        // set session variables upon login
        session_start();
        $_SESSION['auth']="yes";
        $_SESSION['logname']=$register_email;
        $query = "SELECT id FROM player WHERE email='".$register_email."'";
        $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query.");
        $row=pg_fetch_row($result);
        $_SESSION['logid']=$row[0];

        // insert new password for registered player
        $query="UPDATE player SET password='$register_password', lastlogindate='$currentdatetime' WHERE email='$register_email'"; 
        $result=pg_exec($cxn,$query) or die ("Could not execute picks table insert query");

        // redirect user to main page
        header("Location: index.php");
	}
?>