<?php
	/* Heroku Mailgun settings */
	$api_key = getenv('MAILGUN_API_KEY');
	$domain = getenv('MAILGUN_DOMAIN');
	$api_url = "https://api:#{$api_key}@api.mailgun.net/v3/$mailgun_domain";

	/* Return email components needed for Forget Password email message */
	function mg_mailpassword_parameters($id)
	{
	    require ("misc.inc");
	    $query="SELECT email, password FROM player WHERE id='$id'";
	    $result=pg_exec($cxn,$query) or die ("Could not execute password lookup query");
	    $row=pg_fetch_assoc($result);
	    extract($row);

	    $parameters = [
	        'from'=>'admin@ncaahoopspool.com',
	        'to'=>$email,
	        'subject'=>'Your password for NCAAHoopsPool.com',
	        'text'=>'Here is your password: '.$password,
	        'html'=>'Here is your password: <b>'.$password.'</b>'
	    ];

	    return $parameters;
	}
?>