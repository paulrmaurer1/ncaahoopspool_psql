<?php 
	/* Heroku Mailgun settings */
	$api_key = getenv('MAILGUN_API_KEY');
	$domain = getenv('MAILGUN_DOMAIN');
	$api_url = "https://api:#{$api_key}@api.mailgun.net/v3/$mailgun_domain";
?>