<!--PHP program that checks Registration fields -->

<?php
function checkreginfo($formfields)
{   require ("misc.inc");
    
    $firstpassword="";
    $num=0; /*check var for existing email address*/
    $num2=0; /*check var for same password*/
    $num3=0; /*check var for existing password*/
    
    foreach ($formfields as $field => $value)
    {
        /*check each field for blank values*/
        if (preg_match("/registeremail/i",$field) or preg_match("/registerpassword/i",$field) or preg_match("/register2password/i",$field))
        {
            /*echo $field,":",$value, "<br/>";*/
            if (empty($value))
            {
                $blank_array[] = $field;
            }
        }
        /*check email field matches a player email*/
        if(preg_match("/registeremail/i",$field))
        {
            $query = "SELECT id FROM player WHERE email='$value'";
            $result = pg_exec($cxn,$query) or die ("Could not run check player email query");  
            $num=pg_numrows($result);
            $query = "SELECT password FROM player WHERE email='$value'";
            $result = pg_exec($cxn,$query) or die ("Could not run check player email query");  
            $pwordexist=pg_fetch_row($result);
            /* echo "existing password = ".$pwordexist[0]; */
            if ($pwordexist[0]=='')
            {
                $num3=1;
            }
            /*echo "result: ",$num;*/
        }
        if(preg_match("/registerpassword/i",$field))
        {
            $firstpassword=$value;
        }
        if(preg_match("/register2password/i",$field) and $firstpassword<>"")
        {
            if ($value==$firstpassword)
            {
                $num2=1;
            }
        }
    }

/*if any fields were not OK, build error messages and exit*/
    global $message;  /*string variable to store message to display to user, either good or bad*/
    $message="<p "; 
    if (@sizeof($blank_array) > 0 or $num==0 or $num2==0 or $num3==0)
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
            if ($num2==0)
            {
                /*display message for passwords not matching*/
                $message .= "Please enter the same password in both password input fields<br/>";
            }
            if ($num3==0)
            {
                /*display message that password already exists and should login*/
                $message .= "A password already exists for this email address.  Please Login instead.<br/>";
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