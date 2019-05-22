 
	/* Heroku Mailgun settings */
	$api_key = getenv('MAILGUN_API_KEY');
	$domain = getenv('MAILGUN_DOMAIN');
	$api_url = "https://api:#{$api_key}@api.mailgun.net/v3/$mailgun_domain";
?>

<?php
function mg_mailpassword_parameters($id)
{
    require ("misc.inc");
    $query="SELECT email, password FROM player WHERE id='$id'";
    $result=pg_exec($cxn,$query) or die ("Could not execute password lookup query");
    $row=pg_fetch_assoc($result);
    extract($row);

    $parameters = [
        'from'=>'admin@ncaahoopspool.com',
        'to'=>'pmaurer@igojet.com',
        'subject'=>'Test message from NCAAHoopsPool.com',
        'text'=>'Test message body'
    ];

    return $parameters
}
?>