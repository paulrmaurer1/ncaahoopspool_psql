<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
	// ob_start();
	if (! isset($_SESSION['logname']))
	{
	    session_start();
	}

	/* Load Composer dependencies */
	require_once ("vendor/autoload.php");

	/* Load Mailgun Class */
	use Mailgun\Mailgun;

	include_once("includes/head.inc");
	include_once("includes/misc.inc");
	include_once("includes/mailgun.inc");
	include_once("includes/mailfunctions.inc");
	include_once("includes/checklogininfo.inc");
?>

<body>
<?php
echo "<div id=\"wrapper\">\n";

include_once("includes/title.inc");
?>
<div id="body-content">
<h3>Enter your email address</h3>
<?php
if($_POST)
{
    if (checklogininfo($_POST))/*if login matches player table, send email*/
	{
	    $query="SELECT id FROM player WHERE email='".$_POST['loginemail']."'";
	    $result=pg_exec($cxn,$query) or die ("Could not execute id lookup query");
	    $row=pg_fetch_assoc($result);
	    extract($row);
	    /* Guzzle code */
        // $client = new Client(['base_uri' => $api_url]);
		// $request = new Request('POST', $api_url.'/messages');
	    // mailpassword($id, $client, $request);
		
		/* Mailgun code */
		$mg = new Mailgun($api_key);

		$mg->sendMessage($domain, array(
        'from'=>'admin@ncaahoopspool.com',
        'to'=>'pmaurer@igojet.com',
        'subject'=>'Test message from NCAAHoopsPool.com',
        'text'=>'Test message body'
        )
    );
		echo $response;
	    // mailpassword($id, $mg, $mailgun_domain);

	    header("Location: login.php");
	}
        else
        {
            echo $message;
        }
}
?>
<form action="forgetpassword.php" method='POST'>
	<label for="loginemail">Email:</label>
	<input type="text" name="loginemail" id="loginemail" size="25" maxlength="50" <?php if ($_POST) {echo 'value="'.$_POST['loginemail'].'"';}
	elseif ($_GET) {echo 'value="'.$_GET['loginemail'].'"';}?>/><br/>
	<p class="submit"><input type="submit" name="forget_button" value="Send Password"/></p><br/>
</form>

<?php
echo "</div>"; /*end of body-content div*/
include_once("includes/footer.inc");
echo "</div>";  /*end of wrapper div*/
echo "</body>";
echo "</html>";
?>