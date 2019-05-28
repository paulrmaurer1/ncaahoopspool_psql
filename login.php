<!doctype html>
<html lang="en">
<?php
    if (! isset($_SESSION['logname']))
    {
        // start session in order to record login or register session variables
        session_start();
    }
    if($_GET) {
        if ($_GET['logout']=1) { 
            /*end session if user clicked on Log Out link from other pages before displaying Login form*/
            session_destroy();
        }
    }

    include_once("includes/head.inc");
    include_once("includes/misc.inc");
    include_once("includes/checklogininfo.inc");
    include_once("includes/checkreginfo.inc");
    include_once("includes/loginuser.inc");
    include_once("includes/registeruser.inc");
?>

<body>
    <div id="wrapper">
        <?php include_once("includes/title.inc"); ?>
        <div id="body-content">
            <h2>Please Login or Register your Account</h2>
            <?php
                if ($_POST) {
                    if ($_POST['logreg_button'] == "Login") { 
                        /*Process to run when user clicks on Login button*/
                        if (checkLoginInfo($_POST)) {
                            /*if login and password match player table, log user in*/
                            loginUser($_POST['loginemail']);
                        }
                     } elseif ($_POST['logreg_button'] == "Register") {
                        /*Process to run when user clicks on Register button*/
                        if (checkRegInfo($_POST)) {
                            /*if email matches a Player record and no password yet, register and log user in*/
                            registerUser($_POST['registeremail'], $_POST['registerpassword']);
                        }
                     }
                }
            ?>
            <form action="login.php" method='POST'>
            	<?php
                	include_once("includes/displayloginregform.inc"); /*display login & registration form*/
            	?>
            </form>
        </div> <!-- end of body-content div -->
        <?php include_once("includes/footer.inc"); ?>
    </div> <!-- end of wrapper div -->
</body>
</html>