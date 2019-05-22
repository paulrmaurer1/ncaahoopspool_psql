<!--block of code to display the Manage Games fields for admin user to choose game parameters-->
<table class="games">
    <thead>
        <tr>
            <th></th>
            <th class="tiebreaker">TB</th>
            <th class="date">Date</th>
            <th class="locale">Locale</th>
            <th class="team">Team</th>
            <th class="opponent">Opponent</th>
        </tr>
    </thead>
    <tbody>
        <?php
            $counter=0;
            $query = "SELECT * FROM games WHERE week_num=$currentweekid"; /* */
            $result = pg_exec($cxn, $query) or die ("Couldn't execute games table query.");
            while ($row=pg_fetch_assoc($result)) /*display existing games and values from games table   $result->fetch_assoc()*/ 
            {
                extract($row);
                $counter=$counter+1;
                echo "<input type='hidden' name='selections[$counter][gameid]' value=$id>";
                echo "<input type='hidden' name='currentweekid' value=$currentweekid>";
                echo "<tr>";
                echo "<td>$counter.&nbsp</td>\n";
                /*display tiebreaker game radio button with first game auto-selected for week*/
                if ($tiebreaker==1)
                {
                    $checkered="checked='checked'";
                }
                else if ($counter==1)
                {
                    $checkered="checked='checked'";
                }
                else
                {
                    $checkered="";
                }
                echo "<td><input type='radio' name='tbgame' value=$id ".$checkered."></td>";
                /*display date range for week in game date selection list box*/
                echo "<td><select name='selections[$counter][date]'>";
                $begin=strtotime($from_date);
                $end=strtotime($to_date);
                do {
                    $displaydate=date('F jS Y',$begin);
                    $insertdate=date('Y-m-d',$begin);
                    if ($begin==strtotime($gamedate))
                    {
                        echo "<option selected='selected' value=$insertdate>$displaydate</option>\n";
                    }
                    else
                    {
                        echo "<option value=$insertdate>$displaydate</option>\n";
                    }                    
                    $begin=strtotime('+1 day',$begin);
                } while ($begin<=$end);
                echo "</select></td>\n";
                /*display locale radio button options for each game*/
                echo "<td><input type='radio' name='selections[$counter][locale]' id='home' value=1 ";
                    if ($locale==1) {echo "checked='checked'>\n";} else {echo ">\n";}
                echo "<label for='home' class='radiolabel'>Home</label><br/>";
                echo "<input type='radio' name='selections[$counter][locale]' id='away' value=2 ";
                    if ($locale==2) {echo "checked='checked'>\n";} else {echo ">\n";}
                echo "<label for='away' class='radiolabel'>Away</label><br/>";
                echo "<input type='radio' name='selections[$counter][locale]' id='neutral' value=3 ";
                    if ($locale==3) {echo "checked='checked'>\n";} else {echo ">\n";}
                echo "<label for='neutral' class='radiolabel'>Neutral</label><br/>";
                echo "<input type='radio' name='selections[$counter][locale]' id='nogame' value=0 ";
                    if ($locale==0) {echo "checked='checked'>\n";} else {echo ">\n";}
                echo "<label for='nogame' class='radiolabel'>No game</label><br/></td>";
                /*display team name*/
                $tquery = "SELECT team_name FROM teams WHERE id=$team_id";
                $tresult = pg_exec($cxn, $tquery) or die ("Couldn't execute team lookup query.");
                $trow=pg_fetch_assoc($tresult);
                extract($trow);
                echo "<td>$team_name</td>\n";
                /*display opponent drop down list*/
                echo "<td>vs.&nbsp<select name='selections[$counter][opponent_id]'>";
                $query="SELECT * FROM teams ORDER BY team_name ASC";
                $oppresult=pg_exec($cxn,$query) or die ("Could not execute query");
                while ($opprow=pg_fetch_assoc($oppresult))
                {
                    extract($opprow);
                    if ($row['opponent_id']==$opprow['id'])
                    {
                        $seltext='selected="selected" ';
                    }
                    else
                    {
                        $seltext='';
                    }
                    echo "<option ".$seltext." value=$id>$team_name</option>\n";
                }
                echo "</select></td>\n";
                echo "</tr>\n";
            }
            /*display date range for week in deadline date selection list box*/
            echo "<p><strong>Deadline Date: </strong>";
            echo "<select name='deadlinedate'>";
            $begin=strtotime($from_date);
            $end=strtotime($to_date);
            do {
                $displaydate=date('F jS Y',$begin);
                $insertdate=date('Y-m-d',$begin);
                if ($begin==strtotime($deadline_date))
                {
                    echo "<option selected='selected' value=$insertdate>$displaydate</option>\n";
                }
                else
                {
                    echo "<option value=$insertdate>$displaydate</option>\n";
                }                    
                $begin=strtotime('+1 day',$begin);
            } while ($begin<=$end);
            echo "</select>";
            echo " (players must submit picks by 12:00PM EST on this date)</p>\n";
               
        ?>
    </tbody>
</table>
<!--<p class="submit"><input type="submit" class="savebuttons" name="display_button" value="Save"/>-->
<p class="submit"></p><input type="submit" class="savebuttons" name="display_button" value="Save and View"/></p>

