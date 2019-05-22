<!--PHP program that checks Login fields -->

<?php
function checklogininfo($formfields)
{   require ("misc.inc");
            
    $num2=1; /*if password isn't submitted with form (forgetpassword.php) then make sure $num2 doesn't cause error messages*/

    foreach ($formfields as $field => $value)
    {
        /*check each field for blank values*/
        
        if (preg_match("/loginemail/i",$field) or preg_match("/loginpassword/i",$field))
        {
            /*echo $field,":",$value, "<br/>";*/
            if (empty($value))
            {
                $blank_array[] = $field;
            }
        }
        /*check email field matches a player email*/
        if(preg_match("/loginemail/i",$field))
        {
            $query = "SELECT id FROM player WHERE email='$value'";
            $result = pg_exec($cxn,$query) or die ("Could not run check player email query");  
            $num=pg_numrows($result);
            /*echo "result: ",$num;*/
        }
        /*check that password matches account*/
        if(preg_match("/loginpassword/i",$field))
        {
            $query = "SELECT id FROM player WHERE password='$value'";
            $result = pg_exec($cxn,$query) or die ("Could not run check player password query");
            $num2=pg_numrows($result);
        }
    }

/*if any fields were not OK, build error messages and exit*/
    global $message;  /*string variable to store message to display to user, either good or bad*/
    $message="<p "; 
    if (@sizeof($blank_array) > 0 or $num==0 or $num2==0)
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
        else
        {
            if ($num==0)
            {
                /*display message for no email match information*/
                $message .= "There is no account with that email address<br/>";
            }
            else
            {
                if ($num2==0)
                {
                    /*display message for bad password*/
                    $message .=  "This email account exists but the password is wrong.<br/>Please re-enter or click the 'Forget Password' link below.<br/>";
                }
            }
        }
        $message .=  "</p>";
        return FALSE;
    }
    else
    {
        /*$message .= 'class="success">';
        $message .= "Player has been added to the roster<br/>";
        $message .= "</p>";*/
        return TRUE;
    }
}
?>