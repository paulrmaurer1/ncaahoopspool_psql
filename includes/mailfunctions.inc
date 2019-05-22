<!--PHP function that mails various messages to players -->

<?php
// function mailpassword($id, $client, $request)
function mailpassword($id, $mg, $domain)
{
    require ("misc.inc");
    $query="SELECT email, password FROM player WHERE id='$id'";
    $result=pg_exec($cxn,$query) or die ("Could not execute password lookup query");
    $row=pg_fetch_assoc($result);
    extract($row);

    /* Code for sending email via basic mail() functionality ***not being used*** */
    // $to = $email;
    // $subject = "Your password for NCAAHoopsPool.com";
    // $message = "Here is your password: ".$password;

    // /*build additional headers*/
    // $headers .= 'From: Do-not-reply <admin@ncaahoopspool.com>';

    // /*mail the message*/
    // mail($to, $subject, $message, $headers);

    /* Code for sending email via mailgun-php library ***not being used*** */
    $mg->sendMessage($domain, array(
        'from'=>'admin@ncaahoopspool.com',
        'to'=>$email,
        'subject'=>'Your password for NCAAHoopsPool.com',
        'text'=>'Here is your password: '.$password,
        )
    );

    /* Code for sending email via guzzle and mailgun HTTP service */

    // $response = $client->send($request, [
    //     'from' => 'admin@ncaahoopspool.com',
    //     'to' => $email,
    //     'subject' => 'Your password for NCAAHoopsPool.com',
    //     'text' => 'Here is your password: '.$password,
    //     'html' => '<b>HTML</b> version of the body!'
    // ]);

}
?>

<?php
function mailschedule($weeknum, $all, $emailmessage, $emailsubject)
{require ("misc.inc");
    
    /*build $to variable with email addresses sent with function or else everybody on roster, except for Admin*/
    if ($all==0) /*only send to players who haven't submitted results yet*/
    {
        $query="SELECT id, email FROM player WHERE email<>'admin'";
        $result=pg_exec($cxn,$query) or die ("Could not execute players query");
        while ($row=pg_fetch_assoc($result))
        {
            $isplayerincomplete=0;
            extract($row);
            $picksquery="SELECT picks.pick FROM picks INNER JOIN games ON picks.game_id = games.id WHERE games.week_num='$weeknum' AND picks.player_id ='$id'";
            $picksresult=pg_exec($cxn,$picksquery) or die ("Could not execute picks query");
            while ($picksrow=pg_fetch_assoc($picksresult))
            {
                extract($picksrow);
                if ($pick==0)
                    {
                        $isplayerincomplete=1;
                    }
            }
            if ($isplayerincomplete==1)
                {
                    $to .= "$email".", ";
                }
        }
    }
    else /*send to all players*/
    {
        $query = "SELECT email FROM player WHERE email<>'admin'";
        $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query for mail.");
        while ($row=pg_fetch_assoc($result)) 
        {
            extract($row);
            $to .= "$email".", ";
        }
    }
    
    /*build subject line
    $query = "SELECT deadline_date FROM weeks WHERE num=$weeknum";
    $result = pg_exec($cxn, $query) or die ("Couldn't execute firstname, lastname lookup from player table for mail.");
    $row=pg_fetch_assoc($result);
    extract($row);
    
    $subject = "Week ".$weeknum." schedule. Please submit your picks by ".date('l\,\ M j',strtotime($deadline_date)).".";*/
    
    /*build body message with games for week*/
    $message = '
    <html>
    <head>
    </head>
    <body>';
    $message .= '<h1 style = "font-family: Arial, Helvetica, sans-serif; font-size: 12px; font-weight: normal;">'.$emailmessage.'</h1>';
    $message .= '
    <br/>
    <table style="border-collapse: collapse; font-family: Arial, Helvetica, sans-serif; font-size: 10px;">
    <thead>
        <tr style="border-bottom: 1px solid black;">
            <th style="background: blue; text-align:left; color: white; width: 20px;"></th>
            <th style="background: blue; text-align:left; color: white; width: 30px;">TB</th>
            <th style="background: blue; text-align:left; color: white; width: 75px;">Date</th>
            <th style="background: blue; text-align:left; color: white; width: 175px;">Team</th>
            <th style="background: blue; text-align:left; color: white; width: 175px;">Opponent</th>
        </tr>
    </thead>
    <tbody>';
        $counter=0;
        /*display scheduled games and values from games table*/ 
        $query = "SELECT * FROM games WHERE week_num=$weeknum AND locale<>0 ORDER BY gamedate"; /* */
        $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query.");
        while ($row=pg_fetch_assoc($result)) 
        {
            extract($row);
            $counter=$counter+1;
            $message .= "<tr style='border-bottom: 1px solid black;'>";
                /*display game number*/
                $message .= "<td>$counter.</td>\n";
                
                /*display tiebreaker game with asterisk in column*/
                $message .=  "<td>";
                if ($tiebreaker==1)
                {
                    $message .= "<strong>*</strong>";
                }
                $message .= "</td>";
                
                /*display date for each game*/
                $message .=  "<td>".date('M j',strtotime($gamedate))."</td>\n";
                
                /*determine Team name*/
                if ($locale==1) {$homeat="at";} else {$homeat="";}
                $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
                $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
                $trow=pg_fetch_assoc($tresult);
                extract($trow);
                $playerteam=$team_name;
                
                /*determine Opponent team name*/
                if ($locale==2) {$awayat="at";} else {$awayat="";}
                $oquery = "SELECT team_name FROM teams WHERE id=$opponent_id";
                $oresult = pg_exec($cxn, $oquery) or die ("Couldn't execute team lookup query.");
                $orow=pg_fetch_assoc($oresult);
                extract($orow);
                $opponentteam=$team_name;

                
                /*display Team and Opponent with proper styling*/
                $message .=  "<td>$homeat $playerteam</td>\n";
                $message .=  "<td>$awayat $opponentteam</td></tr>\n";
        }
        /*display games and values from games table for teams without games (locale=0)*/
        $query = "SELECT * FROM games WHERE week_num=$weeknum AND locale=0"; /* */
        $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query.");
        while ($row=pg_fetch_assoc($result))  
        {
            extract($row);
            $counter=$counter+1;
            $message .= "<tr>";
            $message .= "<td>$counter.</td>\n";
            /*display tiebreaker game with asterisk in column*/
            $message .= "<td></td>\n";
            /*display date for each game*/
            $message .= "<td></td>\n";
            /*display team name*/
            $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
            $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
            $trow=pg_fetch_assoc($tresult);
            extract($trow);
            $message .= "<td>$team_name</td>\n";
            /*display that there is no game this week for this Team*/
            $message .= "<td><span style=\"font-style:italic\">No game this week</span></td></tr>\n\n";
        }
    $message .= '
    </tbody>
    </table>
    <h1 style = "font-family: Arial, Helvetica, sans-serif; font-size: 10px; font-weight: bold;">Login to <a href="http://www.ncaahoopspool.com">ncaahoopspool.com</a> to make your picks.</h1>
    </body>
    </html>';
    
    /*build additional headers*/
    $headers  = 'MIME-Version: 1.0' . "\r\n";
    $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
    $headers .= 'From: John Judd <john.judd@linde.com>' . "\r\n";
    echo $to;

    /*mail the message*/
    mail($to, $emailsubject, $message, $headers);
    /*mail("paulrmaurer@yahoo.com", $emailsubject, $message, $headers);*/
    /*echo $emailsubject;*/
    /*echo $message;*/
}
?>