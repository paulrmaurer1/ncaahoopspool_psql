<?php ob_start()?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<?php
if (! isset($_SESSION['logname']))
{
    session_start();
}
?>
<html xmlns="http://www.w3.org/1999/xhtml">

<?php
include_once("includes/head.inc");
include_once("includes/misc.inc");
include_once("includes/checklogininfo.inc");
include_once("includes/checkreginfo.inc");
?>

<body>
<?php
if($_GET)
{
    if ($_GET['logout']=1) /*end session if user clicked on Log Out link from other pages before displaying Login form*/
    {
        session_destroy();
    }
}
echo "<div id=\"wrapper\">\n";

include_once("includes/title.inc");
?>
<div id="body-content">
<h2>Please Login or Register your Account</h2>
<?php
if($_POST)
{
	date_default_timezone_set('America/New_York');
        $currentdatetime=date("Y-m-d H:i:s");
        if($_POST['logreg_button'] == "Login") /*Process to run when user clicks on Login button*/
        {
            if (checklogininfo($_POST))/*if login and password match player table, log user in*/
            {
                $_SESSION['auth']="yes";
                $_SESSION['logname']=$_POST['loginemail'];
                $query = "SELECT id FROM player WHERE email='".$_POST['loginemail']."'";
                $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query.");
                $row=pg_fetch_row($result);
                $_SESSION['logid']=$row[0];
                $matchingid = $row[0];
                $query="UPDATE player SET lastlogindate='$currentdatetime' WHERE id='$matchingid'"; /*update lastlogindate for player*/
                $result=pg_exec($cxn,$query) or die ("Could not execute picks lastlogindate update query");
                $query="INSERT INTO player_login (player_id,login_date) VALUES ('$matchingid','$currentdatetime')"; /*add to player_login table each time a player logs in*/
                $result=pg_exec($cxn,$query) or die ("Could not execute player_login table insert query");
                header("Location: index.php");
            }
         }
        elseif ($_POST['logreg_button'] == "Register") /*Process to run when user clicks on Register button*/
        {
            if (checkreginfo($_POST)) /*if email matches a Player record and no password yet, register and log user in*/
            {
                $_SESSION['auth']="yes";
                $_SESSION['logname']=$_POST['registeremail'];
                $query = "SELECT id FROM player WHERE email='".$_POST['registeremail']."'";
                $result = pg_exec($cxn, $query) or die ("Couldn't execute team lookup query.");
                $row=pg_fetch_row($result);
                $_SESSION['logid']=$row[0];
                $newpassword=$_POST['registerpassword'];
                $matchingemail=$_POST['registeremail'];
                $query="UPDATE player SET password='$newpassword', lastlogindate='$currentdatetime' WHERE email='$matchingemail'"; /*insert new password for registered player*/
                $result=pg_exec($cxn,$query) or die ("Could not execute picks table insert query");
                header("Location: index.php");
            }
         }
}
?>
<form action="login.php" method='POST'>
	<?php
	include_once("includes/displayloginregform.inc"); /*display login & registration form*/
	?>
</form>

<?php
echo "</div>"; /*end of body-content div*/
include_once("includes/footer.inc");
echo "</div>";  /*end of wrapper div*/
echo "</body>";
echo "</html>";
?>