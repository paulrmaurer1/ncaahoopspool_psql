<?php 
	/* Code for using Mailgun-PHP library ***not being used***/
	// require '../vendor/autoload.php';
	// use MailgunMailgun;
	// $api_key = getenv('MAILGUN_API_KEY');
	// $mg = new Mailgun($api_key);
	// $domain = getenv('MAILGUN_DOMAIN');

	/* Heroku settings */
	$api_key = getenv('MAILGUN_API_KEY');
	$mailgun_domain = getenv('MAILGUN_DOMAIN');
	$api_url = "https://api:#{$api_key}@api.mailgun.net/v3/$mailgun_domain";
?>