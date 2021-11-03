<?php
	/* Heroku Mailgun settings */
	$api_key = getenv('MAILGUN_API_KEY');
	$domain = getenv('MAILGUN_DOMAIN');
	$api_url = "https://api:#{$api_key}@api.mailgun.net/v3/$domain";

	/* Return email components needed for Forget Password email message */
	function mgMailPasswordParameters($id)
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

	/* Return email components needed for Send Reminder email message */
	function mgMailReminderParameters($weeknum, $all, $emailmessage, $emailsubject)
	{
	    require ("misc.inc");
	    $to="";
	    if ($all==0) {
	        /*only send to players who haven't submitted results yet*/
	        $query="SELECT id, email FROM player WHERE email<>'admin'";
	        $result=pg_exec($cxn,$query) or die ("Could not execute players query");
	        while ($row=pg_fetch_assoc($result)) {
	            $isplayerincomplete=0;
	            extract($row);
	            $picksquery="SELECT picks.pick FROM picks INNER JOIN games ON picks.game_id = games.id WHERE games.week_num='$weeknum' AND picks.player_id ='$id'";
	            $picksresult=pg_exec($cxn,$picksquery) or die ("Could not execute picks query");
	            while ($picksrow=pg_fetch_assoc($picksresult)) {
	                extract($picksrow);
	                if ($pick==0) {
	                        $isplayerincomplete=1;
	                    }
	            }
	            if ($isplayerincomplete==1) {
	                    $to .= "$email".", ";
	                }
	        }
	    } else {
	    	/*send to all players*/
	        $query = "SELECT email FROM player WHERE email<>'admin'";
	        $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query for mail.");
	        while ($row=pg_fetch_assoc($result)) {
	            extract($row);
	            $to .= "$email".", ";
	        }
	    }

	    $parameters = [
	        'from'=>'admin@ncaahoopspool.com',
	        'to'=>$to,
	        'subject'=>$emailsubject,
	        'text'=>$emailmessage,
	        'html'=>'<p>'.$emailmessage.'</p>'
	    ];

	    return $parameters;
    }
?>