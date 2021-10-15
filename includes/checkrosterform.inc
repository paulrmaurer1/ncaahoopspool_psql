<!--PHP program that checks Roster fields captures display messages -->

<?php
function checkrosterform($formfields)
{   require ("misc.inc");
    
    $blank_array=[]; /*create empty array to store form values*/
    $bad_format=[]; /*create empty array to store bad form values*/
    $dup_format=[]; /*create empty array to store duplicate form values*/
    
    foreach ($formfields as $field => $value)
    {
    /*echo $field,":",$value, "<br/>";*/
        /*check each field for blank values*/
        if(empty($value))
        {
            $blank_array[] = $field;
        }
        else
        {
            /*check that first,last name fields contains letters only*/
            if (preg_match("/name/i",$field))
            {
                if(!preg_match("/^[A-Za-z -]+$/",$value))
                {
                    $bad_format[]=$field;
                }
            }
            /*check num_wins field contains numbers only*/
            if(preg_match("/num_wins/i",$field))
            {
                if(!preg_match("/^[0-9]/",$value))
                {
                    $bad_format[]=$field;
                }
            }
            /*check email field contains valid email and is not a duplicate*/
            if(preg_match("/email/i",$field))
            {
                if(!filter_var($value, FILTER_VALIDATE_EMAIL))
                {
                    $bad_format[]=$field;
                }
                /*check for existing email in player table*/
                else
                {
                    /*$query = "SELECT count(*) FROM player";*/
                    $query = "SELECT count(email) FROM player WHERE email='$value'";
                    $result = pg_exec($cxn,$query) or die ("Could not run duplicate email query");  
                    $row=pg_fetch_row($result);
                    /*echo "result: ",$row[0];*/
                    if ($row[0] > 0)
                    {
                        $dup_format[]=$field;
                    }
                }
            }
            /*check that team is not already chosen*/
            if(preg_match("/team/i",$field))
            {
                $query = "SELECT count(team_id) FROM player WHERE team_id=$value";
                $result = pg_exec($cxn,$query) or die ("Could not run duplicate team query");
                $row=pg_fetch_row($result);
                if ($row[0] > 0)
                {
                    $dup_format[]=$field;
                }
            }
        }
    }

/*if any fields were not OK, build error mssages and exit*/
    global $message;  /*string variable to store message to display to user, either good or bad*/
    $message="<p "; 
    if (@sizeof($blank_array) > 0 or @sizeof($bad_format) > 0 or @sizeof($dup_format) > 0)
    {
        $message.= 'class="errors">';
        if (@sizeof($blank_array) > 0)
        {
            /*display message for missing information*/
            $message .= "You didn't fill out one or more required fields.  You must enter:<br/>";
            foreach($blank_array as $value)
            {
                $message .= "&nbsp;&nbsp;&nbsp;$value<br/>";
            }
        }
        if (@sizeof($bad_format) > 0)
        {
            /*display message for bad format information*/
            $message .= "One or more fields have information that appears to be incorrect. Correct the format for:<br/>";
            foreach($bad_format as $value)
            {
                $message .= "&nbsp;&nbsp;&nbsp;$value<br/>";
            }
        }
        if (@sizeof($dup_format) > 0)
        {
            /*display message for duplicate information*/
            $message .=  "Another player already has been entered with the same fields. Input differently for:<br/>";
            foreach($dup_format as $value)
            {
                $message .=  "&nbsp;&nbsp;&nbsp;$value<br/>";
            }
        }
        $message .=  "</p>";
        return FALSE;
    }
    else
    {
        $message .= 'class="success">';
        $message .= "Player has been added to the roster<br/>";
        $message .= "</p>";
        return TRUE;
    }
}
?>