<!--block of code to display players names who have not submitted all of their picks yet-->

<?php
     $incompleteplayers = array();
     $counter=0;
     
     $query="SELECT id, firstname, lastname FROM player WHERE email<>'admin' ORDER BY lastname ASC";
     $result=pg_exec($cxn,$query) or die ("Could not execute players query");
     while ($row=pg_fetch_assoc($result))
     {
         $isplayerincomplete=0;
         extract($row);
         $picksquery="SELECT picks.pick FROM picks INNER JOIN games ON picks.game_id = games.id WHERE games.locale>0 AND games.week_num='$currentweekid' AND picks.player_id ='$id'";
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
                 $incompleteplayers['$counter']=$id;
                 $counter = $counter+1;
                 echo $firstname." ".$lastname."<br/>";
             }
     }
     echo "<br/>";
?>