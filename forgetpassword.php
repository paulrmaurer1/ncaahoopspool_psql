<!doctype html>
<html lang="en">
<?php
	if (! isset($_SESSION['logname']))
	{
	    session_start();
	}

	/* Load Composer dependencies */
	require 'vendor/autoload.php';
	
	/* Load Mailgun Class */
	use Mailgun\Mailgun;
	
	/* Load mailgun environment variables and parameter functions */
	include_once("includes/mailgun.inc");
	
	include_once("includes/head.inc");
	include_once("includes/misc.inc");
	include_once("includes/checklogininfo.inc");
?>

<body>
    <div id="wrapper">
        <?php include_once("includes/title.inc"); ?>
        <div id="body-content">
			<h3>Enter your email address</h3>
			<?php
				if($_POST) {
					if (checkLoginInfo($_POST)) {
						/*if login matches player table, send email*/
					    $query="SELECT id FROM player WHERE email='".$_POST['loginemail']."'";
					    $result=pg_exec($cxn,$query) or die ("Could not execute id lookup query");
					    $row=pg_fetch_assoc($result);
					    extract($row);
						
						/* Send email with Mailgun */
						$mg = Mailgun::create($api_key);
						$parameters = mgMailPasswordParameters($id);
						$mg->messages()->send($domain, $parameters);

					    /* Return user to Login Page */
					    header("Location: login.php");
					} else {
						/* Show error messages created within checkLoginInfo() */
						echo $message;
				        }
				}
			?>
			<form action="forgetpassword.php" method='POST'>
				<label for="loginemail">Email:</label>
				<input type="text" name="loginemail" id="loginemail" size="25" maxlength="50" 
					<?php 
						if ($_POST) {
							echo 'value="'.$_POST['loginemail'].'"';
						} elseif ($_GET) {
							echo 'value="'.$_GET['loginemail'].'"';
							}
					?>
				/> <!-- end input -->
				<br/>
				<p class="submit"><input type="submit" name="forget_button" value="Send Password"/></p><br/>
			</form>
        </div> <!-- end of body-content div -->
        <?php include_once("includes/footer.inc"); ?>
    </div> <!-- end of wrapper div -->
</body>
</html>